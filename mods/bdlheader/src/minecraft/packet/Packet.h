#pragma once

#include <string>

class BinaryStream;
class ReadOnlyBinaryStream;

class Packet {
  char filler[32];

public:
  Packet();
  virtual ~Packet();
  
  virtual int getId() const                 = 0;
  virtual std::string getName() const       = 0;
  virtual void write(BinaryStream &) const  = 0;
  virtual void read(ReadOnlyBinaryStream &) = 0;
  virtual int readExtended(ReadOnlyBinaryStream &stream);
  virtual bool disallowBatching(void) const;
  unsigned char getClientSubId() const;
};
