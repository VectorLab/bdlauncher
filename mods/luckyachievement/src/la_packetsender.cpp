#include "la_struct.h"
#include "la_execute.h"
#include <mlr/logger.h>
#include <minecraft/actor/Player.h>

void la_sendpacket_execute(ServerPlayer* sp,int i){
EventPacket* pk=new EventPacket(sp,static_cast<MinecraftEventing::AchievementIds>(i),true);
sp->sendNetworkPacket(*pk);
delete pk;
};

void la_test(){
do_log("Packet: %ld",sizeof(Packet));
do_log("ActorUniqueID: %ld",sizeof(ActorUniqueID));
do_log("MinecraftEventing::AchievementIds: %ld",sizeof(MinecraftEventing::AchievementIds));
do_log("EventPacket::Data: %ld",sizeof(EventPacket::Data));
do_log("EventPacket: %ld",sizeof(EventPacket));
};
