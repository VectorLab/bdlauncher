#pragma once

#include "pubexport.h"

#ifdef DEBUG
#define do_log_debug(...) do_log_impl(__FILE__, __LINE__, 0, __VA_ARGS__)
#else
#define do_log_debug(...)
#endif
#define do_log(...) do_log_impl(__FILE__, __LINE__, 1, __VA_ARGS__)

PUB_EXPORT void do_log_impl(char const *filename, int line, int level,
                            char const *format, ...)
    __attribute__((format(printf, 4, 5)));
