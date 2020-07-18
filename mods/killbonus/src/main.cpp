#include <fstream>
#include <string>

#include <minecraft/core/getSP.h>
#include <minecraft/core/types.h>
#include <minecraft/level/Level.h>
#include <minecraft/actor/Player.h>
#include <minecraft/actor/ActorDamageSource.h>
#include <mlr/statichook.h>
#include <cmdhelper.h>
#include <minecraft/json.h>
#include <mlr/modcontext.h>
#include <mlr/modmain.h>

#include <base.h>
#include <money.h>
#include "main.command.h"

unordered_map<int,int> bonus_mp;
std::filesystem::path configpath;

void load(CommandOutput *out) {
  std::ifstream ifs{configpath.c_str()};
  Json::Value value;
  Json::Reader reader;
  if (!reader.parse(ifs, value)) {
    std::string msg = reader.getFormattedErrorMessages();
    if(nullptr==out){
      out->error(msg);
    }else{
    do_log("%s",msg.c_str());
    }
  }
  for (auto &i : value) {
    int b     = i["money"].asInt(0);
    int eid      = i["eid"].asInt(0);
    bonus_mp[eid] = b;
  }
  if(nullptr!=out){
out->success("Reloaded!");
}else{
do_log("Reloaded!");
}
};

static int dbg_die;
static void toggle_dbg(CommandOutput *out) {
  dbg_die = !dbg_die;
  if (out){
std::string omsg="Debug mode ";
if(dbg_die){
omsg+="on";
}else{
omsg+="off";
}
 out->success(omsg);
}
};

static void handle_die(Mob &a, ActorDamageSource const &b) {
  if (b.isChildEntitySource() || b.isEntitySource()) {
auto ent = getSrvLevel()->fetchEntity(b.getEntityUniqueID(), false);
    ServerPlayer *sp = getSP(ent);
    if (nullptr!=sp) {
      int vid = a.getEntityTypeId();
if (dbg_die) {
sendText(sp, "You killed " + std::to_string(vid));
}
      if (0<bonus_mp.count(vid)) {
        int am = bonus_mp.at(vid);
        money_add(sp->getName(), am);
        sendText2(sp, "§cYou get $" + std::to_string(am) + " by killing");
      }
    }
  }
};

void CommandKillbonus::invoke(mandatory<KillbonusMode> mode) {
  auto out = &getOutput();
  switch (mode) {
  case KillbonusMode::Reload: load(out); break;
  case KillbonusMode::Debug: toggle_dbg(out); break;
  default: getOutput().error("Unknown mode"); return;
  }
};

void onLoad(ModContext* _mContext) {
configpath=_mContext->datadir;
configpath/="killbonus.json";
  load(nullptr);
  register_commands();
  reg_mobdie(handle_die);
};
