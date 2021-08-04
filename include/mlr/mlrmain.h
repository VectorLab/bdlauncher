
extern "C" {
int __libc_start_main (int (*main) (int, char **,char**),
			    int argc,
			    char **argv,
			    __typeof (main) init,
			    void (*fini) (void),
			    void (*rtld_fini) (void),
			    void *stack_end)
     __attribute__ ((noreturn));
int main(int, char **,char**);
};

struct bdlmlr_t{
void* old_libcsm=nullptr;
void* old_bdsmain=nullptr;
}bdlmlr;

int __bdlmlr_1 (int (*main) (int, char **,char**),
		 int argc, char **argv,
		 __typeof (main) init,
		 void (*fini) (void),
		 void (*rtld_fini) (void), void *stack_end);

int __bdlmlr_2(int argc, char **argv,char** envp);

