
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
  float waveDelay = 0.f;  // Таймер паузы
  bool isWaitingBetweenWaves = false; 
  Player* player = nullptr;


public:
  void loadRoomWaves(const std::string& roomBaseName); // NEW: Загружает все волны комнаты
  void spawnNextWave(Player* player);         // NEW: Переименовано из spawnCurrentWave
  bool isWaveCleared() const;
  bool areAllWavesDone() const;               // NEW: Проверка окончания всех волн
  void startNextWave(Player* player);
  void update(float deltaTime);  // Обновляет таймер
  bool isWaiting() const;        // Проверка паузы
  int getCurrentWaveIndex() const { return currentWaveIndex; }
  int getTotalWaves() const { return waves.size(); }
  float getRemainingDelay() const { return waveDelay; }

  std::vector<std::vector<SpawnPoint>> waves; // NEW: Все волны комнаты
  size_t currentWaveIndex = 0;                // NEW: Текущая волна
  std::vector<std::shared_ptr<Monster>> monsters;
};