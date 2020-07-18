#pragma once

#include "taskstatus.h"

class ExecShellTask {
public:
  const char *cmd;
  int p_status;
  TaskStatus status = TaskStatus::pending;
  void run();
};
