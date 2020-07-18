#pragma once

#include <vector>

class ItemStack;
enum class ContainerID : unsigned char { DEFAULT = 0 };

class PlayerInventory {
public:
  bool add(ItemStack &, bool);
  bool canAdd(ItemStack const &)const;
  void clearInventory(int);
  void clearInventoryWithDefault(bool);
  int getHotbarSize()const;
  std::vector<ItemStack const *> getSlots() const;
  bool setItem(int, ItemStack const &, ContainerID,bool);
  bool removeResource(ItemStack const &);
  bool removeResource(int);
  
  ItemStack &getItem(int, ContainerID) const;
  int getContainerSize(ContainerID) const;
  ContainerID clearSlot(int, ContainerID);
};
