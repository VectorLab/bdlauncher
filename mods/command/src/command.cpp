#include "command.h"
#include <mlr/statichook.h>
#include <cstring>
#include <list>

namespace BDL::CustomCommand {

PUB_EXPORT CustomCommandRegistry &CustomCommandRegistry::getInstance() {
  static CustomCommandRegistry registry;
  return registry;
}

THook(void *, _ZN9XPCommand5setupER15CommandRegistry, CommandRegistry &thi) {
  auto &instance = CustomCommandRegistry::getInstance();
  instance.startRegister(&thi);
  return original(thi);
}

} // namespace BDL::CustomCommand
