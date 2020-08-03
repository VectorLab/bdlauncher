#include <mlr/statichook.h>
#include <base.h>
#include <mlr/modmain.h>
#include <mlr/modcontext.h>
#include <dbimpl.h>
#include "cname.command.h"

LDBImpl names;
unordered_map<string, string> name_map;
THook(
    void *, _ZN10TextPacket10createChatERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_S7_S7_, void *a1,
    string *s1, string *s2, string *s3, string *s4) {
  auto it = name_map.find(*s1);
  if (it != name_map.end()) {
    return original(a1, &it->second, s2, s3, s4);
  } else {
    return original(a1, s1, s2, s3, s4);
  }
}
static void load() {
  names.Iter([](string_view k, string_view v) { name_map.emplace(k, v); });
}
void cnamecmd::oncmd(mandatory<std::string> target, mandatory<std::string> name)
{
  SPBuf<1024> buf;
  for (decltype(name.size()) i = 0; i < name.size(); ++i) {
    if (name[i] != '"')
    buf.buf[buf.ptr++] = name[i];
  }
  name_map[string(target)] = buf.getstr();
  names.Put(target, buf.get());
  getOutput().success("Success");
}

void onLoad(ModContext* _c) {
  std::filesystem::path db_file=_c->datadir;
  if(!std::filesystem::is_directory(db_file)){
  std::filesystem::create_directories(db_file);
  }
  db_file/="CustName";
  names.load(db_file.c_str());
  load();
  register_commands();
}
