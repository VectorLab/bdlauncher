#include <mlr/execshelltask.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

char **environ;

void ExecShellTask::run() {
  this->status = TaskStatus::running;
  pid_t a_pid;
  // char a_argv_1[]="sh",
  //		a_argv_2[]="-c";
  // char* const a_argv[]={a_argv_1,a_argv_2,this->cmd,nullptr};

  const char *const a_argv[] = {"sh", "-c", this->cmd, nullptr};
  int a_status;
  a_status = posix_spawn(&a_pid, "/bin/sh", nullptr, nullptr,
                         const_cast<char *const *>(a_argv), environ);
  if (a_status == 0) {
    if (waitpid(a_pid, &a_status, 0) != -1) {
      this->status = TaskStatus::finish;
      this->p_status = a_status;
    } else {
      this->status = TaskStatus::fail;
    }
  } else {
    this->status = TaskStatus::fail;
  }
};

