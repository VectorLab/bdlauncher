#pragma once

#include "pubexport.h"

class ModContext;

extern "C" {
/* @arg1(memory=external) */
PUB_EXPORT void onLoad(ModContext *);
}
