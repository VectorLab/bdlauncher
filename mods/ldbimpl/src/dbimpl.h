#pragma once
#include <mlr/pubexport.h>
#include <functional>
#include <leveldb/db.h>
#include <string>
#include <string_view>

class LDBImpl {
public:
  leveldb::DB *db;
  leveldb::ReadOptions rdopt;
  leveldb::WriteOptions wropt;
  leveldb::Options options;
  PUB_EXPORT void load(const char *name, bool read_cache = true,
                       int lru_cache_sz = 0);
  PUB_EXPORT LDBImpl(const char *name, bool read_cache = true,
                     int lru_cache_sz = 0);
  PUB_EXPORT LDBImpl(); // wait load();
  PUB_EXPORT void close();
  PUB_EXPORT ~LDBImpl();
  PUB_EXPORT bool Get(std::string_view key, std::string &val) const;
  PUB_EXPORT void Put(std::string_view key, std::string_view val);
  PUB_EXPORT bool Del(std::string_view key);
  PUB_EXPORT void
  Iter(std::function<void(std::string_view, std::string_view)> fn) const;
  PUB_EXPORT void CompactAll();
};
