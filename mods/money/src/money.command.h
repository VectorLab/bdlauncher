#include <command.h>
#include <parameter.h>

using namespace BDL::CustomCommand;

command_register_function register_commands();

enum class Query { query };
enum class Pay { pay };
enum class Paygui { paygui };

struct MoneyCommand : CustomCommandContext {
public:
  static constexpr auto cmd_name = "money";
  static constexpr auto description = "Money system";
  static constexpr auto permission = CommandPermissionLevel::NORMAL;

  MoneyCommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void query(mandatory<Query> cmd);
  void pay(mandatory<Pay> cmd, mandatory<std::string> target,
           mandatory<int> count);
  void paygui(mandatory<Paygui> cmd);
};

enum class Add { add };
enum class Reduce { reduce };
enum class Set { set };
enum class Reloadcfg { reload };
enum class OpQuery { query };

struct MoneyOP : CustomCommandContext {
public:
  static constexpr auto cmd_name = "money_op";
  static constexpr auto description = "Money admin command";
  static constexpr auto permission = CommandPermissionLevel::OP;

  MoneyOP(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void OPQUERY(mandatory<OpQuery> cmd, mandatory<std::string> target);
  void add(mandatory<Add> cmd, mandatory<std::string> target,
           mandatory<int> count);
  void reduce(mandatory<Reduce> cmd, mandatory<std::string> target,
              mandatory<int> count);
  void set(mandatory<Set> cmd, mandatory<std::string> target,
           mandatory<int> count);
  void reloadcfg(mandatory<Reloadcfg> cmd);
};
