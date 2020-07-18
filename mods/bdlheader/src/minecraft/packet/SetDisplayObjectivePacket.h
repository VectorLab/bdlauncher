#pragma once

#include "../global.h"
#include "Packet.h"
#include <string>
#include <dlfcn.h>
#include <cstdint>
using std::string;
struct ObjectiveSortOrder {
  unsigned char x;
  ObjectiveSortOrder(unsigned char y) { x = y; }
};
class SetDisplayObjectivePacket {
  char filler[200];

public:
  SetDisplayObjectivePacket(
      std::string const &displaySlot,
      std::string const &objectiveName,
      std::string const &displayName,
      std::string const &criteriaName,
      char sortOrder) {
    void (*pp)(char *) = (typeof(pp)) dlsym(NULL, "_ZN6PacketC2Ev");
    uint64_t vt             = (uint64_t) dlsym(NULL, "_ZTV25SetDisplayObjectivePacket");
    pp(filler);
    *((uint64_t *) filler) = vt + 16;
    new (filler + 32) std::string(displaySlot);
    new (filler + 64) std::string(objectiveName);
    new (filler + 96) std::string(displayName);
    new (filler + 128) std::string(criteriaName);
    filler[160] = sortOrder;
  }
  //{asm("call
  //_ZN25SetDisplayObjectivePacketC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_S7_S7_18ObjectiveSortOrder;");}

  ~SetDisplayObjectivePacket() {
    //((string*)(filler+32))->~string();
    //((string*)(filler+64))->~string();
    //((string*)(filler+96))->~string();
    //((string*)(filler+128))->~string();
  } //{asm("call _ZN25SetDisplayObjectivePacketD2Ev;");}
};
class TransferPacket {
  char filler[100];

public:
  inline TransferPacket(std::string ip, short port) {
    void (*pp)(char *) = (typeof(pp)) dlsym(NULL, "_ZN6PacketC2Ev");
    uint64_t vt             = (uint64_t) dlsym(NULL, "_ZTV14TransferPacket");
    printf("get %p\n", vt);
    pp(filler);
    *((uint64_t *) filler) = vt + 16;
    new (filler + 32) std::string(ip);
    *((short *) (filler + 16)) = port;
  }

  inline ~TransferPacket() { ((std::string *) (filler + 32))->~string(); }
};
