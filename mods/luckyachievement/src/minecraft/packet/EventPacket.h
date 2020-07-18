#pragma once

// sizeof(Packet) = 0x28
#include <minecraft/packet/Packet.h>
static_assert(0x28==sizeof(Packet));

// sizeof(ActorUniqueID) = 8
#include <minecraft/core/types.h>
static_assert(8==sizeof(ActorUniqueID));

// sizeof(MinecraftEventing::AchievementIds) = 4
#include "../core/MinecraftEventing.h"
static_assert(4==sizeof(MinecraftEventing::AchievementIds));

class Player;

class EventPacket:public Packet{
// sizeof(EventPacket) = 0xf8
public:

class Data{
// sizeof(EventPacket::Data) = 0xc8
public:
	virtual ~Data();
	int mType;
	bool mUsePlayerID;
	char unknown[3];
	// unknown union class $A9B56BB2A8A14AFA32257D71180A3DDB
	char _anon_0[20];
	std::string mEntityName,mCommandName,mResultKey,mResultString,mErrorList;
	
	Data();
};

	ActorUniqueID mPlayerId;
	Data mEventData;
	
	EventPacket(Player const*, MinecraftEventing::AchievementIds, bool);
	EventPacket(ActorUniqueID, MinecraftEventing::AchievementIds, bool);
	~EventPacket();
	
	int getId() const;
	std::string getName() const;
	void write(BinaryStream &) const;
	void read(ReadOnlyBinaryStream &);
};

static_assert(0xc8==sizeof(EventPacket::Data));
static_assert(0xf8==sizeof(EventPacket));
