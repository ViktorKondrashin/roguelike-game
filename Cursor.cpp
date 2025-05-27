#include "Cursor.h"

Cursor::Cursor(sf::String way)
{
  cursorImage.loadFromFile(way);
  cursorTexture.loadFromImage(cursorImage);
  cursorSprite.setTexture(cursorTexture);
}

void Cursor::update(const sf::RenderWindow& window)
{
  cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
}

