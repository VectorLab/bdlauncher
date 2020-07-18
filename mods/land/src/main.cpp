#include <cstdio>
#include <forward_list>
#include <list>
#include <string>
#include <sys/cdefs.h>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

#include <cmath>
#include <deque>
#include <dlfcn.h>
#include <fstream>
#include <tuple>
#include <unistd.h>

#include <minecraft/actor/ActorDamageSource.h>
#include <minecraft/actor/Player.h>
#include <minecraft/block/BlockPos.h>
#include <minecraft/core/GameMode.h>
#include <minecraft/core/access.h>
#include <minecraft/core/getSP.h>
#include <minecraft/item/ItemStack.h>
#include <minecraft/json.h>
#include <minecraft/level/Level.h>

#include <mlr/modcontext.h>
#include <mlr/modmain.h>
#include <mlr/statichook.h>

#include <dbimpl.h>
#include <PlayerMap.h>
#include <base.h>
#include <cmdhelper.h>
#include <gui.h>
#include <money.h>

#include "land.command.h"

using std::deque;
using std::list;
using std::string;
using std::swap;
using std::unordered_map;

static LDBImpl db;
static std::filesystem::path pathdata;
static std::filesystem::path pathconfig;
static std::filesystem::path pathdb;

enum LandPerm : char {
  PERM_NULL = 0,
  PERM_OWNER = 1,
  PERM_USE = 2,
  PERM_ATK = 4,
  PERM_BUILD = 8,
  PERM_POPITEM = 16,
  PERM_INTERWITHACTOR = 32,
  PERM_ADMIN_FLY = 64
};
typedef unsigned int lpos_t;
struct FastLand {
  lpos_t x, z, dx, dz;
  uint lid;
  short refcount;
  char dim;
  LandPerm perm;
  int owner_sz;
  char owner[0];
  int chkOwner(string_view x) {
    // x="|"+x+"|";
    int sz = x.size();
    auto dat = x.data();
    // do_log("owner sz %d",owner_sz);
    for (int i = 1; i < owner_sz - sz; ++i) {
      if (memcmp(dat, owner + i, sz) == 0 && owner[i - 1] == '|' &&
          owner[i + sz] == '|') {
        return i == 1 ? 2 : 1;
      }
    }
    return 0;
  }
  inline bool hasPerm(string_view x, LandPerm PERM) {
    if (PERM == PERM_OWNER) {
      return chkOwner(x) == 2;
    }
    return chkOwner(x) ? true : (perm & PERM);
  }
  int memsz() { return sizeof(FastLand) + owner_sz + 1; }
  inline string_view getOwner() { return {owner, (size_t)owner_sz}; }
};
static_assert(sizeof(FastLand) == 28);
static_assert(offsetof(FastLand, dim) == 22);
static_assert(offsetof(FastLand, refcount) == 20);
struct DataLand {
  lpos_t x, z, dx, dz;
  uint lid;
  short ver;
  char dim;
  LandPerm perm;
  string owner;
  DataLand() {}
  DataLand(FastLand &fastland) {
    x = fastland.x;
    z = fastland.z;
    dx = fastland.dx;
    dz = fastland.dz;
    lid = fastland.lid;
    ver = fastland.refcount;
    dim = fastland.dim;
    perm = fastland.perm;
    owner = fastland.getOwner();
  }
  void addOwner(string_view x, bool SuperOwner = false) {
    SPBuf sb;
    if (SuperOwner) {
      sb.write("|");
      sb.write(x);
      sb.write("|%s", owner.c_str());
    } else {
      sb.write("%s|", owner.c_str());
      sb.write(x);
      sb.write("|");
    }
    owner = sb.get();
  }
  void delOwner(string_view x) {
    SPBuf sb;
    sb.write("|");
    sb.write(x);
    sb.write("|");
    auto sv = sb.get();
    auto pos = owner.find(sv);
    if (pos != string::npos)
      owner.erase(pos, sv.size());
  }
  void packto(DataStream &ds) const {
    ds << x << z << dx << dz << lid << ver << dim << perm << owner; // version
  }
  void unpack(DataStream &ds) {
    ds >> x >> z >> dx >> dz >> lid >> ver >> dim >> perm >>
        owner; // refcount=version
  }
};
static_assert(sizeof(DataLand) == 24 + sizeof(string));
static_assert(offsetof(DataLand, dim) == 22);
static struct LandCacheManager {
  unordered_map<int, FastLand *> cache;
  void noticeFree(FastLand *fl) {
    fl->refcount--;
    if (fl->refcount <= 0) {
      cache.erase(fl->lid);
      delete fl;
    }
  }
  FastLand *requestLand(int id) {
    // do_log("req %d",id);
    auto it = cache.find(id);
    FastLand *res;
    if (it == cache.end()) {
      string landstr;
      char buf[6];
      buf[0] = 'l';
      buf[1] = '_';
      memcpy(buf + 2, &id, 4);
      db.Get(string_view(buf, 6), landstr);
      res = (FastLand *)malloc(landstr.size() + 1);
      memcpy(res, landstr.data(), landstr.size());
      res->owner[res->owner_sz] = 0;
      res->refcount = 0;
      cache[id] = res;
    } else {
      res = it->second;
    }
    res->refcount++;
    // do_log("req %d rcnt %d lp %p",id,res->refcount,res);
    return res;
  }
} LCMan;
struct ChunkLandManager {
  lpos_t xx, zz;
  char dim;
  FastLand *lands[16][16];
  static_deque<FastLand *, 256> managed_lands;
  void reset() {
    if (managed_lands.size() != 0)
      memset(lands, 0, sizeof(lands));
    for (decltype(managed_lands.tail) i = managed_lands.head;
         i != managed_lands.tail; ++i) {
      LCMan.noticeFree(managed_lands[i]);
    }
    managed_lands.clear();
  }
  void init(int *landlist, int siz) {
    for (int I = 0; I < siz; ++I) {
      auto fl = LCMan.requestLand(landlist[I]);
      managed_lands.push_back(fl);
      lpos_t sx, dx, sz, dz;
      if ((fl->x >> 4) == xx)
        sx = fl->x & 15;
      else
        sx = 0;
      if ((fl->z >> 4) == zz)
        sz = fl->z & 15;
      else
        sz = 0;
      if ((fl->dx >> 4) == xx)
        dx = fl->dx & 15;
      else
        dx = 15;
      if ((fl->dz >> 4) == zz)
        dz = fl->dz & 15;
      else
        dz = 15;
      for (lpos_t i = sx; i <= dx; ++i) {
        for (lpos_t j = sz; j <= dz; ++j) {
          lands[i][j] = fl;
        }
      }
    }
  }
  void load(lpos_t x, lpos_t z, int di) {
    string val;
    char buf[9];
    xx = x;
    zz = z;
    dim = di;
    memcpy(buf, &x, 4);
    memcpy(buf + 4, &z, 4);
    buf[8] = dim;
    db.Get(string_view(buf, 9), val);
    init((int *)val.data(), val.size() / 4);
  }
};
template <int CACHE_SZ> struct CLCache {
  struct Data {
    short prev;
    short next;
    short idx;
    unsigned long hash;
  };
  ChunkLandManager cm[CACHE_SZ];
  Data pool[CACHE_SZ + 1];
  CLCache() {
    for (int i = 0; i <= CACHE_SZ; ++i) {
      pool[i].prev = i - 1;
      pool[i].next = i + 1;
      pool[i].idx = i - 1;
      pool[i].hash = 0xfffffffffffffffful;
    }
    pool[0].prev = CACHE_SZ;
    pool[CACHE_SZ].next = 0;
    for (int i = 0; i < CACHE_SZ; ++i) {
      cm[i].reset();
    }
  }
  inline void detach(int idx) {
    auto &now = pool[idx];
    pool[now.prev].next = now.next;
    pool[now.next].prev = now.prev;
  }
  inline void insert_after(int pos, int idx) {
    auto &now = pool[idx];
    now.prev = pos;
    now.next = pool[pos].next;
    pool[now.next].prev = idx;
    pool[pos].next = idx;
  }
  inline ChunkLandManager *get_or_build(lpos_t x, lpos_t z, char dim) {
    int nowidx;
    unsigned long hash =
        (((unsigned long)x) << 32) | (((unsigned long)dim) << 29) | z;
    for (nowidx = pool[0].next; nowidx != 0; nowidx = pool[nowidx].next) {
      auto &now = pool[nowidx];
      if (now.hash == hash) {
        // found
        detach(nowidx);
        insert_after(0, nowidx);
        return cm + now.idx;
      }
    }
    // not found;
    int tailidx = pool[0].prev;
    detach(tailidx);
    insert_after(0, tailidx);
    auto &now = pool[tailidx];
    now.hash = hash;
    cm[now.idx].reset();
    cm[now.idx].load(x, z, dim);
    return cm + now.idx;
  }
  inline void purge() {
    for (int i = 1; i <= CACHE_SZ; ++i) {
      cm[pool[i].idx].reset();
      pool[i].hash = 0xfffffffffffffffful;
    }
  }
};
static CLCache<160> CLMan;
static inline void purge_cache() { CLMan.purge(); }
static inline ChunkLandManager *getChunkMan(lpos_t x, lpos_t z, int dim) {
  return CLMan.get_or_build(x, z, dim);
}
static inline FastLand *getFastLand(int x, int z, int dim) {
  lpos_t xx, zz;
  xx = (x) ^ 0x80000000;
  zz = (z) ^ 0x80000000;
  auto cm = getChunkMan(xx >> 4, zz >> 4, dim);
  return cm->lands[xx & 15][zz & 15];
}
static inline bool generic_perm(int x, int z, int dim, LandPerm perm,
                                const string &name) {
  auto ld = getFastLand(x, z, dim);
  if (unlikely(ld)) {
    return ld->hasPerm(name, perm);
  }
  return true;
}
static uint getLandUniqid() {
  string val;
  uint id = 0;
  if (!db.Get("land_id", val)) {
    db.Put("land_id", string((char *)&id, 4));
    return 0;
  } else {
    id = access(val.data(), uint, 0);
    id++;
    db.Put("land_id", string((char *)&id, 4));
    return id;
  }
}
static void proc_chunk_add(lpos_t x, lpos_t dx, lpos_t z, lpos_t dz, int dim,
                           uint lid) {
  char buf[9];
  x >>= 4;
  dx >>= 4;
  z >>= 4;
  dz >>= 4;
  buf[8] = dim;
  string_view key(buf, 9);
  for (auto i = x; i <= dx; ++i) {
    for (auto j = z; j <= dz; ++j) {
      memcpy(buf, &i, 4);
      memcpy(buf + 4, &j, 4);
      string val;
      db.Get(key, val);
      val.append((char *)&lid, 4);
      db.Put(key, val);
    }
  }
}
static void proc_chunk_del(lpos_t x, lpos_t dx, lpos_t z, lpos_t dz, int dim,
                           uint lid) {
  char buf[9];
  x >>= 4;
  dx >>= 4;
  z >>= 4;
  dz >>= 4;
  buf[8] = dim;
  string_view key(buf, 9);
  string val;
  for (decltype(dx) i = x; i <= dx; ++i) {
    for (decltype(dz) j = z; j <= dz; ++j) {
      memcpy(buf, &i, 4);
      memcpy(buf + 4, &j, 4);
      // do_log("proc del %d %d %d",i,j,dim);
      db.Get(key, val);
      // do_log("size %d access %u",val.size(),access(val.data(),uint,0));
      for (decltype(val.size()) i = 0; i < val.size(); i += 4) {
        if (access(val.data(), uint, i) == lid) {
          // do_log("erase %d",i);
          val.erase(i, 4);
          break;
        }
      }
      db.Put(key, val);
    }
  }
}
static void addLand(lpos_t x, lpos_t dx, lpos_t z, lpos_t dz, int dim,
                    const string &owner, LandPerm perm = PERM_NULL) {
  DataLand ld;
  ld.x = x, ld.z = z, ld.dx = dx, ld.dz = dz, ld.dim = dim;
  ld.owner = owner[0] == '|' ? owner : ('|' + owner + '|');
  ld.perm = perm;
  auto lid = getLandUniqid();
  ld.lid = lid;
  // string key="l_"+string((char*)&lid,4);
  char buf[6];
  buf[0] = 'l';
  buf[1] = '_';
  memcpy(buf + 2, &lid, 4);
  string_view key(buf, 6);
  DataStream ds;
  ds << ld;
  // do_log("sss %d",ds.dat.size());
  db.Put(key, ds.dat);
  proc_chunk_add(x, dx, z, dz, dim, lid);
  purge_cache();
}
static void updLand(DataLand &ld) {
  auto lid = ld.lid;
  ld.ver = 1; // NEW
  DataStream ds;
  char buf[6];
  buf[0] = 'l';
  buf[1] = '_';
  memcpy(buf + 2, &lid, 4);
  string_view key(buf, 6);
  ds << ld;
  db.Put(key, ds.dat);
  purge_cache();
}
static void removeLand(FastLand *land) {
  char buf[6];
  buf[0] = 'l';
  buf[1] = '_';
  memcpy(buf + 2, &land->lid, 4);
  string_view key(buf, 6);
  db.Del(key);
  proc_chunk_del(land->x, land->dx, land->z, land->dz, land->dim,
                 land->lid); // BUG HERE
  purge_cache();
}
void iterLands(function<void(DataLand &)> cb) {
  vector<pair<string, string>> newdata;
  db.Iter([&](string_view k, string_view v) {
    if (k.size() == 6 && k[0] == 'l' && k[1] == '_') {
      DataLand dl;
      DataStream ds;
      ds.dat = v;
      ds >> dl;
      cb(dl);
      ds.reset();
      ds << dl;
      newdata.emplace_back(k, ds.dat);
    }
  });
  for (auto &i : newdata) {
    db.Put(i.first, i.second);
  }
  purge_cache();
}
void iterLands_const(function<void(const DataLand &)> cb) {
  db.Iter([&](string_view k, string_view v) {
    if (k.size() == 6 && k[0] == 'l' && k[1] == '_') {
      DataLand dl;
      DataStream ds;
      ds.dat = v;
      ds >> dl;
      cb(dl);
    }
  });
}

void CHECK_AND_FIX_ALL() {
  do_log("start data fix&optimize");
  vector<pair<string, DataLand>> lands;
  string val;
  uint lids;
  if (!db.Get("land_id", val))
    lids = 0;
  else
    lids = access(val.data(), uint, 0);
  db.Iter([&](string_view k, string_view v) {
    if (k.size() == 6 && k[0] == 'l' && k[1] == '_') {
      DataLand dl;
      DataStream ds;
      ds.dat = v;
      ds >> dl;
      lands.emplace_back(k, dl);
    }
  });
  db.close();
  setenv("LD_PRELOAD", "", 1);
  std::filesystem::path pathdbbak = pathdata;
  pathdbbak /= "land_old";
  std::filesystem::remove_all(pathdbbak);
  std::filesystem::rename(pathdb, pathdbbak);
  db.load(pathdb.c_str(), true, 1048576 * 8); // 8MB Cache
  db.Put("land_id", string_view((char *)&lids, 4));
  do_log("%ld lands found!", lands.size());
  for (auto &nowLand : lands) {
    DataStream ds;
    auto &Land = nowLand.second;
    if (Land.ver == 0) {
      do_log("upgrade land %d", Land.lid);
      Land.ver = 1;
      int xx = Land.x - 200000;
      int zz = Land.z - 200000;
      Land.x = xx ^ (0x80000000);
      Land.z = zz ^ (0x80000000);
      xx = Land.dx - 200000;
      zz = Land.dz - 200000;
      Land.dx = xx ^ (0x80000000);
      Land.dz = zz ^ (0x80000000);
    }
    ds << Land;
    db.Put(nowLand.first, ds.dat);
    proc_chunk_add(Land.x, Land.dx, Land.z, Land.dz, Land.dim, Land.lid);
  }
  db.CompactAll();
  do_log("Done land data fix: %ld!", lands.size());
}

struct LPOS {
  int x, z;
};

static std::unordered_map<string, LPOS> startpos, endpos;
// static unordered_map<string, int> choose_state;
static PlayerMap<int> choose_state;
int LAND_PRICE, LAND_PRICE2;
static bool land_tip = true;
static void loadcfg() {
  std::ifstream ifs{pathconfig.c_str()};
  Json::Value value;
  Json::Reader reader;
  if (!reader.parse(ifs, value)) {
    auto msg = reader.getFormattedErrorMessages();
    do_log("%s", msg.c_str());
    exit(1);
  }
  LAND_PRICE = value["buy_price"].asInt(0);
  LAND_PRICE2 = value["sell_price"].asInt(0);
  land_tip = value["land_tip"].asBool(land_tip);
  if (getenv("NO_LTIP"))
    land_tip = false;
}
void LDCommand::exit(mandatory<Exit> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  choose_state.defe(sp);
  getOutput().success("§bExit selection mode, please input /land buy");
}
void LDCommand::AB_(mandatory<AB> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  choose_state[sp] = (cmd == AB::b) + 1;
  getOutput().success(
      "§bEnter selection mode, please click on the ground to select point");
}
void LDCommand::query(mandatory<LandQuery> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  auto &pos = sp->getPos();
  int dim = sp->getDimensionId();
  auto lp = getFastLand(pos.x, pos.z, dim);
  if (lp) {
    char buf[1000];
    snprintf(buf, 1000, "§bThis is %s's land", lp->owner);
    getOutput().success(buf);
  } else {
    getOutput().error("No land here");
    return;
  }
}
void LDCommand::buy(mandatory<Buy> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  SPBuf sb;
  //  auto op    = sp->getPlayerPermissionLevel() > 1;
  auto &nm = sp->getNameTag();
  auto &hash = nm;
  int x, z, dx, dz;
  int dim = sp->getDimensionId();
  if (startpos.count(hash) + endpos.count(hash) != 2) {
    getOutput().error("Choose 2 points please.");
    return;
  }
  choose_state.defe(sp);
  x = std::min(startpos[hash].x, endpos[hash].x);
  z = std::min(startpos[hash].z, endpos[hash].z);
  dx = std::max(startpos[hash].x, endpos[hash].x);
  dz = std::max(startpos[hash].z, endpos[hash].z);
  // step 1 check money
  int deltax = dx - x + 1, deltaz = dz - z + 1;
  uint siz = deltax * deltaz;
  if (deltax >= 4096 || deltaz >= 4096 || siz >= 5000000) {
    getOutput().error("Too big land");
    return;
  }
  int price = siz * LAND_PRICE;
  if (price <= 0 || price >= 500000000) {
    getOutput().error("Too big land");
    return;
  }
  auto mon = money_get(nm);
  if (mon < price) {
    getOutput().error("Money not enough");
    return;
  }
  // step 2 check coll
  for (int i = x; i <= dx; ++i)
    for (int j = z; j <= dz; ++j) {
      auto lp = getFastLand(i, j, dim);
      if (lp) {
        sb.write("Land collision detected! hint: "sv);
        sb.write(lp->getOwner());
        sb.write("'s land"sv);
        getOutput().error(sb.getstr());
        return;
      }
    }
  // step 3 add land
  money_set(nm, mon - price);
  addLand((x) ^ 0x80000000, (dx) ^ 0x80000000, (z) ^ 0x80000000,
          (dz) ^ 0x80000000, dim, nm);
  getOutput().success("§bSuccessful land purchase");
}
void LDCommand::sell(mandatory<Sell> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  auto op = sp->getPlayerPermissionLevel() > 1;
  auto &nm = sp->getNameTag();
  auto &pos = sp->getPos();
  int dim = sp->getDimensionId();
  auto lp = getFastLand(pos.x, pos.z, dim);
  if (lp && (lp->chkOwner(nm) == 2 || op)) {
    int siz = (lp->dx - lp->x) * (lp->dz - lp->z);
    money_add(nm, siz * LAND_PRICE2);
    removeLand(lp);
    getOutput().success("§bYour land has been sold");
  } else {
    getOutput().error("No land here or not your land");
    return;
  }
}

void LDCommand::trust(mandatory<Trust> cmd, mandatory<std::string> target) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  auto op = sp->getPlayerPermissionLevel() > 1;
  auto &nm = sp->getNameTag();
  auto &pos = sp->getPos();
  int dim = sp->getDimensionId();
  auto lp = getFastLand(pos.x, pos.z, dim);
  if (lp && (lp->chkOwner(nm) == 2 || op)) {
    SPBuf sb;
    DataLand dl(*lp);
    dl.addOwner(target);
    updLand(dl);
    sb.write("§bMake ");
    sb.write(target);
    sb.write(" a trusted player");
    getOutput().success(sb.getstr());
  } else {
    getOutput().error("No land here or not your land");
    return;
  }
}
void LDCommand::untrust(mandatory<Untrust> cmd, mandatory<std::string> target) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  auto op = sp->getPlayerPermissionLevel() > 1;
  auto &nm = sp->getNameTag();
  auto &pos = sp->getPos();
  int dim = sp->getDimensionId();
  auto lp = getFastLand(pos.x, pos.z, dim);
  if (lp && (lp->chkOwner(nm) == 2 || op)) {
    SPBuf sb;
    DataLand dl{*lp};
    dl.delOwner(target);
    updLand(dl);
    sb.write("§bMake ");
    sb.write(target);
    sb.write(" a untrusted player");
    getOutput().success(sb.getstr());
  } else {
    getOutput().error("No land here or not your land");
    return;
  }
}
void LDCommand::perm(mandatory<Perm> cmd, mandatory<int> perm) {
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;
  auto op = sp->getPlayerPermissionLevel() > 1;
  auto &nm = sp->getNameTag();
  auto &pos = sp->getPos();
  int dim = sp->getDimensionId();
  auto lp = getFastLand(pos.x, pos.z, dim);
  if (lp && (lp->chkOwner(nm) == 2 || op)) {
    DataLand dl{*lp};
    auto pm = (LandPerm)perm;
    if ((pm & PERM_ADMIN_FLY) && !op) {
      getOutput().error("Permission denied");
      return;
    }
    dl.perm = pm;
    updLand(dl);
    getOutput().success("§bChange permissions successfully");
  } else {
    getOutput().error("No land here or not your land");
    return;
  }
}
void LDCommand::give(mandatory<Give> cmd, mandatory<std::string> target) {
  auto dst = getplayer_byname2(target);
  auto sp = getSP(getOrigin().getEntity());
  if (sp && dst) {
    auto &pos = sp->getPos();
    int dim = sp->getDimensionId();
    auto lp = getFastLand(pos.x, pos.z, dim);
    if (lp && (lp->chkOwner(sp->getNameTag()) == 2 ||
               sp->getPlayerPermissionLevel() > 1)) {
      DataLand dl{*lp};
      dl.addOwner(dst->getNameTag(), true);
      dl.delOwner(sp->getNameTag());
      updLand(dl);
      sendText(dst, "§bYou get a land from " + sp->getNameTag());
      getOutput().success("§bSuccessfully give your territory to " +
                          dst->getNameTag());
    } else {
      getOutput().error("No land here or not your land");
      return;
    }
  }
}
void LDCommand::trustgui(mandatory<Trustgui> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (sp) {
    gui_ChoosePlayer(sp, "Choose players to trust", "Trust",
                     [](ServerPlayer *xx, string_view dest) {
                       SPBuf sb;
                       sb.write("land trust \"");
                       sb.write(dest);
                       sb.write("\"");
                       runcmdAs(sb.get(), xx);
                     });
    getOutput().success();
  } else {
    getOutput().error("Error");
  }
}
void LDCommand::untrustgui(mandatory<Untrustgui> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  if (sp) {
    auto &pos = sp->getPos();
    int dim = sp->getDimensionId();
    auto lp = getFastLand(pos.x, pos.z, dim);
    if (lp && (lp->chkOwner(sp->getNameTag()) == 2 ||
               sp->getPlayerPermissionLevel() > 1)) {
      SharedForm *sf = getForm("Untrust a player", "Untrust a player");
      DataLand dl{*lp};
      static_deque<string_view> owners;
      split_string(dl.owner, owners, "||");
      for (auto &i : owners) {
        string_view x;
        if (i[0] == '|') {
          x = i.substr(1);
        } else {
          if (i[i.size() - 1] == '|') {
            x = i.substr(0, i.size() - 1);
          } else {
            x = i;
          }
        }
        sf->addButton(x);
      }
      sf->cb = [](ServerPlayer *sp, string_view c, int idx) {
        SPBuf<512> buf;
        buf.write("land untrust \"");
        buf.write(c);
        buf.write("\"");
        runcmdAs(buf.get(), sp);
      };
      sendForm(*sp, sf);
      getOutput().success();
    } else {
      getOutput().error("Error");
    }
  }
}
void LDOCommand::dumpall(mandatory<Dumpall> cmd) {
  iterLands_const([&](const DataLand &dl) {
    char buf[1000];
    snprintf(buf, 1000, "owner %s pos %d %d -> %d %d dim %d perm %d",
             dl.owner.c_str(), dl.x ^ 0x80000000, dl.z ^ 0x80000000,
             dl.dx ^ 0x80000000, dl.dz ^ 0x80000000, dl.dim, dl.perm);
    getOutput().addMessage(buf);
  });
  getOutput().success("okay");
}
void LDOCommand::forceperm(mandatory<Forceperm>, mandatory<int> newperm) {
  iterLands([newperm](DataLand &dl) { dl.perm = (LandPerm)newperm; });
  getOutput().success("okay");
}
void LDOCommand::fix(mandatory<Fix> cmd) {
  db.Del("land_fixed_v3");
  getOutput().success("scheduled data fix at next server start");
}
void LDOCommand::reload(mandatory<LandReload> cmd) {
  loadcfg();
  getOutput().success("Reloaded");
}

static void NoticePerm(FastLand *fl, ServerPlayer *sp) {
  SPBuf sb;
  sb.write("§cThis is "sv);
  sb.write(fl->getOwner());
  sb.write("'s land"sv);
  sendText(sp, sb.get(), POPUP);
}
static bool handle_dest(GameMode *a0, BlockPos const *a1) {
  ServerPlayer *sp = a0->getPlayer();
  if (isOp(sp))
    return 1;
  int x(a1->x), z(a1->z), dim(sp->getDimensionId());
  FastLand *fl = getFastLand(x, z, dim);
  if (!fl || fl->hasPerm(sp->getNameTag(), PERM_BUILD)) {
    return 1;
  } else {
    NoticePerm(fl, sp);
    return 0;
  }
}
static bool handle_attack(Actor &vi, ActorDamageSource const &src, int &val) {
  if (src.isChildEntitySource() || src.isEntitySource()) {
    auto id = src.getEntityUniqueID();
    auto ent = getSrvLevel()->fetchEntity(id, false);
    ServerPlayer *sp = getSP(ent);
    if (!sp || isOp(sp))
      return 1;
    auto &pos = vi.getPos();
    int x(pos.x), z(pos.z), dim(vi.getDimensionId());
    FastLand *fl = getFastLand(x, z, dim);
    if (!fl || fl->hasPerm(sp->getNameTag(), PERM_ATK)) {
      return 1;
    } else {
      NoticePerm(fl, sp);
      return 0;
    }
  }
  return 1;
}
static bool handle_inter(GameMode *a0, Actor &a1) {
  ServerPlayer *sp = a0->getPlayer();
  if (isOp(sp))
    return 1;
  auto &pos = a1.getPos();
  int x(pos.x), z(pos.z), dim(a1.getDimensionId());
  FastLand *fl = getFastLand(x, z, dim);
  if (!fl || fl->hasPerm(sp->getNameTag(), PERM_INTERWITHACTOR)) {
    return 1;
  } else {
    NoticePerm(fl, sp);
    return 0;
  }
}
static bool handle_useion(GameMode *a0, ItemStack *a1, BlockPos const *a2,
                          BlockPos const *dstPos, Block const *a5) {
  ServerPlayer *sp = a0->getPlayer();
  auto &hash = sp->getNameTag();
  if (choose_state[sp] != 0) {
    if (choose_state[sp] == 1) {
      startpos[hash] = {a2->x, a2->z};
      sendText(sp, "§bPoint A selected");
    }
    if (choose_state[sp] == 2) {
      endpos[hash] = {a2->x, a2->z};
      char buf[1000];
      auto siz = (abs(startpos[hash].x - endpos[hash].x) + 1) *
                 (abs(startpos[hash].z - endpos[hash].z) + 1);
      snprintf(buf, 1000, "§bPoint B selected,size: %d price: %d", siz,
               siz * LAND_PRICE);
      sendText(sp, buf);
    }
    return 0;
  }
  if (isOp(sp))
    return 1;
  int x(a2->x), z(a2->z), dim(sp->getDimensionId());
  LandPerm pm;
  if (a1->isNull()) {
    pm = PERM_USE;
  } else {
    pm = PERM_BUILD;
  }
  FastLand *fl = getFastLand(x, z, dim),
           *fl2 = getFastLand(dstPos->x, dstPos->z, dim);
  auto &name = sp->getNameTag();
  if (fl && !fl->hasPerm(name, pm)) {
    NoticePerm(fl, sp);
    return 0;
  }
  if (fl2 && !fl2->hasPerm(name, pm)) {
    NoticePerm(fl2, sp);
    return 0;
  }
  return 1;
}
static bool handle_popitem(ServerPlayer &sp, BlockPos &bpos) {
  if (isOp(&sp))
    return 1;
  int x(bpos.x), z(bpos.z), dim(sp.getDimensionId());
  FastLand *fl = getFastLand(x, z, dim);
  if (!fl || fl->hasPerm(sp.getNameTag(), PERM_POPITEM)) {
    return 1;
  } else {
    NoticePerm(fl, &sp);
    return 0;
  }
}
THook(void *, _ZNK14ItemFrameBlock6attackEP6PlayerRK8BlockPos, void *a0,
      ServerPlayer &sp, BlockPos &bpos) {
  if (isOp(&sp))
    return original(a0, sp, bpos);
  int x(bpos.x), z(bpos.z), dim(sp.getDimensionId());
  FastLand *fl = getFastLand(x, z, dim);
  if (!fl || fl->hasPerm(sp.getNameTag(), PERM_POPITEM)) {
    return original(a0, sp, bpos);
  } else {
    NoticePerm(fl, &sp);
    return NULL;
  }
}

#ifdef LAND_TIP_CRASH
static unordered_map<string, string> lastland;
static unordered_set<string> flying;
THook(void *, _ZN12ServerPlayer9tickWorldERK4Tick, ServerPlayer *sp,
      unsigned long const *tk) {
  if (!land_tip)
    return original(sp, tk);
  if (*tk % 16 == 0) {
    auto &plyname = sp->getNameTag();
    auto &oldname = lastland[plyname];
    auto &pos = sp->getPos();
    int dim = sp->getDimensionId();
    FastLand *fl = getFastLand(round(pos.x), round(pos.z), dim);
    string_view newname;
    if (fl) {
      newname = string_view(fl->owner, fl->owner_sz);
      /*if((fl->perm&PERM_ADMIN_FLY) && fl->chkOwner(plyname)){
          if(!flying.count(plyname)){
              flying.insert(plyname);
          }
      }*/
    }
    if (oldname != newname) {
      SPBuf sb;
      if (newname == "") {
        sb.write("You lefted "sv);
        sb.write(string_view(oldname));
        sb.write("'s land!"sv);
      } else {
        sb.write("You entered "sv);
        sb.write(newname);
        sb.write("'s land!"sv);
      }
      sendText(sp, sb.get(), TextType::TIP);
      oldname = newname;
    }
  }
  return original(sp, tk);
}
#endif

void onLoad(ModContext *_mContext) {
  pathdata = _mContext->datadir;
  pathconfig = pathdata;
  pathconfig /= "land.json";
  pathdb = pathdata;
  pathdb /= "land";

  db.load(pathdb.c_str(), true, 1048576 * 8);

  loadcfg();
  string dummy;
  if (!db.Get("land_fixed_v3", dummy)) {
    CHECK_AND_FIX_ALL();
    db.Put("land_fixed_v3", dummy);
  }
  register_commands();
  reg_destroy(handle_dest);
  reg_useitemon(handle_useion);
  reg_actorhurt(handle_attack);
  reg_popItem(handle_popitem);
  reg_interact(handle_inter);
};
