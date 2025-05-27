#include "WaveManager.h"
#include "Player.h"
#include "LevelManager.h"
#include "Entity.h"
#include "Bow.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class RoomManager {
public:
  void init(Player* player, LevelManager* level);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
  void drawUI(sf::RenderWindow& window, const sf::View& view);
  void loadRoom(const std::string& roomName);
  bool isRoomCleared() const;
  int getCurrentWaveIndex() const;
  int getTotalWaves() const;
  bool isWaiting() const;
  float getRemainingDelay() const;
  const std::vector<std::shared_ptr<Monster>>& getMonsters() const { return waveManager.monsters; }

private:
  Player* player = nullptr;
  LevelManager* level = nullptr;
  WaveManager waveManager;
  bool isActive = false;

  sf::Text waveText;
  sf::Font font;
  sf::Text pauseText;
  sf::Text roomClearedText;

  void initUI();
  std::vector<std::shared_ptr<Entity>> getAllEntities() const;
  void updateCollisions();
};