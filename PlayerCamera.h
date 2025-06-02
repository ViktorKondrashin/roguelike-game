#pragma once
#include "SFML/Graphics.hpp"
#include "LevelManager.h"

class PlayerCamera {
public:
  static sf::View getViewForPlayer(float playerX, float playerY,
    const LevelManager& LevelManager,
    float screenWidth,
    float screenHeight);
};