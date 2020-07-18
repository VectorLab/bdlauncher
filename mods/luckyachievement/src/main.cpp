#include "la_execute.h"
#include <minecraft/core/getSP.h>
#include <minecraft/commands/CommandOrigin.h>
#include <minecraft/commands/CommandOutput.h>
#include <mlr/modmain.h>
#include <cmdreg.h>
#include <string>
#include <algorithm>

void oncmd(argVec &p1_args, CommandOrigin const &p2_origin,
           CommandOutput &p3_output, std::string_view p4_unknown) {
if(0==p1_args.size()){
p3_output.error("usage: /la <int:id>");
return;
}
if(1==p1_args.size()){
int v1_id=0;
try{
std::string v2_tmp(p1_args[0]);
v1_id=std::stoi(v2_tmp);
}catch(std::exception const& e){
p3_output.error(e.what());
return;
}
ServerPlayer* v2_sp = getSP(p2_origin.getEntity());
la_sendpacket_execute(v2_sp,v1_id);
p3_output.success("la: "+std::to_string(v1_id));
}else{
int v1_ids=0,v1_ide=0;
try{
std::string v2_t1(p1_args[0]);
v1_ids=std::stoi(v2_t1);
std::string v2_t2(p1_args[1]);                                v1_ide=std::stoi(v2_t2);
}catch(std::exception const& e){
p3_output.error(e.what());
return;
}
ServerPlayer* v2_sp = getSP(p2_origin.getEntity());
int v3_is=std::min(v1_ids,v1_ide),v3_ie=std::max(v1_ids,v1_ide);
for(int i=v3_is;i<=v3_ie;i++){
la_sendpacket_execute(v2_sp,i);
}
p3_output.success("la: "+std::to_string(v3_is)+" ~ "+std::to_string(v3_ie));
return;
}
};

void onLoad(ModContext* _tmp) {
  register_cmd("la", oncmd, "lucky achievement callable command",1);
//  la_test();
};

