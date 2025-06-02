#pragma once

#include <SFML/Graphics.hpp>
#include "LevelManager.h"
#include "Player.h"
#include "InteractionHint.h"
#include "StaticObstacle.h"
#include "Button.h"
#include <memory>

class Lobby {
public:
  // Шрифт передаётся из GameManager::initialize()
  void loadResources(const sf::Font& sharedFont);
  void update(float deltaTime, Player& player);
  void draw(sf::RenderWindow& window);

  bool isPlayerInShopZone(sf::Vector2f playerPos) const;
  bool isPlayerInExitZone(sf::Vector2f playerPos) const;
  const LevelManager& getLevelManager() const { return levelManager; }

  bool isExitToMenuPressed(const sf::Vector2f& mousePos) const;

private:
  static constexpr float SHOP_INTERACTION_RANGE = 50.f;
  static constexpr float EXIT_INTERACTION_RANGE = 30.f;

  InteractionHint interactionHint;
  LevelManager levelManager;
  std::shared_ptr<StaticObstacle> shopObject;
  sf::FloatRect exitZone;

  std::unique_ptr<Button> returnToMenuButton;
};