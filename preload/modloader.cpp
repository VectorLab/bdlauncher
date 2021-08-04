#include <mlr/logger.h>
#include <mlr/modcontext.h>
#include <mlr/modloader.h>
#include <mlr/modloaderutil.h>
#include <mlr/modmain.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

void launcher_modloaader_main() {
  path_mlr_def::setPath(std::filesystem::current_path());
  std::filesystem::path path_mods = path_mlr_def::p___mods;
  if (!std::filesystem::is_directory(path_mods)) {
    std::filesystem::create_directories(path_mods);
    do_log("no mods dir found,create only");
    return;
  }
  std::filesystem::path path_moddata = path_mlr_def::p___moddata;
  if (!std::filesystem::is_directory(path_moddata)) {
    std::filesystem::create_directories(path_moddata);
  }
  std::filesystem::path path_mlr = path_mlr_def::p___mlr;
  if (!std::filesystem::is_directory(path_mlr)) {
    do_log("warning: data path not found");
    exit(1);
    return;
  }
  std::filesystem::directory_iterator path_mods_foreach(path_mods);
  std::unordered_map<std::string, ModContext *> mod_list;
  for (auto &path_mods_forentry : path_mods_foreach) {
    std::filesystem::path path_mod = path_mods_forentry.path();
    /* @action malloc(meta) */
    ModContext *meta = tryLoadMeta(path_mod);
    if (nullptr == meta) {
      continue;
    }
    if (mod_list.count(meta->name) > 0) {
      do_log("too many mods named: %s", meta->name.c_str());
      exit(1);
    }
    meta->mod_list = &mod_list;
    mod_list.insert({meta->name, meta});
  }
  bool depok = true;
  for (auto &[name, meta] : mod_list) {
    depok &= meta->resolveDependency();
  }
  if (!depok) {
    do_log("dependency error");
    exit(1);
  }
  do_log("scan ok , start load");
  for (auto &[name, meta] : mod_list) {
    meta->load();
  }
  do_log("load ok , start post load");
  for (auto &[name, meta] : mod_list) {
    if(nullptr!=meta->onPostLoad){
      meta->onPostLoad();
    }
  }
  do_log("everything is ok");
  for (auto &[name, meta] : mod_list) {
    delete meta;
  }
  mod_list.clear();
};
