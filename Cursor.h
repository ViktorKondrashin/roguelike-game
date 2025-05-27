#pragma once
#include "SFML/Graphics.hpp"

class Cursor {
private:
  sf::Image cursorImage;
  sf::Texture cursorTexture;
  sf::Sprite cursorSprite;

public:
  Cursor(sf::String way);
  void update(const sf::RenderWindow& window);
};