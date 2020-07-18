#include <command.h>
#include <parameter.h>

using namespace BDL::CustomCommand;

command_register_function register_commands();

enum class Exit { exit };
enum class AB { a, b };
enum class LandQuery { query };
enum class Buy { buy };
enum class Sell { sell };
enum class Trust { trust };
enum class Untrust { untrust };
enum class Trustgui { trustgui };
enum class Untrustgui { untrustgui };
enum class Perm { perm };
enum class Give { give };

struct LDCommand : CustomCommandContext {
public:
  inline static alias_list aliases = {"ld"};
  static constexpr auto cmd_name = "land";
  static constexpr auto description = "Land functions";
  static constexpr auto permission = CommandPermissionLevel::NORMAL;

  LDCommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void exit(mandatory<Exit> cmd);
  void AB_(mandatory<AB> cmd);
  void query(mandatory<LandQuery> cmd);
  void buy(mandatory<Buy> cmd);
  void sell(mandatory<Sell> cmd);
  void trust(mandatory<Trust> cmd, mandatory<std::string> target);
  void untrust(mandatory<Untrust> cmd, mandatory<std::string> target);
  void trustgui(mandatory<Trustgui> cmd);
  void untrustgui(mandatory<Untrustgui> cmd);
  void perm(mandatory<Perm> cmd, mandatory<int> perm);
  void give(mandatory<Give> cmd, mandatory<std::string> target);
};

// op
enum class Forceperm { forceperm };
enum class Dumpall { dumpall };
enum class Fix { fix };
enum class LandReload { reload };
struct LDOCommand : CustomCommandContext {
public:
  // inline static alias_list aliases  = {"ld"};
  static constexpr auto cmd_name = "land_op";
  static constexpr auto description = "Land functions";
  static constexpr auto permission = CommandPermissionLevel::OP;

  LDOCommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void fix(mandatory<Fix> cmd);
  void forceperm(mandatory<Forceperm> cmd, mandatory<int> perm);
  void dumpall(mandatory<Dumpall> cmd);
  void reload(mandatory<LandReload> cmd);
};
