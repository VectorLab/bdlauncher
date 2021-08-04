
#include <mlr/mlrmain.h>

#include <cstdio>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <mlr/modloader.h>
#include <mlr/myhook.h>

int __bdlmlr_2(int argc, char **argv,char** envp) {
  printf("[BDL] start loader\n");
  launcher_modloaader_main();
  printf("[BDL] done , start launch server\n");
  int fk = ((typeof(&main))bdlmlr.old_bdsmain)(argc,argv,envp);
  printf("[BDL] server exit with %i\n", fk);
  return fk;
};

int __bdlmlr_1 (int (*main) (int, char **,char**),
		 int argc, char **argv,
		 __typeof (main) init,
		 void (*fini) (void),
		 void (*rtld_fini) (void), void *stack_end){
bdlmlr.old_bdsmain=fp(main);
printf("[BDL] libc enter: main = %p , loader_main = %p\n",main,__bdlmlr_2);
int r = ((typeof(&__libc_start_main))bdlmlr.old_libcsm)(&__bdlmlr_2,argc,argv,init,fini,rtld_fini,stack_end);
printf("[BDL] libc exit: return = %d\n",r);
return r;
};

struct init_d {
  init_d() {
    prctl(PR_SET_TIMERSLACK, 5000000);
    printf("[BDL] inject to %p pid %d\n", __libc_start_main, getpid());
    bdlmlr.old_libcsm = MyHook(fp(__libc_start_main), fp(__bdlmlr_1));
    setenv("LD_PRELOAD", "", 1);
  }
} _dummy;

