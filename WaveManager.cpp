#include "WaveManager.h"
#include "MonsterFactory.h"
#include <iostream>
#include "Player.h"

void WaveManager::loadRoomWaves(const std::string& roomBaseName) {
  waves.clear();
  currentWaveIndex = 0;
  std::cout << "Trying to load waves for room: " << roomBaseName << std::endl; // Лог

  int waveNum = 1;
  while (true) {
    std::string path = "data/" + roomBaseName + "_" + std::to_string(waveNum) + ".waves";
    std::cout << "Checking file: " << path << std::endl; // Лог
    auto loadedWave = WaveLoader::loadFromFile(path);
    if (loadedWave.empty()) break;

    std::vector<SpawnPoint> wave;
    for (const auto& spawn : loadedWave) {
      wave.push_back({ spawn.x, spawn.y, spawn.type });
    }
    waves.push_back(wave);
    waveNum++;
  }
  std::cout << "Total waves loaded: " << waves.size() << std::endl; // Лог
}

void WaveManager::spawnNextWave(Player* player) {
  if (currentWaveIndex >= waves.size()) {
    std::cout << "No more waves to spawn!" << std::endl; // Лог
    return;
  }

  monsters.clear();
  for (const auto& spawn : waves[currentWaveIndex]) {
    std::string texturePath = "image/" + spawn.type + ".png";
    std::cout << "Spawning " << spawn.type << " at ("
      << spawn.x * 32 << "," << spawn.y * 32
      << ") with texture: " << texturePath << std::endl; // Лог

    std::string typeUpper = spawn.type;
    std::transform(typeUpper.begin(), typeUpper.end(), typeUpper.begin(), ::toupper);

    auto monster = MonsterFactory::create(
      typeUpper,
      spawn.x * 32.0f,
      spawn.y * 32.0f,
      32.0f, 32.0f,
      "image/" + spawn.type + ".png",
      player, player->levelManager
    );

    if (monster) {
      monsters.push_back(std::move(monster));
    }
    else {
      std::cerr << "Failed to spawn " << spawn.type << std::endl;
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