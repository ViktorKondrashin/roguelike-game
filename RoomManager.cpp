#include "RoomManager.h"
#include "Weapon.h"
#include <iostream>
#include <memory>

void RoomManager::init(Player* player, LevelManager* level) {
  this->player = player;
  this->level = level;
  initUI();
}

void RoomManager::loadRoom(const std::string& roomName) {
  waveManager.loadRoomWaves(roomName);
  waveManager.spawnNextWave(player);
  isActive = true;
}

void RoomManager::update(float deltaTime) {
  if (!isActive) return;

  waveManager.update(deltaTime);
  updateCollisions();

  if (waveManager.isWaveCleared() && !waveManager.areAllWavesDone()) {
    if (!waveManager.isWaiting()) {
      waveManager.startNextWave(player);
    }
  }
}

void RoomManager::draw(sf::RenderWindow& window)
{
    if (!isActive) return;

    // статические объекты
    for (auto& obstacle : staticObstacles)
        obstacle->draw(window);

    // все монстры (теперь у каждого вызовется свой draw)
    for (auto& monster : waveManager.monsters)
        monster->draw(window);
}

void RoomManager::drawUI(sf::RenderWindow& window, const sf::View& view) {
  if (!isActive) return;

  // Позиционируем текст относительно центра камеры
  sf::Vector2f viewCenter = view.getCenter();
  sf::Vector2f viewSize = view.getSize();

  // Отображение текущей волны (верхний левый угол)
  waveText.setString("Wave: " + std::to_string(waveManager.getCurrentWaveIndex()) +
    "/" + std::to_string(waveManager.getTotalWaves()));
  waveText.setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2);
  window.draw(waveText);

  // Таймер паузы между волнами (центр экрана)
  if (waveManager.isWaiting()) {
    pauseText.setString("Next wave in: " +
      std::to_string(static_cast<int>(waveManager.getRemainingDelay()) + 1));
    sf::FloatRect bounds = pauseText.getLocalBounds();
    pauseText.setPosition(viewCenter.x - bounds.width / 2, viewCenter.y - 100);
    window.draw(pauseText);
  }

  // Сообщение о завершении комнаты (центр экрана)
  if (isRoomCleared()) {
    sf::FloatRect bounds = roomClearedText.getLocalBounds();
    roomClearedText.setPosition(viewCenter.x - bounds.width / 2, viewCenter.y+50);
    window.draw(roomClearedText);
  }
}

bool RoomManager::isRoomCleared() const {
  return isActive &&
    waveManager.areAllWavesDone() &&
    waveManager.isWaveCleared() &&
    (waveManager.getCurrentWaveIndex() == waveManager.getTotalWaves());
}

int RoomManager::getCurrentWaveIndex() const {
  return waveManager.getCurrentWaveIndex();
}

int RoomManager::getTotalWaves() const {
  return waveManager.waves.size();
}

bool RoomManager::isWaiting() const {
  return waveManager.isWaiting();
}

float RoomManager::getRemainingDelay() const {
  return waveManager.getRemainingDelay();
}

void RoomManager::initUI() {
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load font for RoomManager UI!" << std::endl;
  }

  waveText.setFont(font);
  waveText.setCharacterSize(24);
  waveText.setFillColor(sf::Color::Yellow);

  pauseText.setFont(font);
  pauseText.setCharacterSize(32);
  pauseText.setFillColor(sf::Color(0xC8, 0x2A, 0x2A));
  pauseText.setString("Next wave in: 5");

  roomClearedText.setFont(font);
  roomClearedText.setCharacterSize(36);
  roomClearedText.setFillColor(sf::Color::Green);
  roomClearedText.setString("Room cleared!");
}

std::vector<std::shared_ptr<Entity>> RoomManager::getAllEntities() const {
  std::vector<std::shared_ptr<Entity>> entities;

  if (player) {
    entities.push_back(std::shared_ptr<Entity>(player, [](Entity*) {}));
  }

  for (const auto& monster : waveManager.monsters) {
    entities.push_back(std::static_pointer_cast<Entity>(monster));
  }

  if (player && player->getWeapon()) {
    if (auto bow = dynamic_cast<Bow*>(player->getWeapon())) {
      for (const auto& arrow : bow->getArrows()) {
        entities.push_back(std::static_pointer_cast<Entity>(arrow));
      }
    }
  }

  return entities;
}

void RoomManager::updateCollisions() {
  auto entities = getAllEntities();

  for (const auto& obstacle : staticObstacles) {
    entities.push_back(std::static_pointer_cast<Entity>(obstacle));
  }

  for (size_t i = 0; i < entities.size(); ++i) {
    for (size_t j = i + 1; j < entities.size(); ++j) {
      if (entities[i] && entities[j] && entities[i]->checkCollision(entities[j].get())) {
        entities[i]->onCollision(entities[j].get());
        entities[j]->onCollision(entities[i].get());
      }
    }
  }
}

void RoomManager::addStaticObstacle(std::shared_ptr<StaticObstacle> obstacle) {
  staticObstacles.push_back(obstacle);
}