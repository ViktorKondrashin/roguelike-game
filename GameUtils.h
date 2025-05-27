#pragma once
#include <SFML/Graphics.hpp>
#include <vector>


namespace Collision {

  inline bool BoundingBoxTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
  }
  bool PixelPerfectTest(const sf::Sprite& sprite1, const sf::Sprite& sprite2,
    sf::Uint8 alphaThreshold = 128);
}