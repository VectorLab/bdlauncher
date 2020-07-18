#pragma once

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>

class ModContext;

/* @return(memory=new) */
ModContext* tryLoadMeta(std::filesystem::path &t_dir);

struct CompareCStrings {
  bool operator()(const char *lhs, const char *rhs) const {
    return std::strcmp(lhs, rhs) < 0;
  }
};

enum class mlr_filename_type : uint8_t {
  tnull = 0,   // ""
  tdot = 1,    // "."
  t_other = 2, // no except
  t_c = 3,
  t_cpp = 4,
  t_h = 5,
  t_hpp = 6,
  t_ipp = 7,
};

extern std::unordered_map<std::string, mlr_filename_type>
    mlr_filename_type_values;

inline mlr_filename_type mlr_filename_type_get(std::string &a) {
  if (mlr_filename_type_values.count(a) > 0) {
    return mlr_filename_type_values[a];
  }
  return mlr_filename_type::t_other;
};

class path_mlr_def {
public:
  static std::filesystem::path p___mlr;
  static std::filesystem::path p___mlr_sinclude;
  static std::filesystem::path p___mlr_sconfig_scc;
  static std::filesystem::path p___mlr_sconfig_scxx;
  static std::filesystem::path p___mods;
  static std::filesystem::path p___moddata;

  static void setPath(std::filesystem::path);
};

/* @return(type=millionsecond) @src(visibility=hidden) */
int64_t stdfs_filetime_to_ms(std::filesystem::file_time_type);

/* @arg2(return(value=isdone))*/
bool stdfs_dir_foreach_recursion_a(
    std::filesystem::path &, std::function<bool(std::filesystem::path &)>);
bool stdfs_dir_foreach_recursion_b(
    std::filesystem::path &, std::function<bool(std::filesystem::path &)>);
