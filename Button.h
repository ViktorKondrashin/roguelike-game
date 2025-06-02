#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
  Button(const std::string& text, const sf::Font& font, sf::Vector2f size);

  void setPosition(sf::Vector2f position);
  void setColors(sf::Color textColor, sf::Color idleColor,
    sf::Color hoverColor, sf::Color activeColor);
  void setOnClick(std::function<void()> onClick);

  void update(const sf::Vector2f& mousePos);
  void draw(sf::RenderWindow& window) const;
  bool isPressed() const;

private:
  sf::RectangleShape shape;
  sf::Text text;

  sf::Color textColor;
  sf::Color idleColor;
  sf::Color hoverColor;
  sf::Color activeColor;

  std::function<void()> onClick = nullptr;
  bool pressed = false;

  void centerText();
};