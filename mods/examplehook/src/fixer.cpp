#include "fixer.h"

bool fixer::fixLevelData(LevelData *this_ptr) {
  char *this_real = reinterpret_cast<char *>(this_ptr);
  if (0 == (*(this_real + 996) & 1)) {
    return false;
  }
  *(this_real + 996) = 0;
  return true;
};

bool fixer::fixLevelSettings(LevelSettings *this_ptr) {
  char *this_real = reinterpret_cast<char *>(this_ptr);
  if (0 == (*(this_real + 72) & 1)) {
    return false;
  }
  *(this_real + 72) = 0;
  return true;
};
