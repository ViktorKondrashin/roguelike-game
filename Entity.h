#pragma once
#include "SFML/Graphics.hpp"
#include "StatusSystem.h"
#include "GameUtils.h"
#include <memory>

class Entity {
public:
  float x, y, height, width, dx = 0, dy = 0, speed;
  int dir = 0, hp = 100, maxHp;
  bool life = true;
  sf::String fileWay;
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;

  Entity* owner;
  bool isSolid = true;        // Может ли объект быть препятствием
  bool canCollide = true;     // Нужно ли вообще проверять коллизии
  bool usePixelPerfect = false; // Использовать ли точную проверку (если false — только AABB)

  bool isDying = false;
  float deathTimer = 0.0f;
  float deathDuration = 0.5f; // Время анимации смерти в секундах

  virtual void startDying();

  virtual void updateDeath(float deltaTime);

  std::unique_ptr<StatusSystem> statusSystem;

  Entity(float _x, float _y, float _height, float _width, std::string way);
  Entity(float _x, float _y, float _height, float _width, std::string way, Entity* owner);
  virtual ~Entity() = default;
  virtual void update(float time) = 0; 
  virtual void draw(sf::RenderWindow& window) const;

  const sf::Sprite& getSprite() const { return sprite; }
  virtual bool checkCollision(Entity* other);
  virtual void onCollision(Entity* other);
  bool AABBCollision(Entity* other);
  bool pixelPerfectCollision(Entity* other, sf::Uint8 alphaThreshold = 128);

  void takeDamage(int damage);

  void applyStatusEffect(StatusEffectType type, float duration, float intensity = 1.0f);
  bool hasStatusEffect(StatusEffectType type) const;

};