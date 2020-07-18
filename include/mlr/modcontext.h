#pragma once

#include <cstdint>
#include <deque>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <functional>

enum class LoadStatus : uint8_t {
  pending = 1,
  loading = 2,
  done = 3,
};

enum class LoadMode : uint8_t {
  m_onLoad = 0,
  m_dlopen = 1,
  m_compile = 2,
  m_nothing = 3,
};

class ModContext {
public:
  /*
  src
  data
  meta.json (name dependency loadmode solist)
  disabled (status)
  mod.so
  */
  std::function<void(ModContext*)> onChildLoad=nullptr;

  std::filesystem::path basedir;
  std::filesystem::path datadir;
  std::filesystem::path resdir;

  LoadStatus load_status = LoadStatus::pending;
  LoadMode load_mode = LoadMode::m_onLoad;
  bool debug = false;
  /* @set(__set(val){this=true;}) */
  bool changed = false;

  std::filesystem::path p___mod_dso;
  std::filesystem::path p___src;
  std::filesystem::path p___data;
  std::filesystem::path p___lib;
  std::filesystem::path p___meta_djson;
  std::deque<std::string> m_libsharedobj;

  /* value==this?nullptr:value */
  std::unordered_map<std::string, ModContext *> dep_list;

  std::string name;

  /* @memory=external */
  std::unordered_map<std::string, ModContext *> *mod_list = nullptr;

  bool resolveDependency();
  void load();
};
