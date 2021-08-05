#include <mlr/logger.h>
#include <mlr/modcontext.h>
#include <mlr/modloaderutil.h>
#include <fstream>

#include <json/json.h>

std::unordered_map<std::string, mlr_filename_type> mlr_filename_type_values({
    {"", mlr_filename_type::tnull},
    {".", mlr_filename_type::tdot},
    {".c", mlr_filename_type::t_c},
    {".cpp", mlr_filename_type::t_cpp},
    {".h", mlr_filename_type::t_h},
    {".hpp", mlr_filename_type::t_hpp},
    {".ipp", mlr_filename_type::t_ipp},
});

std::filesystem::path path_mlr_def::p___mlr;
std::filesystem::path path_mlr_def::p___mlr_sinclude;
std::filesystem::path path_mlr_def::p___mlr_sconfig_scc;
std::filesystem::path path_mlr_def::p___mlr_sconfig_scxx;
std::filesystem::path path_mlr_def::p___mods;
std::filesystem::path path_mlr_def::p___moddata;

void path_mlr_def::setPath(std::filesystem::path rp){
rp=std::filesystem::absolute(rp);
path_mlr_def::p___mlr=rp;
path_mlr_def::p___mlr/="mlr";
path_mlr_def::p___mlr_sinclude=rp;
path_mlr_def::p___mlr_sinclude/="mlr/include";
path_mlr_def::p___mlr_sconfig_scc=rp;
path_mlr_def::p___mlr_sconfig_scc/="mlr/config/cc";
path_mlr_def::p___mlr_sconfig_scxx=rp;
path_mlr_def::p___mlr_sconfig_scxx/="mlr/config/cxx";
path_mlr_def::p___mods=rp;
path_mlr_def::p___mods/="mods";
path_mlr_def::p___moddata=rp;
path_mlr_def::p___moddata/="moddata";
};

bool stdfs_dir_foreach_recursion_b(
    std::filesystem::path &t_dir,
    std::function<bool(std::filesystem::path &)> callback) {
  std::filesystem::directory_iterator path_foreach(t_dir);
  for (auto &path_forentry : path_foreach) {
    std::filesystem::path path_child = path_forentry.path();
    if (callback(path_child)) {
      return true;
    }
    if (std::filesystem::is_directory(path_child)) {
      if (stdfs_dir_foreach_recursion_b(path_child, callback)) {
        return true;
      }
    }
  }
  return false;
};

bool stdfs_dir_foreach_recursion_a(
    std::filesystem::path &t_dir,
    std::function<bool(std::filesystem::path &)> callback) {
  std::filesystem::directory_iterator path_foreach(t_dir);
  for (auto &path_forentry : path_foreach) {
    std::filesystem::path path_child = path_forentry.path();
    if (std::filesystem::is_directory(path_child)) {
      if (stdfs_dir_foreach_recursion_a(path_child, callback)) {
        return true;
      }
    }
    if (callback(path_child)) {
      return true;
    }
  }
  return false;
};

ModContext* tryLoadMeta(std::filesystem::path &t_dir) {
  if (!std::filesystem::is_directory(t_dir)) {
    return nullptr;
  }
  std::filesystem::path test_is_disabled = t_dir;
  test_is_disabled /= "disabled";
  if (std::filesystem::is_regular_file(test_is_disabled)) {
    return nullptr;
  }
  ModContext *ret = new ModContext();
  ret->p___meta_djson = t_dir;
  ret->p___meta_djson /= "meta.json";
  if (!std::filesystem::is_regular_file(ret->p___meta_djson)) {
    delete ret;
    return nullptr;
  }

  ret->basedir = t_dir;
  ret->p___src = t_dir;
  ret->p___src /= "src";
  ret->p___include = t_dir;
  ret->p___include /= "include";
  ret->p___data = t_dir;
  ret->p___data /= "data";
  ret->p___lib = t_dir;
  ret->p___lib /= "lib";
  ret->p___mod_dso = t_dir;
  ret->p___mod_dso /= "mod.so";

  std::ifstream ifs;
  ifs.open(ret->p___meta_djson.c_str());
  Json::Value value;
  Json::Reader reader;
  if (!reader.parse(ifs, value)) {
    do_log("config invalid");
    auto msg = reader.getFormattedErrorMessages();
    do_log("%s", msg.c_str());
    exit(1);
  }
  ret->name = value["name"].asString();
  if (ret->name.empty()) {
    do_log("name not found: %s", t_dir.c_str());
    exit(1);
  }
  uint32_t load_mode_temp = static_cast<uint32_t>(ret->load_mode);
  load_mode_temp = value["loadmode"].asUInt();
  if (load_mode_temp > 3) {
    do_log("%s not has a valid loadmode", t_dir.c_str());
    exit(1);
  }
  ret->load_mode = static_cast<LoadMode>(load_mode_temp);
  uint32_t i, c;
  if (value["dependency"].isArray()) {
    i = 0, c = value["dependency"].size();
    while (i < c) {
      std::string t_name(value["dependency"][i].asString());
      ret->dep_list.insert({t_name, nullptr});
      ++i;
    }
  }
  if (value["solist"].isArray()) {
    i = 0, c = value["solist"].size();
    while (i < c) {
      std::string t_name(value["solist"][i].asString());
      ret->m_libsharedobj.push_back(t_name);
      ++i;
    }
  }
  if (value["debug"].isBool()) {
    ret->debug = value["debug"].asBool();
  }

  ret->datadir = path_mlr_def::p___moddata;
  ret->datadir /= ret->name.c_str();
  ret->resdir = t_dir;
  ret->resdir /= "res";
  return ret;
};

