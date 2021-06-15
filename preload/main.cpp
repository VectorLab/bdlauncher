#include <cstdio>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <mlr/modloader.h>
#include <mlr/myhook.h>

#define LIBC_START_MAIN __libc_start_main

extern "C" {
int LIBC_START_MAIN (int (*main) (int, char **),
			    int argc,
			    char **argv,
			    __typeof (main) init,
			    void (*fini) (void),
			    void (*rtld_fini) (void),
			    void *stack_end)
     __attribute__ ((noreturn));
int main(int ac, char **av);
};

void* __bdlmlr_backup[3];

int __bdlmlr_2(int ac, char **av) {
  printf("[BDL] start loader\n");
  launcher_modloaader_main();
  printf("[BDL] done , start launch server\n");
  int fk = ((typeof(&main))__bdlmlr_backup[1])(ac, av);
  printf("[BDL] server exit with %i\n", fk);
  return fk;
};

int __bdlmlr_1 (int (*main) (int, char **),
		 int argc, char **argv,
		 __typeof (main) init,
		 void (*fini) (void),
		 void (*rtld_fini) (void), void *stack_end){
__bdlmlr_backup[1]=fp(main);
printf("[BDL] libc enter: main = %p , loader_main = %p\n",main,__bdlmlr_2);
int r = ((typeof(&LIBC_START_MAIN))__bdlmlr_backup[0])(&__bdlmlr_2,argc,argv,init,fini,rtld_fini,stack_end);
printf("[BDL] libc exit: return = %d\n",r);
return r;
};

struct init_d {
  init_d() {
    prctl(PR_SET_TIMERSLACK, 5000000);
    printf("[BDL] inject to %p pid %d\n", __libc_start_main, getpid());
    __bdlmlr_backup[0] = MyHook(fp(__libc_start_main), fp(__bdlmlr_1));
    setenv("LD_PRELOAD", "", 1);
  }
} _dummy;

