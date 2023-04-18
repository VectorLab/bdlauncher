
#include <mlr/PFishHook.h>

#include "haddri.h"

void haddri::enable(){
if(this->active){
return;
}
HookIt((void*)(this->target),&(this->origin),(void*)(this->inject));
this->active=true;
};

void haddri::disable(){
if(!this->active){
return;
}
//UnHook((void*)(this->target),)
this->active=false;
};

