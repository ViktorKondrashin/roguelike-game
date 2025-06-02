
#include <memory> 
#include <vector>
#include <string>
#include "Monster.h"
#include "WaveLoader.h"

class WaveManager {
private:
  struct SpawnPoint {
    int x, y;
    std::string type;
  };
  float waveDelay = 0.f;
  bool isWaitingBetweenWaves = false; 
  Player* player = nullptr;


public:
  void loadRoomWaves(const std::string& roomBaseName);
  void spawnNextWave(Player* player);
  bool isWaveCleared() const;
  bool areAllWavesDone() const;
  void startNextWave(Player* player);
  void update(float deltaTime);
  bool isWaiting() const;
  int getCurrentWaveIndex() const { return currentWaveIndex; }
  int getTotalWaves() const { return waves.size(); }
  float getRemainingDelay() const { return waveDelay; }

  std::vector<std::vector<SpawnPoint>> waves;
  size_t currentWaveIndex = 0;
  std::vector<std::shared_ptr<Monster>> monsters;
};