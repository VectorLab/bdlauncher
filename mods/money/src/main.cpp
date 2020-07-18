#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <minecraft/core/access.h>
#include <minecraft/json.h>

#include <mlr/modcontext.h>
#include <mlr/modmain.h>
#include <base.h>
#include <cmdhelper.h>
#include <dbimpl.h>
#include <gui.h>

#include "money.command.h"
#include "money.h"

static std::filesystem::path pathconfig;
static std::filesystem::path pathdb;
static LDBImpl db; // 2MB Cache
static int INIT_MONEY;

void loadcfg() {
  std::ifstream ifs(pathconfig.c_str());
  Json::Value value;
  Json::Reader reader;
  if (!reader.parse(ifs, value)) {
    auto msg = reader.getFormattedErrorMessages();
    do_log("%s", msg.c_str());
    exit(1);
  }
  INIT_MONEY = value["init_money"].asInt(0);
};
int money_get(std::string_view pn) {
  // lazy init
  string val;
  auto succ = db.Get(pn, val);
  if (!succ)
    return INIT_MONEY;
  return access(val.data(), int, 0);
};
void money_set(std::string_view pn, int am) {
  string val((char *)&am, 4);
  db.Put(pn, val);
};
bool money_red(std::string_view pn, int am) {
  int mo = money_get(pn);
  if (mo >= am) {
    money_set(pn, mo - am);
    return true;
  } else {
    return false;
  }
};
void money_add(std::string_view pn, int am) { money_red(pn, -am); };

void MoneyCommand::query(mandatory<Query> cmd) {
  string dst = getOrigin().getName();
  char buf[1024];
  snprintf(buf, 1024, "§b%s's money: %d", dst.c_str(), money_get(dst));
  getOutput().addMessage(string(buf));
  getOutput().success();
  return;
};

void MoneyCommand::pay(mandatory<Pay> cmd, mandatory<std::string> target,
                       mandatory<int> count) {
  string pl;
  string pl2;
  int mon;
  pl = getOrigin().getName();
  pl2 = target;
  mon = count;
  if (mon <= 0 || mon > 50000) {
    getOutput().error("§bToo big or small money.Maximum is 50000.");
  } else {
    if (money_red(pl, mon)) {
      money_add(pl2, mon);
      char msg[1000];
      // sprintf(msg,"§bYou gave %s %d money",pl2.c_str(),mon);
      snprintf(msg, 1000, "§bYou gave %s %d money", pl2.c_str(), mon);
      getOutput().success(string(msg));
      auto dstp = getplayer_byname(pl2);
      if (dstp)
        sendText(dstp,
                 "§bYou get " + std::to_string(mon) + " money from " + pl);
    } else {
      getOutput().error("You don't have enough money");
    }
  }
};

void MoneyCommand::paygui(mandatory<Paygui> cmd) {
  string nm = getOrigin().getName();
  gui_ChoosePlayer(
      (ServerPlayer *)getOrigin().getEntity(), "Choose a player to pay", "Pay",
      [](ServerPlayer *p, string_view chosen) {
        SPBuf sb;
        sb.write("How much do you pay to ");
        sb.write(chosen);
        sb.write("?");
        SharedForm *sf = getForm("Pay", "", true);
        sf->addInput(sb.get());
        string chosen_c(chosen);
        sf->cb = [chosen_c](ServerPlayer *p, string_view mon, int idx) {
          SPBuf sb;
          sb.write("money pay \"");
          sb.write(chosen_c);
          sb.write("\" ");
          sb.write(mon);
          runcmdAs(sb.get(), p);
        };
        sendForm(*p, sf);
      });
  getOutput().success();
};

void MoneyOP::OPQUERY(mandatory<OpQuery> cmd, mandatory<std::string> target) {
  char msg[1024];
  snprintf(msg, 1024, "§b%s has %d money", target.c_str(), money_get(target));
  getOutput().addMessage(string(msg));
  getOutput().success("");
};

void MoneyOP::add(mandatory<Add> cmd, mandatory<std::string> target,
                  mandatory<int> count) {
  // int amo;
  SPBuf buf;
  money_add(target, count);
  buf.write("§bAdded ");
  buf.write(std::to_string(count));
  buf.write(" money for ");
  buf.write(target);
  getOutput().success(buf.getstr());
  auto dstp = getplayer_byname(target);
  if (dstp)
    sendText(dstp, buf.get());
  getOutput().success();
};

void MoneyOP::set(mandatory<Set> cmd, mandatory<std::string> target,
                  mandatory<int> count) {
  string dst;
  int amo;
  dst = target;
  amo = count;
  money_set(dst, amo);
  char buf[1024];
  snprintf(buf, 1024, "§bSucceed to set %s 's money to %d", dst.c_str(),
           money_get(dst));
  getOutput().success(string(buf));
};

void MoneyOP::reduce(mandatory<Reduce> cmd, mandatory<std::string> target,
                     mandatory<int> count) {
  string_view dst = target;
  int amo = count;
  if (money_red(dst, amo)) {
    getOutput().success("§bDeducted successfully");
    auto dstp = getplayer_byname(dst);
    if (dstp)
      sendText(dstp, "§bYou used " + std::to_string(amo) + " money");
  } else {
    getOutput().error("§bTarget player does not have enough money");
  }
};

void MoneyOP::reloadcfg(mandatory<Reloadcfg> cmd) {
  loadcfg();
  getOutput().success("Reloaded");
};

void onLoad(ModContext *_mContext) {
  std::filesystem::path datadir = _mContext->datadir;
  pathconfig = datadir;
  pathconfig /= "money.json";
  pathdb = datadir;
  pathdb /= "money";
  db.load(pathdb.c_str(), true, 1048576 * 2);
  register_commands();
  loadcfg();
};
