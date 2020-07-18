#pragma once

#include <string>

#include <minecraft/json.h>

struct ScriptCommandCallbackData {
  std::string result;
  bool success;
  Json::Value extra;
};
