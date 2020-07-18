#include <cstdio>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <mlr/modloader.h>
#include <mlr/myhook.h>

extern "C" {
int main(int ac, char **av);
};
static void *old_main;

static int mc_entry(int ac, char **av) {
  printf("[BDL] start loader\n");
  launcher_modloaader_main();
  printf("[BDL] done , start launch server\n");
  int fk = ((typeof(&main))old_main)(ac, av);
  printf("[BDL] server exit with %i\n", fk);
  return fk;
};

struct init_d {
  init_d() {
    prctl(PR_SET_TIMERSLACK, 5000000);
    printf("[BDL] inject to %p pid %d\n", main, getpid());
    old_main = MyHook(fp(main), fp(mc_entry));
    setenv("LD_PRELOAD", "", 1);
  }
} _dummy;
