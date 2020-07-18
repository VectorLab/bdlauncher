#include <mlr/global.h>
#include <mlr/logger.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

static INLINE char get_log_level(int level) {
  switch (level) {
  case 0:
    return 'D';
  case 1:
    return 'I';
  default:
    return 'U';
  }
};

PUB_EXPORT void do_log_impl(char const *filename, int line, int level,
                            char const *format, ...) {
  va_list vas;
  va_start(vas, format);
  char *buffer;
  vasprintf(&buffer, format, vas);
  defer_(buffer, free);
  va_end(vas);
  printf("%c [%s:%d] %s\n", get_log_level(level), filename, line, buffer);
};
