
#include "mlrbase.h"

// 0-15 success, 16 error
char inline hexbin_unsafe(char p1){
if(48>p1){
return 16;
}
if(58>p1){
return p1-48;
}
if(65>p1){
return 16;
}
if(71>p1){
return p1-55;
}
if(97>p1){
return 16;
}
if(103>p1){
return p1-87;
}
return 16;
};


void mlrbase_hex2bin_unsafe(const char* p1,char* p2,std::size_t p3){
std::size_t v1=0,v3;
while(v1<p3){
v3=v1<<1;
p2[v1]=(((hexbin_unsafe(p1[v3]))&0xf)<<4)|((hexbin_unsafe(p1[v3|1]))&0xf);
++v1;
}
};

const char* hex="0123456789abcdef";

void mlrbase_bin2hex_unsafe(const char*p1,char*p2,std::size_t p3){
std::size_t v1=0,v3;
while(v1<p3){
v3=v1<<1;
p2[v3]=hex[(p1[v1]>>4)&0xf];
p2[v3|1]=hex[(p1[v1])&0xf];
++v1;
}
};

