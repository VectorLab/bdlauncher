#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <forward_list>
#include <fstream>
#include <list>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include <cmdhelper.h>
#include <mlr/modcontext.h>
#include <mlr/modmain.h>
#include <mlr/statichook.h>
#include <minecraft/actor/Player.h>
#include <minecraft/core/getSP.h>
#include <minecraft/json.h>

#include "tp.command.h"
#include <PlayerMap.h>
#include <base.h>
#include <dbimpl.h>
#include <gui.h>

using std::forward_list;
using std::string;
using std::unordered_map;
//#define dbg_printf(...) {}
#define dbg_printf do_log

static bool CanBack = true, CanHome = true, CanTP = true;
static int MaxHomes = 5;
struct Vpos {
  int x, y, z, dim;
  string name;
  Vpos() {}
  Vpos(int a, int b, int c, int d, string_view e) {
    x = a, y = b, z = c, dim = d, name = e;
  }
  void packto(DataStream &ds) const { ds << x << y << z << dim << name; }
  void unpack(DataStream &ds) { ds >> x >> y >> z >> dim >> name; }
  void tele(Actor &ply) const {
    TeleportA(ply, {(float)x, (float)y, (float)z}, {dim});
  }
};
static list<string> warp_list;
static unordered_map<string, Vpos> warp;
static LDBImpl tp_db;
struct home {
  list<Vpos> vals;
  void packto(DataStream &ds) const { ds << vals; }
  void unpack(DataStream &ds) { ds >> vals; }
  home(ServerPlayer &sp) {
    DataStream ds;
    if (tp_db.Get("home_" + sp.getName(), ds.dat)) {
      ds >> *this;
    }
  }
  void save(ServerPlayer &sp) {
    DataStream ds;
    ds << *this;
    tp_db.Put("home_" + sp.getName(), ds.dat);
  }
};

void add_warp(int x, int y, int z, int dim, const string &name) {
  warp_list.push_back(name);
  Vpos ps;
  ps.x = x, ps.y = y, ps.z = z, ps.dim = dim, ps.name = name;
  warp[name] = ps;
  DataStream ds;
  ds << ps;
  tp_db.Put("warp_" + name, ds.dat);
  ds.reset();
  ds << warp_list;
  tp_db.Put("warps", ds.dat);
}
void del_warp(const string &name) {
  warp_list.remove(name);
  DataStream ds;
  ds << warp_list;
  tp_db.Put("warps", ds.dat);
  tp_db.Del("warp_" + name);
}
void load_warps_new() {
  DataStream ds;
  if (!tp_db.Get("warps", ds.dat))
    return; // fix crash
  ds >> warp_list;
  do_log("%ld warps found", warp_list.size());
  for (auto &i : warp_list) {
    DataStream tmpds;
    tp_db.Get("warp_" + i, tmpds.dat);
    tmpds >> warp[i];
  }
}
/*
static unordered_map<string, home> home_cache;
static home &getHome(const string &key) {
  auto it = home_cache.find(key);
  if (it != home_cache.end()) { return it->second; }
  if (home_cache.size() > 256) { home_cache.clear(); }
  DataStream ds;
  home hm;
  if (tp_db.Get("home_" + key, ds.dat)) { ds >> hm; }
  home_cache[key] = hm;
  return home_cache[key];
}
static void putHome(const string &key, home &hm) {
  DataStream ds;
  ds << hm;
  tp_db.Put("home_" + key, ds.dat);
}*/
static PlayerMap<home> ply_homes;
struct tpreq {
  int dir; // 0=f
  string name;
  clock_t reqtime;
};
static unordered_map<string, tpreq> tpmap;
static void oncmd_suic(argVec &a, CommandOrigin const &b, CommandOutput &outp) {
  auto sp = getSP(b.getEntity());
  if (sp) {
    ((Mob *)sp)->kill();
    outp.success("You are dead");
  }
}

static void sendTPChoose(ServerPlayer *sp, int type) { // 0=t
  //  string name = sp->getNameTag();
  gui_ChoosePlayer(sp, "Select target player", "Send teleport request",
                   [type](ServerPlayer *xx, string_view dest) {
                     SPBuf<512> sb;
                     sb.write("tpa "sv);
                     if (type == 0)
                       sb.write("t");
                     else
                       sb.write("f");
                     sb.write(" \""sv);
                     sb.write(dest);
                     sb.write("\""sv);
                     runcmdAs(sb.get(), xx);
                   });
}

static void sendTPForm(const string &from, int type, ServerPlayer *sp) {
  SPBuf<512> sb;
  sb.write("§b ");
  sb.write(from);
  sb.write((type ? " wants to teleport to your location"
                 : " wants to teleport you to his location"));
  sb.write(",you can enter \"/tpa ac\" to accept or \"/tpa de\" to reject");
  sendText(sp, sb);
  sb.clear();
  sb.write(from);
  sb.write((type ? " want to teleport to your location"
                 : " wants to teleport you to his location"));
  SharedForm *sf = getForm("TP Request", sb.get());
  sf->addButton("Accept");
  sf->addButton("Refuse");
  sf->cb = [](ServerPlayer *sp, string_view choice, int idx) {
    idx == 0 ? runcmdAs("tpa ac", sp) : runcmdAs("tpa de", sp);
  };
  sendForm(*sp, sf);
}
static SharedForm TPGUI("Send teleport request", "Send teleport request",
                        false);
static void SendTPGUI(ServerPlayer *sp) { sendForm(*sp, &TPGUI); }
static void initTPGUI() {
  TPGUI.addButton("Teleport to a player");
  TPGUI.addButton("Teleport a player to you");
  TPGUI.cb = [](ServerPlayer *sp, string_view sv, int idx) {
    sendTPChoose(sp, idx);
  };
}
static unordered_map<string, string> player_target;

void TPACommand::CANCEL(mandatory<Cancel> cmd) {
  string name = getOrigin().getName();
  if (player_target.count(name)) {
    auto &nm = player_target[name];
    if (tpmap.count(nm) && tpmap[nm].name == name) {
      tpmap.erase(nm);
      getOutput().success("§bCancelled");
    }
  } else {
    getOutput().error("No teleport request");
  }
}

void TPACommand::AC(mandatory<Ac> cmd) {
  auto sp = getSP(getOrigin().getEntity());
  string name = getOrigin().getName();
  if (tpmap.count(name) == 0)
    return;
  tpreq &req = tpmap[name];
  getOutput().success(
      "§bYou have accepted the send request from the other party");
  player_target.erase(req.name);
  auto dst = getplayer_byname(req.name);
  if (dst) {
    SPBuf sb;
    sb.write("§b ");
    sb.write(name);
    sb.write(" accepted the transmission request");
    sendText(dst, sb);
    if (req.dir == 0) {
      // f
      TeleportA(*sp, dst->getPos(), {dst->getDimensionId()});
    } else {
      TeleportA(*dst, sp->getPos(), {sp->getDimensionId()});
    }
  }
  tpmap.erase(name);
}

void TPACommand::DE(mandatory<De> cmd) {
  string name = getOrigin().getName();
  if (tpmap.count(name) == 0)
    return;
  tpreq &req = tpmap[name];
  getOutput().success("§bYou have rejected the send request");
  player_target.erase(req.name);
  auto dst = getplayer_byname(req.name);
  if (dst) {
    SPBuf sb;
    sb.write("§b ");
    sb.write(name);
    sb.write(" rejected the transmission request");
    sendText(dst, sb);
  }
  tpmap.erase(name);
}

void TPACommand::GUI(mandatory<Gui> cmd) {
  SendTPGUI(getSP(getOrigin().getEntity()));
  getOutput().success();
}

void TPACommand::CMDT(mandatory<CmdT> cmd, mandatory<std::string> target) {
  if (!CanTP) {
    getOutput().error("Teleport not enabled on this server!");
    return;
  }
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;

  string name = getOrigin().getName();
  auto dst = getplayer_byname2(target);
  if (!dst) {
    getOutput().error("target not found!");
    return;
  }
  auto &dnm = dst->getNameTag();
  if (tpmap.count(dnm)) {
    getOutput().error("A request of your target is pending.");
    return;
  }
  if (player_target.count(dnm)) {
    getOutput().error("You have already initiated the request");
    return;
  }
  player_target[name] = dnm;
  tpmap[dnm] = {1, name, clock()};
  getOutput().success("§bYou sent a teleport request to the target player");
  sendTPForm(name, 1, (ServerPlayer *)dst);
  return;
}

void TPACommand::CMDF(mandatory<CmdF> cmd, mandatory<std::string> target) {
  if (!CanTP) {
    getOutput().error("Teleport not enabled on this server!");
    return;
  }
  auto sp = getSP(getOrigin().getEntity());
  if (!sp)
    return;

  string name = getOrigin().getName();
  auto dst = getplayer_byname2(target);
  if (!dst) {
    getOutput().error("target not found!");
    return;
  }
  auto &dnm = dst->getNameTag();
  if (tpmap.count(dnm)) {
    getOutput().error("A request of your target is pending.");
    return;
  }
  if (player_target.count(dnm)) {
    getOutput().error("You have already initiated the request");
    return;
  }
  player_target[name] = dnm;
  tpmap[dnm] = {0, name, clock()};
  getOutput().success("§bYou sent a teleport request to the target player");
  sendTPForm(name, 0, (ServerPlayer *)dst);
  return;
}

// Home start

void HOMECommand::HGO(mandatory<HomeGo> cmd, mandatory<std::string> name) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  home &myh = ply_homes[sp];
  for (auto i = myh.vals.begin(); i != myh.vals.end(); ++i) {
    if (i->name == name) {
      i->tele(*sp);
      getOutput().success("§bTeleported you to home");
    }
  }
}

void HOMECommand::HADD(mandatory<HomeAdd> cmd, mandatory<std::string> name) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  Vec3 pos = getOrigin().getWorldPosition();
  home &myh = ply_homes[sp];
  if ((decltype(MaxHomes))myh.vals.size() >= MaxHomes) {
    getOutput().error("Can't add more homes");
    return;
  }
  myh.vals.push_back(Vpos(pos.x, pos.y, pos.z,
                          getOrigin().getEntity()->getDimensionId(), name));
  myh.save(*sp);
  getOutput().success("§bSuccessfully added a home");
}

void HOMECommand::HDEL(mandatory<HomeDel> cmd, mandatory<std::string> name) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  home &myh = ply_homes[sp];
  for (auto i = myh.vals.begin(); i != myh.vals.end(); ++i) {
    if (i->name == name) {
      myh.vals.erase(i);
      getOutput().success("§bHome has been deleted");
      myh.save(*sp);
      return;
    }
  }
  getOutput().error("Home not found");
}

void HOMECommand::HLS(mandatory<HomeLs> cmd) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  home &myh = ply_homes[sp];
  getOutput().addMessage("§b==Home list==");
  for (auto i = myh.vals.begin(); i != myh.vals.end(); ++i)
    getOutput().addMessage(i->name);
  getOutput().success("§b==Home list==");
}

void HOMECommand::HGUI(mandatory<HomeGui> cmd) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  home &myh = ply_homes[sp];
  auto sf = getForm("Home", "Please choose a home");
  for (auto i = myh.vals.begin(); i != myh.vals.end(); ++i) {
    auto &hname = i->name;
    sf->addButton(hname);
  }
  sf->cb = [](ServerPlayer *sp, string_view sv, int idx) {
    SPBuf<512> sb;
    sb.write("home go \"");
    sb.write(sv);
    sb.write("\"");
    runcmdAs(sb.get(), sp);
  };
  sendForm(*sp, sf);
  getOutput().success();
}

void HOMECommand::HDELGUI(mandatory<HomeDelGui> cmd) {
  if (!CanHome) {
    getOutput().error("Home not enabled on this server!");
    return;
  }
  ServerPlayer *sp = getSP(getOrigin().getEntity());
  if (!sp) {
    getOutput().error("this is a command for players");
    return;
  }
  home &myh = ply_homes[sp];
  auto sf = getForm("Home", "Please choose a home to DELETE");
  for (auto i = myh.vals.begin(); i != myh.vals.end(); ++i) {
    auto &hname = i->name;
    sf->addButton(hname);
  }
  sf->cb = [](ServerPlayer *sp, string_view sv, int idx) {
    SPBuf<512> sb;
    sb.write("home del \"");
    sb.write(sv);
    sb.write("\"");
    runcmdAs(sb.get(), sp);
  };
  sendForm(*sp, sf);
  getOutput().success();
}

// Home end, Warp start

void WARPCommand::WGO(mandatory<WarpGo> cmd, mandatory<std::string> name) {
  auto it = warp.find(name);
  if (it != warp.end()) {
    it->second.tele(*getOrigin().getEntity());
    getOutput().success("§bTeleported you to warp");
    return;
  }
}

void WARPCommand::WADD(mandatory<WarpAdd> cmd, mandatory<std::string> name) {
  int pl = (int)getOrigin().getPermissionsLevel();
  Vec3 pos = getOrigin().getWorldPosition();
  if (pl < 1)
    return;
  add_warp(pos.x, pos.y, pos.z, getOrigin().getEntity()->getDimensionId(),
           string(name));
  getOutput().success("§bSuccessfully added a warp");
  return;
}

void WARPCommand::WDEL(mandatory<WarpDel> cmd, mandatory<std::string> name) {
  int pl = (int)getOrigin().getPermissionsLevel();
  if (pl < 1)
    return;
  del_warp(string(name));
  getOutput().success("§bSuccessfully deleted a Warp");
  return;
}

void WARPCommand::WLS(mandatory<WarpLs> cmd) {
  getOutput().addMessage("§b==Warp list==");
  for (auto const &i : warp_list) {
    getOutput().addMessage(i);
  }
  getOutput().success("§b==Warp list==");
  return;
}

void WARPCommand::WGUI(mandatory<WarpGUI> cmd) {
  auto sf = getForm("Warp", "Please choose a warp");
  for (auto const &i : warp_list) {
    auto &hname = i;
    sf->addButton(hname);
  }
  sf->cb = [](ServerPlayer *sp, string_view sv, int idx) {
    SPBuf<512> sb;
    sb.write("warp go \"");
    sb.write(sv);
    sb.write("\"");
    runcmdAs(sb.get(), sp);
  };
  sendForm(*(ServerPlayer *)getOrigin().getEntity(), sf);
  getOutput().success();
}

// Warp end

static unordered_map<string, pair<Vec3, int>> deathpoint;
static void oncmd_back(argVec &a, CommandOrigin const &b, CommandOutput &outp) {
  if (!CanBack) {
    outp.error("Back not enabled on this server");
    return;
  }
  ServerPlayer *sp = (ServerPlayer *)b.getEntity();
  if (!sp)
    return;
  auto it = deathpoint.find(sp->getNameTag());
  if (it == deathpoint.end()) {
    outp.error("Can't find deathpoint");
    return;
  }
  TeleportA(*sp, it->second.first, {it->second.second});
  deathpoint.erase(it);
  outp.success("§bBack to deathpoint");
}
static void handle_mobdie(Mob &mb, const ActorDamageSource &) {
  if (!CanBack)
    return;
  auto sp = getSP(mb);
  if (sp) {
    ServerPlayer *sp = (ServerPlayer *)&mb;
    sendText(sp, "§bYou can use /back to return last deathpoint");
    deathpoint[sp->getNameTag()] = {sp->getPos(), sp->getDimensionId()};
  }
}
static int TP_TIMEOUT = 30;
THook(void *, _ZN12ServerPlayer9tickWorldERK4Tick, ServerPlayer *sp,
      unsigned long const *tk) {
  auto res = original(sp, tk);
  if (*tk % 40 == 0) {
    auto &name = sp->getNameTag();
    auto it = tpmap.find(name);
    if (it != tpmap.end()) {
      if (it->second.reqtime + CLOCKS_PER_SEC * TP_TIMEOUT <= clock()) {
        player_target.erase(it->second.name);
        sendText(sp, "§bYou rejected the TP request(Timeout)");
        auto sp2 = getuser_byname(it->second.name);
        sendText(sp2, "§bTarget rejected the TP request(Timeout)");
        tpmap.erase(it);
      }
    }
  }
  return res;
}

static void load_cfg(const char *v__3_p) {
  std::ifstream ifs{v__3_p};
  Json::Value value;
  Json::Reader reader;
  if (!reader.parse(ifs, value)) {
    auto msg = reader.getFormattedErrorMessages();
    do_log("%s", msg.c_str());
    exit(1);
  }
  CanBack = value["can_back"].asBool(false);
  CanHome = value["can_home"].asBool(false);
  CanTP = value["can_tp"].asBool(false);
  MaxHomes = value["max_homes"].asInt(5);
  auto &timeout = value["TP_TIMEOUT"];
  if (timeout.isInt()) {
    TP_TIMEOUT = timeout.asInt(30);
  } else {
    do_log("NO TP_TIMEOUT FOUND!USE 30s AS DEFAULT!!!");
  }
}

void onLoad(ModContext *__mContext) {
  std::filesystem::path v__2_cfg = __mContext->datadir;
  v__2_cfg /= "tp.json";
  std::filesystem::path v__4_db = __mContext->datadir;
  v__4_db /= "tp";
  tp_db.load(v__4_db.c_str());
  load_warps_new();
  load_cfg(v__2_cfg.c_str());
  initTPGUI();
  register_cmd("suicide", oncmd_suic, "kill yourself");
  register_cmd("back", oncmd_back, "back to deathpoint");
  reg_mobdie(handle_mobdie);
  register_commands();
}
