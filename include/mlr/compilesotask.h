#pragma once

#include <deque>
#include <filesystem>
#include <string>

#include "taskstatus.h"

class CompileSoTask {
public:
  std::string cflags = " -fPIC -s -O3 -c -fvisibility=hidden -std=gnu17 ";
  std::string cxxflags = " -fPIC -s -O3 -c -fvisibility=hidden -std=gnu++17 ";
  std::string ldflags =
      " -shared -fPIC -ldl -fvisibility=hidden "; // linker: g++

  std::filesystem::path p_out; // a.out.so
  std::filesystem::path p_src; // src

  std::deque<std::filesystem::path> p_include; // deque<inclide>
  TaskStatus status = TaskStatus::pending;

  void run();
  /* ok on p_out create */

  std::deque<std::filesystem::path> p_libstatic;
  std::deque<std::filesystem::path> p_libsharedpath;
  std::deque<std::string> m_libsharedobj;
};
