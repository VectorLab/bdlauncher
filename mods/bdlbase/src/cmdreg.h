#pragma once

#include "stl.hpp"
#include <minecraft/commands/CommandOrigin.h>
#include <minecraft/commands/CommandOutput.h>
#include <string>
#include <string_view>

typedef static_deque<string_view, 64> argVec;

PUB_EXPORT void register_cmd(const std::string &name,
                             void (*fn)(argVec &, CommandOrigin const &,
                                        CommandOutput &outp, std::string_view),
                             const std::string &desc = "mod command",
                             int permlevel = 0);

PUB_EXPORT void
register_cmd(const std::string &name,
             void (*fn)(argVec &, CommandOrigin const &, CommandOutput &outp),
             const std::string &desc = "mod command", int permlevel = 0);

PUB_EXPORT void register_cmd(const std::string &name, void (*fn)(void),
                             const std::string &desc = "mod command",
                             int permlevel = 0);
