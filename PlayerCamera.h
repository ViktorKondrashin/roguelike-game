#pragma once
#include "SFML/Graphics.hpp"
#include "LevelManager.h"

class Player—amera {
public:
  static sf::View getViewForPlayer(float playerX, float playerY,
    const LevelManager& LevelManager,
    float screenWidth,
    float screenHeight);
};