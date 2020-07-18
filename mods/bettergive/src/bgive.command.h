#include <command.h>
#include <parameter.h>

using namespace BDL::CustomCommand;

command_register_function register_commands();

class givecommand : public CustomCommandContext {
public:
  static constexpr auto cmd_name = "bgive";
  static constexpr auto description = "Give someone something";
  static constexpr auto permission = CommandPermissionLevel::OP;

  givecommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void oncmd(mandatory<std::string> target, mandatory<int> id,
             mandatory<int> data, mandatory<int> count);
};
