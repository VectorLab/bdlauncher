
#include <string>
#include <cstdlib>

#include <mlr/logger.h>
#include <mlr/modmain.h>
#include <mlr/modcontext.h>
#include <mlr/modloaderutil.h>

std::filesystem::path s_gendef;
const std::string e_1=".h",e_2=".command",e_3=".command.gen.cpp";

void child_load_callback(ModContext* mod_child) {
stdfs_dir_foreach_recursion_a(mod_child->p___src,[&](std::filesystem::path& p1){
if(!std::filesystem::is_regular_file(p1)){return false;}
if(0!=e_1.compare(p1.extension())){return false;}
std::filesystem::path v1=p1.stem();
if(0!=e_2.compare(v1.extension())){return false;}
std::filesystem::path v2=v1.stem();
std::filesystem::path v3=p1.parent_path();
v3/=v2;
v3+=e_3;
bool v4=std::filesystem::is_regular_file(v3);
if(v4){
int64_t v5=stdfs_filetime_to_ms(std::filesystem::last_write_time(p1)),
v6=stdfs_filetime_to_ms(std::filesystem::last_write_time(v3));
if(v5>v6){
v4=false;
}
}
if(v4){return false;}
do_log("GEN CMD DEF: %s => %s ...",p1.c_str(),v3.c_str());
mod_child->changed=true;
std::filesystem::remove_all(v3);
std::string v5=s_gendef.c_str();
v5+=" ";
v5+=p1.c_str();
v5+=" > ";
v5+=v3.c_str();
int32_t v6=system(v5.c_str());
if(0!=v6){
do_log("GEN CMD DEF failed: %s",p1.c_str());
exit(1);
}
return false;
}
);
};

void onLoad(ModContext* mod_this) {
std::filesystem::path v1=mod_this->resdir;
v1/="test.py";
if(0!=system(v1.c_str())){
do_log("bdl generate_command_definition.py disabled");
return;
}
s_gendef=mod_this->resdir;
s_gendef/="generate_command_definition.py";
mod_this->onChildLoad=child_load_callback;
};

