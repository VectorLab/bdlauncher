// Generated by scripts/generate_command_definition.py

#include "tp.command.h"

template <> class BDL::CustomCommand::CommandParameterProxy<Cancel> {
  Cancel value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, Cancel>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "Cancel";

  operator Cancel() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<Ac> {
  Ac value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, Ac>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "Ac";

  operator Ac() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<De> {
  De value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, De>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "De";

  operator De() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<Gui> {
  Gui value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, Gui>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "Gui";

  operator Gui() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<CmdF> {
  CmdF value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, CmdF>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "CmdF";

  operator CmdF() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<CmdT> {
  CmdT value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, CmdT>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "CmdT";

  operator CmdT() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeGo> {
  HomeGo value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeGo>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeGo";

  operator HomeGo() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeAdd> {
  HomeAdd value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeAdd>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeAdd";

  operator HomeAdd() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeDel> {
  HomeDel value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeDel>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeDel";

  operator HomeDel() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeLs> {
  HomeLs value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeLs>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeLs";

  operator HomeLs() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeGui> {
  HomeGui value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeGui>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeGui";

  operator HomeGui() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<HomeDelGui> {
  HomeDelGui value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, HomeDelGui>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "HomeDelGui";

  operator HomeDelGui() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<WarpGo> {
  WarpGo value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, WarpGo>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "WarpGo";

  operator WarpGo() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<WarpAdd> {
  WarpAdd value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, WarpAdd>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "WarpAdd";

  operator WarpAdd() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<WarpDel> {
  WarpDel value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, WarpDel>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "WarpDel";

  operator WarpDel() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<WarpLs> {
  WarpLs value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, WarpLs>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "WarpLs";

  operator WarpLs() const noexcept { return value; }
};

template <> class BDL::CustomCommand::CommandParameterProxy<WarpGUI> {
  WarpGUI value;

public:
  static auto fetch_tid() { return type_id<CommandRegistry, WarpGUI>(); }
  static constexpr auto parser = &CommandRegistry::fake_parse;
  static constexpr auto type = CommandParameterDataType::NORMAL;
  static constexpr auto enum_name = "WarpGUI";

  operator WarpGUI() const noexcept { return value; }
};

struct TPACommand_0 : Command {
  CommandParameterProxy<Cancel> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.CANCEL(cmd);
  }
};

struct TPACommand_1 : Command {
  CommandParameterProxy<Ac> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.AC(cmd);
  }
};

struct TPACommand_2 : Command {
  CommandParameterProxy<De> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.DE(cmd);
  }
};

struct TPACommand_3 : Command {
  CommandParameterProxy<Gui> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.GUI(cmd);
  }
};

struct TPACommand_4 : Command {
  CommandParameterProxy<CmdF> cmd;
  CommandParameterProxy<std::string> target;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.CMDF(cmd, target);
  }
};

struct TPACommand_5 : Command {
  CommandParameterProxy<CmdT> cmd;
  CommandParameterProxy<std::string> target;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    TPACommand context{origin, output};
    context.CMDT(cmd, target);
  }
};

struct HOMECommand_0 : Command {
  CommandParameterProxy<HomeGo> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HGO(cmd, name);
  }
};

struct HOMECommand_1 : Command {
  CommandParameterProxy<HomeAdd> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HADD(cmd, name);
  }
};

struct HOMECommand_2 : Command {
  CommandParameterProxy<HomeDel> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HDEL(cmd, name);
  }
};

struct HOMECommand_3 : Command {
  CommandParameterProxy<HomeLs> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HLS(cmd);
  }
};

struct HOMECommand_4 : Command {
  CommandParameterProxy<HomeGui> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HGUI(cmd);
  }
};

struct HOMECommand_5 : Command {
  CommandParameterProxy<HomeDelGui> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    HOMECommand context{origin, output};
    context.HDELGUI(cmd);
  }
};

struct WARPCommand_0 : Command {
  CommandParameterProxy<WarpGo> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    WARPCommand context{origin, output};
    context.WGO(cmd, name);
  }
};

struct WARPCommand_1 : Command {
  CommandParameterProxy<WarpAdd> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    WARPCommand context{origin, output};
    context.WADD(cmd, name);
  }
};

struct WARPCommand_2 : Command {
  CommandParameterProxy<WarpDel> cmd;
  CommandParameterProxy<std::string> name;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    WARPCommand context{origin, output};
    context.WDEL(cmd, name);
  }
};

struct WARPCommand_3 : Command {
  CommandParameterProxy<WarpLs> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    WARPCommand context{origin, output};
    context.WLS(cmd);
  }
};

struct WARPCommand_4 : Command {
  CommandParameterProxy<WarpGUI> cmd;
  virtual void execute(CommandOrigin const &origin,
                       CommandOutput &output) override {
    WARPCommand context{origin, output};
    context.WGUI(cmd);
  }
};

void register_commands() {
  auto &instance = CustomCommandRegistry::getInstance();
  {
    auto &item = instance.registerEnum<Cancel>();
    item.addValue("cancel", Cancel::cancel);
  }
  {
    auto &item = instance.registerEnum<Ac>();
    item.addValue("ac", Ac::ac);
  }
  {
    auto &item = instance.registerEnum<De>();
    item.addValue("de", De::de);
  }
  {
    auto &item = instance.registerEnum<Gui>();
    item.addValue("gui", Gui::gui);
  }
  {
    auto &item = instance.registerEnum<CmdF>();
    item.addValue("f", CmdF::f);
  }
  {
    auto &item = instance.registerEnum<CmdT>();
    item.addValue("t", CmdT::t);
  }
  {
    auto &item = instance.registerEnum<HomeGo>();
    item.addValue("go", HomeGo::go);
  }
  {
    auto &item = instance.registerEnum<HomeAdd>();
    item.addValue("add", HomeAdd::add);
  }
  {
    auto &item = instance.registerEnum<HomeDel>();
    item.addValue("del", HomeDel::del);
  }
  {
    auto &item = instance.registerEnum<HomeLs>();
    item.addValue("ls", HomeLs::Ls);
  }
  {
    auto &item = instance.registerEnum<HomeGui>();
    item.addValue("gui", HomeGui::gui);
  }
  {
    auto &item = instance.registerEnum<HomeDelGui>();
    item.addValue("delgui", HomeDelGui::delgui);
  }
  {
    auto &item = instance.registerEnum<WarpGo>();
    item.addValue("go", WarpGo::go);
  }
  {
    auto &item = instance.registerEnum<WarpAdd>();
    item.addValue("add", WarpAdd::add);
  }
  {
    auto &item = instance.registerEnum<WarpDel>();
    item.addValue("del", WarpDel::del);
  }
  {
    auto &item = instance.registerEnum<WarpLs>();
    item.addValue("ls", WarpLs::ls);
  }
  {
    auto &item = instance.registerEnum<WarpGUI>();
    item.addValue("gui", WarpGUI::gui);
  }
  {
    auto &cmd = instance.registerCommand<TPACommand>();
    {
      auto &ovl = cmd.registerOverload<TPACommand_0>();
      ovl.addParameter<Cancel>("cmd", false, offsetof(TPACommand_0, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<TPACommand_1>();
      ovl.addParameter<Ac>("cmd", false, offsetof(TPACommand_1, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<TPACommand_2>();
      ovl.addParameter<De>("cmd", false, offsetof(TPACommand_2, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<TPACommand_3>();
      ovl.addParameter<Gui>("cmd", false, offsetof(TPACommand_3, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<TPACommand_4>();
      ovl.addParameter<CmdF>("cmd", false, offsetof(TPACommand_4, cmd));
      ovl.addParameter<std::string>("target", false,
                                    offsetof(TPACommand_4, target));
    }
    {
      auto &ovl = cmd.registerOverload<TPACommand_5>();
      ovl.addParameter<CmdT>("cmd", false, offsetof(TPACommand_5, cmd));
      ovl.addParameter<std::string>("target", false,
                                    offsetof(TPACommand_5, target));
    }
  }
  {
    auto &cmd = instance.registerCommand<HOMECommand>();
    {
      auto &ovl = cmd.registerOverload<HOMECommand_0>();
      ovl.addParameter<HomeGo>("cmd", false, offsetof(HOMECommand_0, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(HOMECommand_0, name));
    }
    {
      auto &ovl = cmd.registerOverload<HOMECommand_1>();
      ovl.addParameter<HomeAdd>("cmd", false, offsetof(HOMECommand_1, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(HOMECommand_1, name));
    }
    {
      auto &ovl = cmd.registerOverload<HOMECommand_2>();
      ovl.addParameter<HomeDel>("cmd", false, offsetof(HOMECommand_2, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(HOMECommand_2, name));
    }
    {
      auto &ovl = cmd.registerOverload<HOMECommand_3>();
      ovl.addParameter<HomeLs>("cmd", false, offsetof(HOMECommand_3, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<HOMECommand_4>();
      ovl.addParameter<HomeGui>("cmd", false, offsetof(HOMECommand_4, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<HOMECommand_5>();
      ovl.addParameter<HomeDelGui>("cmd", false, offsetof(HOMECommand_5, cmd));
    }
  }
  {
    auto &cmd = instance.registerCommand<WARPCommand>();
    {
      auto &ovl = cmd.registerOverload<WARPCommand_0>();
      ovl.addParameter<WarpGo>("cmd", false, offsetof(WARPCommand_0, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(WARPCommand_0, name));
    }
    {
      auto &ovl = cmd.registerOverload<WARPCommand_1>();
      ovl.addParameter<WarpAdd>("cmd", false, offsetof(WARPCommand_1, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(WARPCommand_1, name));
    }
    {
      auto &ovl = cmd.registerOverload<WARPCommand_2>();
      ovl.addParameter<WarpDel>("cmd", false, offsetof(WARPCommand_2, cmd));
      ovl.addParameter<std::string>("name", false,
                                    offsetof(WARPCommand_2, name));
    }
    {
      auto &ovl = cmd.registerOverload<WARPCommand_3>();
      ovl.addParameter<WarpLs>("cmd", false, offsetof(WARPCommand_3, cmd));
    }
    {
      auto &ovl = cmd.registerOverload<WARPCommand_4>();
      ovl.addParameter<WarpGUI>("cmd", false, offsetof(WARPCommand_4, cmd));
    }
  }
}
