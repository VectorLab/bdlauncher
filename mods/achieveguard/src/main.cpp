
#include <mlr/modmain.h>
#include <mlr/PFishHook.h>
#include <mlr/logger.h>

//#include <mlrbase/base.h>

#include "patch.h"

#include <cstdint>

void* achieveguard_backup[6];

void onLoad(ModContext*){
std::size_t v1;//=mlrbase.bds_mem_offset;
v1=0;
std::size_t v2[]{
v1+0x0304d3c0,// LevelData::achievementsWillBeDisabledOnLoad(LevelData*)
v1+0x0304d390,// LevelData::disableAchievements(LevelData*)
v1+0x0304d380,// LevelData::hasAchievementsDisabled(LevelData*)
v1+0x02b80230,// LevelSettings::achievementsWillBeDisabledOnLoad(LevelSettings*)
v1+0x02b69ac0,// LevelSettings::hasAchievementsDisabled(LevelSettings*)
v1+0x02b7fd80 // LevelSettings::setAchievementsDisabled(LevelSettings*,bool)
};

#define hook_impl(i) \
HookIt((void*)(v2[i]),&achieveguard_backup[i],(void*)(achieveguard_patch_##i));

hook_impl(0);
hook_impl(1);
hook_impl(2);
hook_impl(3);
hook_impl(4);
hook_impl(5);

#undef hook_impl
};


