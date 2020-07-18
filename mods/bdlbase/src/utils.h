#pragma once
#include <mlr/pubexport.h>
#include <string>
class ItemStack;
class ServerPlayer;
struct ActorUniqueID;
class BlockSource;
class Vec3;

PUB_EXPORT ItemStack *createItemStack_static(short id, short aux,
                                             unsigned char amo, ItemStack *stk);
PUB_EXPORT ItemStack *createItemStack(short id, short aux, unsigned char amo);
PUB_EXPORT ItemStack *createItemStack_static(std::string const &name,
                                             unsigned char amo, ItemStack *stk);
PUB_EXPORT ItemStack *createItemStack(std::string const &name,
                                      unsigned char amo);

PUB_EXPORT void giveItem(ServerPlayer &sp, ItemStack *is);
PUB_EXPORT ActorUniqueID summon(BlockSource &bs, Vec3 const &vc,
                                std::string const &name);
PUB_EXPORT bool takeItem_unsafe(ServerPlayer &sp, ItemStack *is);
PUB_EXPORT bool takeItem(ServerPlayer &sp, ItemStack *is);


