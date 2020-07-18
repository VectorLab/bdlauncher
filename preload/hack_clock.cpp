#include "hack_clock.h"
#include <mlr/modloaderutil.h>

/*
fix g++-9 gnu++17
 */

int64_t stdfs_filetime_to_ms(std::filesystem::file_time_type __a) {
  return hack_clock::to_sys<std::chrono::milliseconds>(__a)
      .time_since_epoch()
      .count();
};

