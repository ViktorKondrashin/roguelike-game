#pragma once
#include <SFML/Graphics.hpp>

class PopupText {
public:
  PopupText(const std::string& message, sf::Vector2f position, const sf::Font& font);
  void update(float deltaTime);
  void draw(sf::RenderWindow& window) const;
  bool isDone() const;

private:
  sf::Text text;
  float lifetime = 1.0f;
  float currentTime = 0.0f;
};