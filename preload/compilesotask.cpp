
#include <mlr/compilesotask.h>
//#include <mlr/execshelltask.h>
#include <mlr/logger.h>
#include <mlr/modloaderutil.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <unistd.h>

void CompileSoTask::run() {
  this->status = TaskStatus::running;
  std::deque<std::filesystem::path> p_obj;

  std::string __I_include(" -I ");
  __I_include += path_mlr_def::p___mlr_sinclude.c_str();

  uint32_t i = 0, c = this->p_include.size();
  for (; i < c; i++) {
    __I_include += " -I ";
    __I_include += this->p_include.at(i).c_str();
  }

  int32_t pid = getpid();
  char mypid[24], __tmp_ic_1[16];
  sprintf(mypid, "%d", pid);

  int32_t __c_srcf = 0, __c_sccur;
  std::filesystem::path tmpdir = std::filesystem::temp_directory_path();
  bool ok = true;

  stdfs_dir_foreach_recursion_a(
      this->p_src, [&](std::filesystem::path &a_path) {
        std::string a_path_type_s(a_path.extension().c_str());
        mlr_filename_type a_path_type = mlr_filename_type_get(a_path_type_s);
        switch (a_path_type) {
        case mlr_filename_type::t_cpp: {
          std::string objname("__bedrock_task_");
          objname += mypid;
          __c_sccur = ++__c_srcf;
          sprintf(__tmp_ic_1, "_%d_.o", __c_sccur);
          objname += __tmp_ic_1;
          std::filesystem::path objfile = tmpdir;
          objfile /= objname.c_str();

          std::string cmd(path_mlr_def::p___mlr_sconfig_scxx.c_str());
          cmd += this->cxxflags;
          cmd += "-o ";
          cmd += objfile.c_str();
          cmd += ' ';
          cmd += a_path.c_str();
          cmd += ' ';
          cmd += __I_include;
          /*
          ExecShellTask cmdexec;
          cmdexec.cmd=cmd.c_str();
          cmdexec.run();
          bool cmdexec_notok=TaskStatus::fail==cmdexec.status;
          */
          bool cmdexec_notok = 0 != system(cmd.c_str());

          if (cmdexec_notok) {
            do_log("error: build failed\n%s", cmd.c_str());
            ok = false;
            return true;
          }
          p_obj.push_front(objfile);
          break;
        }
        case mlr_filename_type::t_c: {
          std::string objname("__bedrock_task_");
          objname += mypid;
          __c_sccur = ++__c_srcf;
          sprintf(__tmp_ic_1, "_%d_.o", __c_sccur);
          objname += __tmp_ic_1;
          std::filesystem::path objfile = tmpdir;
          objfile /= objname.c_str();

          std::string cmd(path_mlr_def::p___mlr_sconfig_scc.c_str());
          cmd += this->cflags;
          cmd += "-o ";
          cmd += objfile.c_str();
          cmd += ' ';
          cmd += a_path.c_str();
          cmd += ' ';
          cmd += __I_include;

          /*
          ExecShellTask cmdexec;
          cmdexec.cmd=cmd.c_str();
          cmdexec.run();
          bool cmdexec_notok=TaskStatus::fail==cmdexec.status;
          */
          bool cmdexec_notok = 0 != system(cmd.c_str());

          if (cmdexec_notok) {
            do_log("error: build failed\n%s", cmd.c_str());
            ok = false;
            return true;
          }
          p_obj.push_front(objfile);
          break;
        }
        default: {
          return false;
        }
        }
        return false;
      });
  if (!ok) {
    this->status = TaskStatus::fail;
    return;
  }

  std::string linkcmdstr(path_mlr_def::p___mlr_sconfig_scxx.c_str());
  linkcmdstr += this->ldflags;
  linkcmdstr += "-o ";
  linkcmdstr += this->p_out.c_str();

  i = 0;
  c = p_obj.size();
  for (; i < c; i++) {
    linkcmdstr += ' ';
    linkcmdstr += p_obj.at(i).c_str();
  }

  i = 0;
  c = this->p_libstatic.size();
  for (; i < c; i++) {
    linkcmdstr += ' ';
    linkcmdstr += this->p_libstatic.at(i).c_str();
  }

  i = 0;
  c = this->p_libsharedpath.size();
  for (; i < c; i++) {
    linkcmdstr += " -L";
    linkcmdstr += this->p_libsharedpath.at(i).c_str();
  }

  i = 0;
  c = this->m_libsharedobj.size();
  for (; i < c; i++) {
    linkcmdstr += " -l";
    linkcmdstr += this->m_libsharedobj.at(i);
  }
  /*
  ExecShellTask linkcmdexec;
  linkcmdexec.cmd=linkcmdstr.c_str();
  linkcmdexec.run();
  bool linkcmdexec_notok=TaskStatus::fail==linkcmdexec.status;
  */
  bool linkcmdexec_notok = 0 != system(linkcmdstr.c_str());

  if (linkcmdexec_notok) {
    do_log("error: build failed\n%s", linkcmdstr.c_str());
    this->status = TaskStatus::fail;
    return;
  }

  i = 0;
  c = p_obj.size();
  for (; i < c; i++) {
    std::filesystem::remove(p_obj.at(i));
  }

  this->status = TaskStatus::finish;
};

