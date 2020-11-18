#pragma once
#include "cmdreg.h"
#include "utils.h"
#include <mlr/pubexport.h>
#include "cmdhelper.h"
#include "stkbuf.hpp"
#include "stl.hpp"
#include <functional>
#include <minecraft/actor/Player.h>
#include <minecraft/core/types.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std::literals;
using std::function;
using std::string;
using std::string_view;
using std::vector;

class Actor;
class Vec3;
// class AutomaticID<Dimension,int>;
class Dimension;
struct MCRESULT;
class GameMode;
class ItemActor;
class ActorDamageSource;
class Mob;

enum TextType : char { RAW = 0, POPUP = 3, JUKEBOX_POPUP = 4, TIP = 5 };

PUB_EXPORT void sendText(Player *a, string_view ct, TextType type = RAW);
PUB_EXPORT void broadcastText(string_view ct, TextType type = RAW);
#define sendText2(a, b) sendText(a, b, JUKEBOX_POPUP)
PUB_EXPORT void TeleportA(Actor &a, Vec3 b, AutomaticID<Dimension, int> c);
PUB_EXPORT ServerPlayer *getplayer_byname2(string_view name);
PUB_EXPORT void get_player_names(vector<string> &a);
PUB_EXPORT MCRESULT runcmd(string_view a,int dim=0);
PUB_EXPORT MCRESULT runcmdAs(string_view a, Player *sp);

PUB_EXPORT void split_string(string_view s, std::vector<std::string_view> &v,
                             string_view c);
PUB_EXPORT void split_string(string_view s, static_deque<std::string_view> &v,
                             string_view c);
PUB_EXPORT bool execute_cmdchain(string_view chain_, string_view sp,
                                 bool chained = true);
PUB_EXPORT void
reg_useitemon(bool (*)(GameMode *a0, ItemStack *a1, BlockPos const *a2,
                       BlockPos const *dstPos, Block const *a5));
PUB_EXPORT void reg_destroy(bool (*)(GameMode *a0, BlockPos const *));
PUB_EXPORT void reg_attack(bool (*)(ServerPlayer *a0, Actor *a1));
PUB_EXPORT void reg_chat(bool (*)(ServerPlayer *a0, string &payload));
PUB_EXPORT void reg_player_join(void (*)(ServerPlayer *));
PUB_EXPORT void reg_player_left(void (*)(ServerPlayer *));
PUB_EXPORT void reg_pickup(bool (*)(Actor *a0, ItemActor *a1));
PUB_EXPORT void reg_popItem(bool (*)(ServerPlayer &, BlockPos &));
PUB_EXPORT void reg_mobhurt(bool (*)(Mob &, ActorDamageSource const &, int &));
PUB_EXPORT void reg_mobdie(void (*)(Mob &, ActorDamageSource const &));
PUB_EXPORT void reg_actorhurt(bool (*)(Actor &, ActorDamageSource const &,
                                       int &));
PUB_EXPORT void reg_interact(bool (*)(GameMode *, Actor &));

PUB_EXPORT int getPlayerCount();
//PUB_EXPORT int getMobCount();

PUB_EXPORT NetworkIdentifier *getPlayerNeti(ServerPlayer &sp);
PUB_EXPORT ServerPlayer *getuser_byname(string_view a);
PUB_EXPORT void base_sendPkt(ServerPlayer *sp, Packet &pk);

#define getplayer_byname(x) (getuser_byname(x))

PUB_EXPORT void forceKickPlayer(ServerPlayer &sp);

// template

#define ARGSZ(b)                                                               \
  if (a.size() < b) {                                                          \
    char buf[256];                                                             \
    sprintf(buf, "check your arg.%d required ,%d found", b, a.size());         \
    outp.error(buf);                                                           \
    return;                                                                    \
  }

#ifndef BASE
extern "C" {
void MC(); // dummy
void ServLevel();
}
#endif
#define getMC() (*reinterpret_cast<Minecraft **>(MC))
#define getSrvLevel() (*reinterpret_cast<Level **>(ServLevel))
static inline bool isOp(ServerPlayer const *sp) {
  return (int)sp->getPlayerPermissionLevel() > 1;
}
static inline bool isOp(CommandOrigin const &sp) {
  return (int)sp.getPermissionsLevel() > 0;
}

using std::pair;
#ifdef BDL_TAG
#error "DEPRECATED BDL_TAG"
#endif
PUB_EXPORT void* base_getptr_Minecraft();
PUB_EXPORT void* base_getptr_Level();
PUB_EXPORT void* base_getptr_DedicatedServer();

