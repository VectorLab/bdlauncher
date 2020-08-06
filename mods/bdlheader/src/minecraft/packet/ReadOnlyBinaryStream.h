#pragma once
#include <string>
#include <cstdint>

class ReadOnlyBinaryStream {
public:
size_t read_pointer{};
bool unk_1;
std::string ownbuf,*pbuf;
ReadOnlyBinaryStream(std::string&& buffer);
ReadOnlyBinaryStream(std::string const& buffer,bool owned);
virtual ~ReadOnlyBinaryStream();
virtual bool read(void*,unsigned long);
  
  template<typename T>
  T getType();
};
