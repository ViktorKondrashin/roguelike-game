#include "WaveManager.h"
#include "MonsterFactory.h"
#include <iostream>

void WaveManager::loadRoomWaves(const std::string& roomBaseName) {
  waves.clear();
  currentWaveIndex = 0;

  // NEW: Загружаем все волны комнаты (room1_1.waves, room1_2.waves...)
  int waveNum = 1;
  while (true) {
    std::string path = "data/" + roomBaseName + "_" + std::to_string(waveNum) + ".waves";
    auto loadedWave = WaveLoader::loadFromFile(path);

    if (loadedWave.empty()) break; // Файла нет -> конец волн

    // Конвертируем MonsterWave -> SpawnPoint
    std::vector<SpawnPoint> wave;
    for (const auto& spawn : loadedWave) {
      wave.push_back({ spawn.x, spawn.y, spawn.type });
    }

    waves.push_back(wave);
    waveNum++;
  }
  std::cout << "Loaded " << waves.size() << " waves for room " << roomBaseName << std::endl;
}

void WaveManager::spawnNextWave(Player* player) {
  if (currentWaveIndex >= waves.size()) return;

  monsters.clear();
  for (const auto& spawn : waves[currentWaveIndex]) {
    auto monster = MonsterFactory::create(
      spawn.type,
      spawn.x * 32.0f,
      spawn.y * 32.0f,
      32.0f, 32.0f,
      "image/" + spawn.type + ".png",
      player
    );
    if (monster) {
      monsters.push_back(std::move(monster));
      std::cout << "Spawned " << spawn.type << " at ("
        << spawn.x * 32 << "," << spawn.y * 32 << ")" << std::endl;
    }
  }
  currentWaveIndex++;
}

bool WaveManager::isWaveCleared() const {
  for (const auto& monster : monsters) {
    if (monster->life) return false;
  }
  return true;
}

bool WaveManager::areAllWavesDone() const {
  return currentWaveIndex >= waves.size();
}

void WaveManager::startNextWave(Player* player) {
  this->player = player;
  if (currentWaveIndex >= waves.size()) return;
  // Устанавливаем паузу 5 секунд после первой волны
  if (currentWaveIndex > 0) {
    waveDelay = 5.0f;
    isWaitingBetweenWaves = true;
    return;
  }

  spawnNextWave(player);
}

void WaveManager::update(float deltaTime) {
  if (isWaitingBetweenWaves) {
    waveDelay -= deltaTime;
    if (waveDelay <= 0) {
      isWaitingBetweenWaves = false;
      spawnNextWave(player);
    }
  }
  monsters.erase(
    std::remove_if(monsters.begin(), monsters.end(),
      [](const auto& monster) {
        return !monster->life;
      }),
    monsters.end()
  );
  for (auto& monster : monsters) {
    if (monster->life) {
      monster->update(deltaTime); // Теперь монстры будут "жить"
    }
  }
}

bool WaveManager::isWaiting() const {
  return isWaitingBetweenWaves;
}