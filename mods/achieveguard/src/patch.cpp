
#include <mlr/logger.h>

#include "patch.h"

void achieveguard_patch_1(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h1 %hx",v1[1172]);
v1[1172]=0;
};

bool achieveguard_patch_2(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h2 %hx",v1[1172]);
v1[1172]=0;
return false;
};

bool achieveguard_patch_3(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h3 %hx",v1[80]);
v1[80]=0;
return false;
};

void* achieveguard_patch_4(void* p1,bool){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h5 %hx",v1[80]);
v1[80]=0;
return p1;
};

