#include "StaticObstacle.h"
#include <iostream>
StaticObstacle::StaticObstacle(float x, float y, float width, float height,
  const std::string& texturePath, LevelManager* levelManager)
  : Entity(x, y, height, width, texturePath, levelManager) {
  isSolid = true;
  canCollide = true;
  life = true;
  hp = INT_MAX;
  maxHP = INT_MAX;

}

void StaticObstacle::update(float deltaTime) {
  // Можно добавить анимацию или другие эффекты
}

void StaticObstacle::draw(sf::RenderWindow& window) const {
  Entity::draw(window);

}

void StaticObstacle::takeDamage(int damage) {
  // Игнорируем урон
}

void StaticObstacle::startDying() {
  // Не позволяем объекту умирать
}

void StaticObstacle::setInteractionCallback(InteractionCallback callback) {
  interactionCallback = callback;
}

void StaticObstacle::checkPlayerInteraction(const sf::Vector2f& playerPosition) {
  if (!isInteractable) return;

  sf::FloatRect bounds = getGlobalBounds();

  float interactionMargin = 20.f;
  bounds.left -= interactionMargin;
  bounds.top -= interactionMargin;
  bounds.width += interactionMargin * 2;
  bounds.height += interactionMargin * 2;

  if (bounds.contains(playerPosition) && interactionCallback) {
    interactionCallback();
  }
}

void StaticObstacle::setInteractable(bool interactable) {
  isInteractable = interactable;
}

sf::FloatRect StaticObstacle::getInteractionBounds() const {
  sf::FloatRect bounds = getGlobalBounds();
  float margin = 20.f;
  bounds.left -= margin;
  bounds.top -= margin;
  bounds.width += margin * 2;
  bounds.height += margin * 2;
  return bounds;
}

sf::FloatRect StaticObstacle::getGlobalBounds() const {
  sf::Vector2f pos = sprite.getPosition();

  return sf::FloatRect(pos.x, pos.y, customWidth, customHeight);
}

void StaticObstacle::setCustomBounds(float width, float height) {
  customWidth = width;
  customHeight = height;
}