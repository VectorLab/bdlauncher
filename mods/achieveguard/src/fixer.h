#pragma once
class LevelData;
class LevelSettings;

class fixer {
public:
  static bool fixLevelData(LevelData *);
  static bool fixLevelSettings(LevelSettings *);
};