#include "PlayerCamera.h"
#include <algorithm>
sf::View PlayerСamera::getViewForPlayer(float playerX, float playerY, const LevelManager& LevelManager, float screenWidth, float screenHeight) {
  sf::View view;
  float halfWidth = screenWidth / 2;
  float halfHeight = screenHeight / 2;

  // Границы карты 
  float mapWidth = LevelManager.getWidth() * 32;
  float mapHeight = LevelManager.getHeight() * 32;

  // Проверяем, чтобы границы clamp были валидны (low <= high)
  float minX = std::min(halfWidth, mapWidth - halfWidth);
  float maxX = std::max(halfWidth, mapWidth - halfWidth);

  float minY = std::min(halfHeight, mapHeight - halfHeight);
  float maxY = std::max(halfHeight, mapHeight - halfHeight);

  // Ограничиваем позицию камеры
  playerX = std::clamp(playerX, minX, maxX);
  playerY = std::clamp(playerY, minY, maxY);

  view.setCenter(playerX, playerY);
  view.setSize(screenWidth, screenHeight);
  return view;
}