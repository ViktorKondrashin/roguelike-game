#include "Entity.h"
#include "LevelManager.h"
#include <iostream>

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

  const float pushForce = 0.5f; // Коэффициент отталкивания (0-1)
  const float minDistance = 0.1f; // Минимальная дистанция для коррекции

  sf::FloatRect thisBounds = getGlobalBounds();
  sf::FloatRect otherBounds = other->getGlobalBounds();

  // Вектор от текущей сущности к другой
  sf::Vector2f direction(
    other->x - this->x,
    other->y - this->y
  );

  // Нормализуем вектор (если не нулевой)
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > minDistance) {
    direction.x /= length;
    direction.y /= length;
  }
  else {
    // Если сущности слишком близко - случайное направление
    direction = sf::Vector2f(1.f, 0.f);
  }

  // Рассчитываем перекрытие
  float overlapX = std::min(
    thisBounds.left + thisBounds.width - otherBounds.left,
    otherBounds.left + otherBounds.width - thisBounds.left
  );
  float overlapY = std::min(
    thisBounds.top + thisBounds.height - otherBounds.top,
    otherBounds.top + otherBounds.height - thisBounds.top
  );

  // Применяем "мягкое" отталкивание
  if (overlapX < overlapY) {
    // Отталкивание по X
    float push = overlapX * pushForce;
    this->x -= direction.x * push;
    other->x += direction.x * push;
  }
  else {
    // Отталкивание по Y
    float push = overlapY * pushForce;
    this->y -= direction.y * push;
    other->y += direction.y * push;
  }

  // Обновляем позиции спрайтов
  this->sprite.setPosition(this->x, this->y);
  other->sprite.setPosition(other->x, other->y);
}

void Entity::interactWithMap() {
  if (!isSolid || !canCollide || !levelManager) return;

  sf::FloatRect entityBounds = getGlobalBounds();
  bool collided = false;

  // Проверяем 4 стороны объекта
  std::vector<sf::Vector2f> checkPoints = {
      {entityBounds.left, entityBounds.top + entityBounds.height / 2},          // лево
      {entityBounds.left + entityBounds.width, entityBounds.top + entityBounds.height / 2}, // право
      {entityBounds.left + entityBounds.width / 2, entityBounds.top},          // верх
      {entityBounds.left + entityBounds.width / 2, entityBounds.top + entityBounds.height}  // низ
  };

  for (const auto& point : checkPoints) {
    int tileX = static_cast<int>(point.x / 32);
    int tileY = static_cast<int>(point.y / 32);

    if (!levelManager->isWalkable(tileX, tileY)) {
      sf::FloatRect tileBounds(tileX * 32.f, tileY * 32.f, 32.f, 32.f);

      // Рассчитываем перекрытие
      float overlapX = std::min(
        entityBounds.left + entityBounds.width - tileBounds.left,
        tileBounds.left + tileBounds.width - entityBounds.left
      );
      float overlapY = std::min(
        entityBounds.top + entityBounds.height - tileBounds.top,
        tileBounds.top + tileBounds.height - entityBounds.top
      );

      if (overlapX > WALL_PENETRATION && overlapY > WALL_PENETRATION) {
        if (overlapX < overlapY) {
          // Коллизия по X
          if (entityBounds.left < tileBounds.left) {
            entityBounds.left = tileBounds.left - entityBounds.width + WALL_PENETRATION;
          }
          else {
            entityBounds.left = tileBounds.left + tileBounds.width - WALL_PENETRATION;
          }
        }
        else {
          // Коллизия по Y
          if (entityBounds.top < tileBounds.top) {
            entityBounds.top = tileBounds.top - entityBounds.height + WALL_PENETRATION;
          }
          else {
            entityBounds.top = tileBounds.top + tileBounds.height - WALL_PENETRATION;
          }
        }
        collided = true;
      }
    }
  }

  if (collided) {
    x = entityBounds.left + entityBounds.width / 2;
    y = entityBounds.top + entityBounds.height / 2;
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

  // Используем функцию из GameUtils (убедитесь, что она подключена)
  return Collision::PixelPerfectTest(sprite, other->sprite, alphaThreshold);
}

bool Entity::hasStatusEffect(StatusEffectType type) const {
  if (!statusSystem) {
    return false;
  }
  return statusSystem->hasEffect(type);
} 