#include <cstdlib>
#include <dlfcn.h>
#include <fstream>
#include <iostream>

#include <mlr/compilesotask.h>
#include <mlr/logger.h>
#include <mlr/modcontext.h>
#include <mlr/modloaderutil.h>
#include <mlr/modmain.h>

bool ModContext::resolveDependency() {
  bool ret = true;
  for (auto &[t_name, t_meta] : this->dep_list) {
    if (!this->mod_list->count(t_name) > 0) {
      ret = false;
      do_log("%s require %s not found", this->name.c_str(), t_name.c_str());
      continue;
    }
    this->dep_list[t_name] = this->mod_list->operator[](t_name);
  }
  return ret;
};

void ModContext::load() {
  if (this->load_status == LoadStatus::loading) {
    do_log("error: %s is already loading", this->name.c_str());
    exit(1);
    return;
  }
  if (this->load_status == LoadStatus::done) {
    return;
  }
  this->load_status = LoadStatus::loading;

  bool need_compile = false;
  for (auto &[t_name, t_meta] : this->dep_list) {
    switch (t_meta->load_status) {
    case LoadStatus::pending: {
      t_meta->load();
      break;
    }
    case LoadStatus::loading: {
      do_log("%s require %s but it is loading", this->name.c_str(),
             t_name.c_str());
      exit(1);
      break;
    }
    case LoadStatus::done: {
      break;
    }
    }
    need_compile |= t_meta->changed;
  }
  do_log("loading: %s", this->name.c_str());
  if (this->debug) {
    need_compile = true;
  }
  if (!need_compile) {
    if (std::filesystem::is_regular_file(this->p___mod_dso)) {
      bool src_changed = false;
      int64_t last_compile_time = stdfs_filetime_to_ms(
          std::filesystem::last_write_time(this->p___mod_dso));
      int64_t p___meta_djson___time = stdfs_filetime_to_ms(
          std::filesystem::last_write_time(this->p___meta_djson));
      if (p___meta_djson___time >= last_compile_time) {
        need_compile = true;
      } else {
        stdfs_dir_foreach_recursion_b(
            this->p___src, [&](std::filesystem::path &p_me_) {
              if (!std::filesystem::is_regular_file(p_me_)) {
                return false;
              }
              std::string t_val_t_typename(p_me_.extension().c_str());
              mlr_filename_type t_val_t_type =
                  mlr_filename_type_get(t_val_t_typename);
              if (mlr_filename_type::t_cpp == t_val_t_type ||
                  mlr_filename_type::t_h == t_val_t_type ||
                  mlr_filename_type::t_c == t_val_t_type ||
                  mlr_filename_type::t_hpp == t_val_t_type ||
                  mlr_filename_type::t_ipp == t_val_t_type) {
              } else {
                return false;
              }
              int64_t t_val_t_time =
                  stdfs_filetime_to_ms(std::filesystem::last_write_time(p_me_));
              if (t_val_t_time >= last_compile_time) {
                need_compile = true;
                return true;
              }
              return false;
            });
      }
    } else {
      need_compile = true;
    }
  }
  this->changed = need_compile;
  for (auto &[t_name, t_meta] : this->dep_list) {
    if(nullptr!=t_meta->onChildLoad){
      t_meta->onChildLoad(this);
    }
  }
  need_compile=this->changed;
  if (need_compile) {
    if (LoadMode::m_nothing != this->load_mode) {
      // compile mod.so
      CompileSoTask compiletask;
      if (this->debug) {
        compiletask.cflags = " -fPIC -g -Og -c -fvisibility=hidden -std=gnu17 ";
        compiletask.cxxflags =
            " -fPIC -g -Og -c -fvisibility=hidden -std=gnu++17 ";
      }
      compiletask.p_out = this->p___mod_dso;
      compiletask.p_src = this->p___src;

      for (auto &[t_name, t_meta] : this->dep_list) {
        if(std::filesystem::is_directory(t_meta->p___include)){
          compiletask.p_include.push_front(t_meta->p___include);
        }
      }
      if(std::filesystem::is_directory(this->p___include)){
        compiletask.p_include.push_front(this->p___include);
      }
      if (std::filesystem::is_directory(this->p___lib)) {
        // impl static lib
        stdfs_dir_foreach_recursion_b(
            this->p___lib, [&](std::filesystem::path &p_tlib_) {
              if (!std::filesystem::is_regular_file(p_tlib_)) {
                return false;
              }
              std::string t_val_t_typename(p_tlib_.extension().c_str());
              if (".a" != t_val_t_typename) {
                return false;
              }
              compiletask.p_libstatic.push_front(p_tlib_);
              return false;
            });
        // impl shared lib dir
        if (!this->m_libsharedobj.empty()) {
          compiletask.p_libsharedpath.push_front(this->p___lib);
        }
      }
      compiletask.m_libsharedobj = this->m_libsharedobj;
      compiletask.run();
      if (TaskStatus::fail == compiletask.status) {
        do_log("failed to build %s", this->name.c_str());
        exit(1);
      }

    } else {
      std::filesystem::remove(this->p___mod_dso);
      std::fstream ce_p___mod_dso;
      ce_p___mod_dso.open(this->p___mod_dso.c_str(), std::ios::out);
      ce_p___mod_dso.close();
    }
  }
  if (std::filesystem::is_directory(this->p___data) &&
      !std::filesystem::is_directory(this->datadir)) {
    std::filesystem::copy(this->p___data, this->datadir,
                          std::filesystem::copy_options::recursive |
                              std::filesystem::copy_options::copy_symlinks);
  }
  void *mod_this_handle = nullptr;
  if (LoadMode::m_dlopen == this->load_mode ||
      LoadMode::m_onLoad == this->load_mode) {
    mod_this_handle = dlopen(this->p___mod_dso.c_str(),
                             RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);
  }
  if (LoadMode::m_onLoad == this->load_mode) {
    if (nullptr == mod_this_handle) {
      do_log("err loading %s handle %p %s\n", this->name.c_str(),
             mod_this_handle, dlerror());
      exit(1);
    }
    void *mod_this_onLoad = dlsym(mod_this_handle, "onLoad");
    if (nullptr == mod_this_onLoad) {
      do_log("err loading %s onLoad function %p %s\n", this->name.c_str(),
             mod_this_onLoad, dlerror());
      exit(1);
    }
    ((typeof(&onLoad))mod_this_onLoad)(this);
  }
  this->load_status = LoadStatus::done;
};
