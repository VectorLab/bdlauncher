#include <command.h>
#include <parameter.h>

using namespace BDL::CustomCommand;

command_register_function register_commands();

class cnamecmd : public CustomCommandContext
{
public:
    static constexpr auto cmd_name = "cname";
    static constexpr auto description = "Customize name";
    static constexpr auto permission = CommandPermissionLevel::OP;

    cnamecmd(CommandOrigin const &origin, CommandOutput &output) noexcept : CustomCommandContext(origin, output) {}

    void oncmd(mandatory<std::string> target, mandatory<std::string> name);
};
