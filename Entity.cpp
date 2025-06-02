#include "Entity.h"
#include "LevelManager.h"
#include <iostream>
#include <array>
#include <cmath>

void Entity::startDying() {
  isDying = true;
  canCollide = false;
}           

void Entity::updateDeath(float deltaTime) {
  if (isDying) {
    deathTimer += deltaTime;
    if (deathTimer >= deathDuration) {
      life = false;
    }
  }
}

Entity::Entity(float _x, float _y, float _height, float _width, std::string way, LevelManager* lvlMgr)
  : x(_x), y(_y), height(_height), width(_width), fileWay(way), speed(0), owner(this), levelManager(lvlMgr) {
  statusSystem = std::make_unique<StatusSystem>(this);
  if (!texture.loadFromFile(way)) {
    std::cerr << "ERROR: Failed to load texture: " << way << std::endl;
  }
  else {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    sprite.setPosition(_x, _y);
  }
}

Entity::Entity(float _x, float _y, float _height, float _width, std::string way, Entity* owner, LevelManager* lvlMgr)
  : x(_x), y(_y), height(_height), width(_width), fileWay(way), speed(0), owner(owner), levelManager(lvlMgr) {
  statusSystem = std::make_unique<StatusSystem>(this);
  if (!texture.loadFromFile(way)) {
    std::cerr << "ERROR: Failed to load texture: " << way << std::endl;
  }
  else {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
  }
}

void Entity::draw(sf::RenderWindow& window) const {
  window.draw(sprite);
}

bool Entity::checkCollision(Entity* other) {
  if (!canCollide || !other->canCollide || !isSolid || !other->isSolid) return false;
  if (other == owner) return false;
  if (isDying || other->isDying) return false;

  return AABBCollision(other) && (!usePixelPerfect || pixelPerfectCollision(other));
}

void Entity::onCollision(Entity* other) {
  if (!other) return;

  const float pushForce = 0.5f;
  const float minDistance = 0.1f;

  sf::FloatRect thisBounds = getGlobalBounds();
  sf::FloatRect otherBounds = other->getGlobalBounds();

  sf::Vector2f direction(
    other->x - this->x,
    other->y - this->y
  );

  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > minDistance) {
    direction.x /= length;
    direction.y /= length;
  }
  else {
    direction = sf::Vector2f(1.f, 0.f);
  }

  float overlapX = std::min(
    thisBounds.left + thisBounds.width - otherBounds.left,
    otherBounds.left + otherBounds.width - thisBounds.left
  );
  float overlapY = std::min(
    thisBounds.top + thisBounds.height - otherBounds.top,
    otherBounds.top + otherBounds.height - thisBounds.top
  );

  if (overlapX < overlapY) {
    float push = overlapX * pushForce;
    this->x -= direction.x * push;
    other->x += direction.x * push;
  }
  else {
    float push = overlapY * pushForce;
    this->y -= direction.y * push;
    other->y += direction.y * push;
  }

  this->sprite.setPosition(this->x, this->y);
  other->sprite.setPosition(other->x, other->y);
}

void Entity::interactWithMap()
{
    if (!isSolid || !canCollide || !levelManager) return;

    sf::FloatRect bounds = getGlobalBounds();

    bool changed = false;
    if (bounds.left < 0.f) { bounds.left = 0.f;          dx = 0; changed = true; }
    if (bounds.top < 0.f) { bounds.top = 0.f;          dy = 0; changed = true; }

    const std::array<sf::Vector2f, 4> probes = {
        sf::Vector2f{bounds.left,                    bounds.top + bounds.height * 0.5f},
        sf::Vector2f{bounds.left + bounds.width,     bounds.top + bounds.height * 0.5f},
        sf::Vector2f{bounds.left + bounds.width * 0.5f,bounds.top},
        sf::Vector2f{bounds.left + bounds.width * 0.5f,bounds.top + bounds.height}
    };

    for (auto p : probes)
    {
        int tx = static_cast<int>(std::floor(p.x / 32.f));
        int ty = static_cast<int>(std::floor(p.y / 32.f));
        if (levelManager->isWalkable(tx, ty)) continue;

        sf::FloatRect tile(tx * 32.f, ty * 32.f, 32.f, 32.f);
        sf::FloatRect inter;
        if (!bounds.intersects(tile, inter)) continue;

        if (inter.width < inter.height) {
            float push = inter.width - WALL_PENETRATION;
            if (push > 0.f) {
                bounds.left += (bounds.left < tile.left ? -push : push);
                dx = 0;
                changed = true;
            }
        }
        else {
            float push = inter.height - WALL_PENETRATION;
            if (push > 0.f) {
                bounds.top += (bounds.top < tile.top ? -push : push);
                dy = 0;
                changed = true;
            }
        }
    }

    if (changed) {
        x = bounds.left + bounds.width * 0.5f;
        y = bounds.top + bounds.height * 0.5f;
        sprite.setPosition(x, y);
    }
}



sf::FloatRect Entity::getGlobalBounds() const {
  return sf::FloatRect(x - width / 2, y - height / 2, width, height);
}

bool Entity::AABBCollision(Entity* other) {
  sf::FloatRect thisBounds = getGlobalBounds();
  sf::FloatRect otherBounds = other->getGlobalBounds();
  return thisBounds.intersects(otherBounds);
}

void Entity::takeDamage(int damage) {
  hp -= damage;
  if (hp <= 0) {
    hp = 0;
    startDying();
  }
}

bool Entity::pixelPerfectCollision(Entity* other, sf::Uint8 alphaThreshold) {
  if (!other || !other->getSprite().getTexture() || !sprite.getTexture()) {
    return false;
  }

  return Collision::PixelPerfectTest(sprite, other->sprite, alphaThreshold);
}

bool Entity::hasStatusEffect(StatusEffectType type) const {
  if (!statusSystem) {
    return false;
  }
  return statusSystem->hasEffect(type);
} 