
#include <mlr/modmain.h>
#include <mlr/logger.h>

#include "patch.h"
#include "haddri.h"

#include <cstdint>

//void* achieveguard_backup[6];

void onLoad(ModContext*){

#define hook_impl(i,j,k) \
haddri hook_impl_##i((void*)(j),(void*)(k));\
hook_impl_##i.enable();

hook_impl(0,0x062AF720,achieveguard_patch_2);
hook_impl(1,0x062AF6D0,achieveguard_patch_1);
hook_impl(2,0x062AF600,achieveguard_patch_2);
hook_impl(3,0x062AF610,achieveguard_patch_2);
hook_impl(4,0x05E3CDD0,achieveguard_patch_3);
hook_impl(5,0x05E20F00,achieveguard_patch_3);
hook_impl(6,0x05E3C880,achieveguard_patch_4);

#undef hook_impl
};


