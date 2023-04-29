
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

hook_impl(0,0x06567B20,achieveguard_patch_2);
hook_impl(1,0x06563820,achieveguard_patch_1);
hook_impl(2,0x06567A70,achieveguard_patch_2);
hook_impl(3,0x06563760,achieveguard_patch_2);
hook_impl(4,0x0611CFB0,achieveguard_patch_3);
hook_impl(5,0x06104980,achieveguard_patch_3);
hook_impl(6,0x0611C9C0,achieveguard_patch_4);

#undef hook_impl
};


