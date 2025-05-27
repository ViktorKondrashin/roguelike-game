#include "Entity.h"
#include <iostream>

void Entity::startDying() {
  isDying = true;
  canCollide = false; // Отключаем коллизии
}

void Entity::updateDeath(float deltaTime) {
  if (isDying) {
    deathTimer += deltaTime;
    if (deathTimer >= deathDuration) {
      life = false; // Удаляем объект
    }
  }
}

Entity::Entity(float _x, float _y, float _height, float _width, std::string way) :x(_x), y(_y), height(_height), width(_width), fileWay(way), speed(0),owner(this) {
  statusSystem = std::make_unique<StatusSystem>(this);
  image.loadFromFile(fileWay);
  texture.loadFromImage(image);
  sprite.setTextureRect(sf::IntRect(0, 0, height, width));
  if (!texture.loadFromFile(way)) {
    std::cerr << "ERROR: Failed to load texture: " << way << std::endl;
  }
  else {
    sprite.setTexture(texture);
  }
}

Entity::Entity(float _x, float _y, float _height, float _width, std::string way, Entity* owner) :x(_x), y(_y), height(_height), width(_width), fileWay(way), speed(0), owner(owner) {
  statusSystem = std::make_unique<StatusSystem>(this);
  image.loadFromFile(fileWay);
  texture.loadFromImage(image);
  sprite.setTextureRect(sf::IntRect(0, 0, height, width));
  if (!texture.loadFromFile(way)) {
    std::cerr << "ERROR: Failed to load texture: " << way << std::endl;
  }
  else {
    sprite.setTexture(texture);
  }
}

void Entity::draw(sf::RenderWindow& window) const {
  window.draw(sprite);
}

bool Entity::checkCollision(Entity* other) {
  if (!canCollide || !other->canCollide || !isSolid || !other->isSolid)
    return false;
  if (other == owner)
    return false;
  if (isDying || other->isDying) {
    return false; // Мёртвые не сталкиваются
  }

  if (AABBCollision(other)) {
    return usePixelPerfect ? pixelPerfectCollision(other) : true;
  }
  return false;
}

void Entity::onCollision(Entity* other) {

  float overlapLeft = (x + width) - other->x;
  float overlapRight = (other->x + other->width) - x;
  float overlapTop = (y + height) - other->y;
  float overlapBottom = (other->y + other->height) - y;

  float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

  if (minOverlap == overlapLeft) {
    x = other->x - width;
  }
  else if (minOverlap == overlapRight) {
    x = other->x + other->width;
  }
  else if (minOverlap == overlapTop) {
    y = other->y - height;
  }
  else if (minOverlap == overlapBottom) {
    y = other->y + other->height;
  }
}

void Entity::takeDamage(int damage)
{
  hp -= damage;
  if (hp <= 0) {
    hp = 0;
    startDying();
  }
}


void Entity::applyStatusEffect(StatusEffectType type, float duration, float intensity)
{
    statusSystem->applyEffect(type, sf::seconds(duration), intensity);
}

bool Entity::hasStatusEffect(StatusEffectType type) const
{
  return statusSystem->hasEffect(type);
}

// Проверка AABB (быстрая)
bool Entity::AABBCollision(Entity* other) {
    return (x < other->x + other->width) &&
           (x + width > other->x) &&
           (y < other->y + other->height) &&
           (y + height > other->y);
}

// Точная проверка (если нужно)
bool Entity::pixelPerfectCollision(Entity* other, sf::Uint8 alphaThreshold) {
    // Используем твою функцию из GameUtils
    return Collision::PixelPerfectTest(sprite, other->sprite, alphaThreshold);
}
