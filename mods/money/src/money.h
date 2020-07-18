#pragma once
#include <mlr/pubexport.h>
#include <string_view>

PUB_EXPORT int money_get(std::string_view pn);
PUB_EXPORT void money_set(std::string_view pn, int am);
PUB_EXPORT bool money_red(std::string_view pn, int am);
PUB_EXPORT void money_add(std::string_view pn, int am);
