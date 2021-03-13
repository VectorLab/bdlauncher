#include "fixer.h"
#include <mlr/logger.h>
#include <mlr/statichook.h>

THook(bool, _ZNK13LevelSettings23hasAchievementsDisabledEv, LevelSettings *this_ptr) {
  if (fixer::fixLevelSettings(this_ptr)) {
    do_log("error fixed: LevelSettings::hasAchievementsDisabled");
  }
  return false;
};

THook(LevelSettings*, _ZN13LevelSettings22setAchievementDisabledEb, LevelSettings* this_ptr, bool p1){
  do_log("LevelSettings request set AchievementDisabled to %d",p1);
  if (fixer::fixLevelSettings(this_ptr)) {
    do_log("error fixed: LevelSettings::setAchievementDisabled");
  }
  return this_ptr;
};

THook(bool, _ZNK13LevelSettings32achievementsWillBeDisabledOnLoadEv,
      LevelSettings *this_ptr) {
  return false;
};

THook(bool, _ZNK9LevelData23hasAchievementsDisabledEv, LevelData *this_ptr) {
  if (fixer::fixLevelData(this_ptr)) {
    do_log("error fixed: LevelData::hasAchievementsDisabled");
  }
  return false;
};

THook(void, _ZN9LevelData19disableAchievementsEv, LevelData *this_ptr) {
  if (fixer::fixLevelData(this_ptr)) {
    do_log("error skipped: LevelData::disableAchievements");
  } else {
    do_log("error fixed: LevelData::disableAchievements");
  }
};

THook(bool, _ZNK9LevelData32achievementsWillBeDisabledOnLoadEv,
      LevelData *this_ptr) {
  return false;
};
