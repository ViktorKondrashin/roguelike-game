#include "Arrow.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "player.h"

Arrow::Arrow(const sf::Vector2f& position, const sf::Vector2f& target, float speed, float damage, Entity* shooter, LevelManager* lvlMgr): damage(damage), Entity(position.x, position.y, 32, 32, "image/arrow.png", shooter, lvlMgr)
{
  sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
  sprite.setPosition(position);
  startPosition = position;

  usePixelPerfect = true;
  sf::Vector2f direction = target - position;
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length != 0) {
    direction /= length;
  }
  canCollide = false;
  spawnTimer = 0.1f;
  
  velocity = direction * speed;

  float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
  angle += 90.f;

  if (angle < 0) angle += 360.f;
  if (angle >= 360.f) angle -= 360.f;

  sprite.setRotation(angle);
}


void Arrow::update(float time)
{
    if (!life) return;
    if (spawnTimer > 0) {
        spawnTimer -= time;
        if (spawnTimer <= 0) {
            canCollide = true;
        }
    }

    if (canCollide && dynamic_cast<Player*>(owner)) {
        for (int i = y / 32; i <= (y + height - 1) / 32; i++) {
            for (int j = x / 32; j <= (x + width - 1) / 32; j++) {
                if (!levelManager->isShootable(j, i)) {
                    explode();
                    return;
                }
            }
        }
    }

    sprite.move(velocity * time);
    x = sprite.getPosition().x;
    y = sprite.getPosition().y;

    {
        float dx = x - startPosition.x;
        float dy = y - startPosition.y;
        if (dx * dx + dy * dy >= maxDistance * maxDistance) {
            explode();
            return;
        }
    }
}

bool Arrow::isOutOfScreen(const sf::RenderWindow& window) const
{
  const sf::Vector2f& pos = sprite.getPosition();
  const sf::Vector2u& windowSize = window.getSize();
  return pos.x < 0 || pos.y < 0 ||
    pos.x > windowSize.x || pos.y > windowSize.y;
}

void Arrow::draw(sf::RenderWindow& window) const
{
  if (!life) {
    return;
  }
  window.draw(sprite);
}

void Arrow::explode() {
    life = false;
}

void Arrow::onCollision(Entity* other) {
  if (!life || !other || other == owner) return;
  other->takeDamage(damage);
  explode();
}

bool Arrow::checkCollision(Entity* other)
{
  if (AABBCollision(other)) {
    return usePixelPerfect ? pixelPerfectCollision(other) : true;
  }
  return false;
}

