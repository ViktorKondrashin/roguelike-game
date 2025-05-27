#pragma once
#include <vector>
#include <string>

struct MonsterWave {
  int x, y;           // Координаты (2,1)
  std::string type;    // Тип ("SLIME")
};

class WaveLoader {
public:
  static std::vector<MonsterWave> loadFromFile(const std::string& filePath);
};