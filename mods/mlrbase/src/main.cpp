
#include <mlr/modmain.h>
#include <mlr/logger.h>
#include <mlrbase/base.h>

#include <cstring>
#include <fstream>
#include <unistd.h>

struct mlrbase_t mlrbase;

void onLoad(ModContext* mcfg){
char v2[0x10000];
std::error_code v1;
mlrbase.bds_bin_path=std::filesystem::read_symlink("/proc/self/exe",v1);
if(v1){
do_log("Fatal error: failed to get bedrock_server real path: %d",v1.value());
return;
}
do_log_debug("bedrock_server path: %s",mlrbase.bds_bin_path.c_str());
std::string v3="nm --no-demangle -D ";
v3+=mlrbase.bds_bin_path;
v3+=" | grep _ZdlPv 2>&1";
FILE* v4=popen(v3.c_str(),"r");
if(nullptr==v4){
do_log("Fatal error: failed to launch nm tool");
return;
}
char* v5;
std::string v6;
const char* v7="                ";
while(!feof(v4)){
v5=fgets(v2,0x10000,v4);
//00000000037d3f60 T _ZdlPv
if((nullptr!=v5)&&(26==strlen(v2))){
if(0!=memcmp(v2,v7,16)){
v6=std::string(v2,16);
}
}
}
pclose(v4);
union{
char s[8];
void* p;
std::uint64_t l;
}v8,v9;
char va[8];
mlrbase_hex2bin_unsafe(v6.c_str(),va,8);
std::uint8_t vb=0;
for(;vb<8;++vb){
v8.s[(~vb)&0x7]=va[vb];
}
v9.p=(void*)(_ZdlPv);
mlrbase.bds_mem_offset=v9.l-v8.l;
do_log_debug("bin offset: %lX , mem offset: %lX , calc diff: %lX",v8.l,v9.l,mlrbase.bds_mem_offset);
// TODO hook db
};

