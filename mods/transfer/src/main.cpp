#include <string>

#include <mlr/modmain.h>
#include <mlr/statichook.h>
#include <minecraft/actor/Player.h>
#include <minecraft/core/getSP.h>
#include <minecraft/packet/BinaryStream.h>
#include <minecraft/packet/MyPkt.h>

#include "main.command.h"
#include <base.h>

void TransferCommand::invoke(mandatory<std::string> server,
                             mandatory<int> port) {
  MyPkt trpk(0x55, [&](void *, BinaryStream &x) -> void {
    x.writeUnsignedVarInt(server.size());
    x.write(server.data(), server.size());
    x.writeUnsignedShort(port);
  });
  auto sp = getSP(getOrigin().getEntity());
  if (sp) {
    sp->sendNetworkPacket(trpk);
    getOutput().success();
  } else {
    getOutput().error("fucku");
  }
}

void onLoad(ModContext *) { register_commands(); }
