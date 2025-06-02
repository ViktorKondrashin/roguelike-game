#pragma once
#include <vector>
#include <string>

struct MonsterWave {
  int x, y;
  std::string type;
};

class WaveLoader {
public:
  static std::vector<MonsterWave> loadFromFile(const std::string& filePath);
};