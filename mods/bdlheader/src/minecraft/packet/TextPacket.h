#pragma once

#include <string>
#include <vector>
#include "../global.h"
#include "Packet.h"

enum class TextPacketType : char {
  Raw           = 0,
  Chat          = 1,
  JukeboxPopup  = 4,
  SystemMessage = 6,
  Whisper       = 7,
  Announcement  = 8,
  TextObject    = 9,
};

class TextPacket : public Packet {
//  char filler[200 - sizeof(Packet)];
alignas(8) TextPacketType type;
  std::string source, content;
  std::vector<std::string> args;
  bool translated;
  std::string xuid;
  std::string unknown;// TODO uuid

public:
  TextPacket();

//  MakeAccessor(type, char, 32);
//  MakeAccessor(sender, std::string, 40);

  virtual ~TextPacket();
  virtual int getId() const override;
  virtual std::string getName() const override;
  virtual void write(BinaryStream &) const override;
  virtual void read(ReadOnlyBinaryStream &) override;
  virtual bool disallowBatching(void) const override;

  static TextPacket createTranslatedAnnouncement(
      std::string const &sender, std::string const &content, std::string const &uuid, std::string const &xuid);
  static TextPacket createRaw(std::string const &);
  static TextPacket createTranslatedChat(std::string const &txt);
  static TextPacket createJukeboxPopup(std::string const &txt);
  static TextPacket createSystemMessage(std::string const&);
};
