#include "PopupText.h"

PopupText::PopupText(const std::string& message, sf::Vector2f position, const sf::Font& font) {
  text.setString(message);
  text.setFont(font);
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Green);
  text.setPosition(position);
}

void PopupText::update(float deltaTime) {
  currentTime += deltaTime;
  text.move(0, -50 * deltaTime);
  text.setFillColor(sf::Color(0, 255, 0, 255 * (1 - currentTime / lifetime)));
}

bool PopupText::isDone() const {
  return currentTime >= lifetime;
}