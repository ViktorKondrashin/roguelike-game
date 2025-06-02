#include "PlayerCamera.h"
#include <algorithm>
sf::View PlayerCamera::getViewForPlayer(float playerX, float playerY, const LevelManager& LevelManager, float screenWidth, float screenHeight) {
  sf::View view;
  float halfWidth = screenWidth / 2;
  float halfHeight = screenHeight / 2;

  float mapWidth = LevelManager.getWidth() * 32;
  float mapHeight = LevelManager.getHeight() * 32;

  float minX = std::min(halfWidth, mapWidth - halfWidth);
  float maxX = std::max(halfWidth, mapWidth - halfWidth);

  float minY = std::min(halfHeight, mapHeight - halfHeight);
  float maxY = std::max(halfHeight, mapHeight - halfHeight);

  playerX = std::clamp(playerX, minX, maxX);
  playerY = std::clamp(playerY, minY, maxY);

  view.setCenter(playerX, playerY);
  view.setSize(screenWidth, screenHeight);
  return view;
}