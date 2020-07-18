#pragma once
#include <command.h>
#include <parameter.h>

using namespace BDL::CustomCommand;

command_register_function register_commands();

enum class Cancel { cancel };
enum class Ac { ac };
enum class De { de };
enum class Gui { gui };
enum class CmdF { f };
enum class CmdT { t };

class TPACommand : CustomCommandContext {
public:
  // inline static alias_list aliases  = {"tpa"};
  static constexpr auto cmd_name = "tpa";
  static constexpr auto description = "Teleport";
  static constexpr auto permission = CommandPermissionLevel::NORMAL;

  TPACommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void CANCEL(mandatory<Cancel> cmd);
  void AC(mandatory<Ac> cmd);
  void DE(mandatory<De> cmd);
  void GUI(mandatory<Gui> cmd);
  void CMDF(mandatory<CmdF> cmd, mandatory<std::string> target);
  void CMDT(mandatory<CmdT> cmd, mandatory<std::string> target);
};

enum class HomeGo { go };
enum class HomeAdd { add };
enum class HomeDel { del };
enum class HomeLs { Ls };
enum class HomeGui { gui };
enum class HomeDelGui { delgui };

class HOMECommand : CustomCommandContext {
public:
  static constexpr auto cmd_name = "home";
  static constexpr auto description = "Home command";
  static constexpr auto permission = CommandPermissionLevel::NORMAL;

  HOMECommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void HGO(mandatory<HomeGo> cmd, mandatory<std::string> name);
  void HADD(mandatory<HomeAdd> cmd, mandatory<std::string> name);
  void HDEL(mandatory<HomeDel> cmd, mandatory<std::string> name);
  void HLS(mandatory<HomeLs> cmd);
  void HGUI(mandatory<HomeGui> cmd);
  void HDELGUI(mandatory<HomeDelGui> cmd);
};

enum class WarpGo { go };
enum class WarpAdd { add };
enum class WarpDel { del };
enum class WarpLs { ls };
enum class WarpGUI { gui };

class WARPCommand : CustomCommandContext {
public:
  static constexpr auto cmd_name = "warp";
  static constexpr auto description = "Warp command";
  static constexpr auto permission = CommandPermissionLevel::NORMAL;

  WARPCommand(CommandOrigin const &origin, CommandOutput &output) noexcept
      : CustomCommandContext(origin, output) {}

  void WGO(mandatory<WarpGo> cmd, mandatory<std::string> name);
  void WADD(mandatory<WarpAdd> cmd, mandatory<std::string> name);
  void WDEL(mandatory<WarpDel> cmd, mandatory<std::string> name);
  void WLS(mandatory<WarpLs> cmd);
  void WGUI(mandatory<WarpGUI> cmd);
};
