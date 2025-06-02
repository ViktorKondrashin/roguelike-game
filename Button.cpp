#include "Button.h"

Button::Button(const std::string& text, const sf::Font& font, sf::Vector2f size)
  : text(text, font, 24) {
  shape.setSize(size);

  // Default colors
  textColor = sf::Color::White;
  idleColor = sf::Color(70, 70, 70);
  hoverColor = sf::Color(150, 150, 150);
  activeColor = sf::Color(100, 100, 100);

  shape.setFillColor(idleColor);
  this->text.setFillColor(textColor);
  centerText();
}

void Button::setPosition(sf::Vector2f position) {
  shape.setPosition(position);
  centerText();
}

void Button::setColors(sf::Color textColor, sf::Color idleColor,
  sf::Color hoverColor, sf::Color activeColor) {
  this->textColor = textColor;
  this->idleColor = idleColor;
  this->hoverColor = hoverColor;
  this->activeColor = activeColor;

  text.setFillColor(textColor);
  shape.setFillColor(idleColor);
}

void Button::setOnClick(std::function<void()> onClick) {
  this->onClick = onClick;
}

void Button::update(const sf::Vector2f& mousePos) {
  bool mouseOver = shape.getGlobalBounds().contains(mousePos);

  if (mouseOver) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      shape.setFillColor(activeColor);
      pressed = true;
    }
    else {
      if (pressed && onClick) {
        onClick();
      }
      shape.setFillColor(hoverColor);
      pressed = false;
    }
  }
  else {
    shape.setFillColor(idleColor);
    pressed = false;
  }
}

void Button::draw(sf::RenderWindow& window) const {
  window.draw(shape);
  window.draw(text);
}

bool Button::isPressed() const {
  return pressed;
}

void Button::centerText() {
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width / 2.0f,
    textRect.top + textRect.height / 2.0f);
  text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
    shape.getPosition().y + shape.getSize().y / 2.0f);
}