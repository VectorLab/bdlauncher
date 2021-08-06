
#include <mlr/logger.h>

#include "patch.h"

bool achieveguard_patch_0(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h0 %hx",v1[1116]);
v1[1116]=0;
return false;
};

void achieveguard_patch_1(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h1 %hx",v1[1116]);
v1[1116]=0;
};

bool achieveguard_patch_2(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h2 %hx",v1[1116]);
v1[1116]=0;
return false;
};

bool achieveguard_patch_3(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h3 %hx",v1[72]);
v1[72]=0;
return false;
};

bool achieveguard_patch_4(void* p1){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h4 %hx",v1[72]);
v1[72]=0;
return false;
};

void* achieveguard_patch_5(void* p1,bool){
char* v1=reinterpret_cast<char*>(p1);
do_log_debug("h5 %hx",v1[72]);
v1[72]=0;
return p1;
};

