#include "bgive.command.h"
#include <base.h>
#include <mlr/logger.h>
#include <mlr/modmain.h>
#include <utils.h>

void givecommand::oncmd(mandatory<std::string> target, mandatory<int> id,
                        mandatory<int> data, mandatory<int> count) {
  ItemStack *newitem = createItemStack(id, data, count);
  auto tgplayer = getplayer_byname2(target);
  if (tgplayer) {
    giveItem(*tgplayer, newitem);
    getOutput().success("Gave " + target + " item, ID:" + std::to_string(id) +
                        ":" + std::to_string(data) + ":" +
                        std::to_string(count));
  } else {
    getOutput().error("Failed");
  }
}

void onLoad(ModContext *__tmp_1) { register_commands(); }
