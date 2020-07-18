#include "fixer.h"

bool fixer::fixLevelData(LevelData *this_ptr) {
  char *this_real = reinterpret_cast<char *>(this_ptr);
  if (0 == (*(this_real + 1076) & 1)) {
    return false;
  }
  *(this_real + 1076) = 0;
  return true;
};

bool fixer::fixLevelSettings(LevelSettings *this_ptr) {
  char *this_real = reinterpret_cast<char *>(this_ptr);
  if (0 == (*(this_real + 20) & 1)) {
    return false;
  }
  *(this_real + 20) = 0;
  return true;
};
