#pragma once
#include <cstdint>
#include <cstring>
#include <sys/mman.h>
#include <unordered_map>
#ifndef fp
#define fp(a) ((void *)(a))
#endif
#define fpm(a) fp(&a)
void *MyHook(void *oldunc, void *newfunc);
char *wr_jmp(void *addr);
typedef uint64_t u64;
char *wr_regcmd(void *call, void *arg);
