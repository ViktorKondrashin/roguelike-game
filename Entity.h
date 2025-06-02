#pragma once
#include "SFML/Graphics.hpp"
#include "StatusSystem.h"
#include "GameUtils.h"
#include <memory>

class LevelManager;

class Entity {
public:
  float x, y, height, width, dx = 0, dy = 0, speed = 200;
  int dir = 0, hp = 100, maxHP;
  bool life = true;
  sf::String fileWay;
  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;
  LevelManager* levelManager;

  Entity* owner;
  bool isSolid = true;
  bool canCollide = true;
  bool usePixelPerfect = true;
  static constexpr float WALL_PENETRATION = 8.0f;

  bool isDying = false;
  float deathTimer = 0.0f;
  float deathDuration = 0.5f;

  virtual void startDying();
  virtual void updateDeath(float deltaTime);

  std::unique_ptr<StatusSystem> statusSystem;

  Entity(float _x, float _y, float _height, float _width, std::string way, LevelManager* lvlMgr);
  Entity(float _x, float _y, float _height, float _width, std::string way, Entity* owner, LevelManager* lvlMgr);
  virtual ~Entity() = default;
  virtual void update(float time) = 0;
  virtual void draw(sf::RenderWindow& window) const;

  const sf::Sprite& getSprite() const { return sprite; }
  virtual bool checkCollision(Entity* other);
  virtual void onCollision(Entity* other);
  bool AABBCollision(Entity* other);
  bool pixelPerfectCollision(Entity* other, sf::Uint8 alphaThreshold = 128);
  virtual void interactWithMap();
  void resolveCollision(sf::FloatRect& entityBounds, const sf::FloatRect& tileBounds);

  virtual void takeDamage(int damage);
  void applyStatusEffect(StatusEffectType type, float duration, float intensity = 1.0f);
  bool hasStatusEffect(StatusEffectType type) const;

  virtual sf::FloatRect getGlobalBounds() const;
  sf::Vector2f getPosition() const { return { x, y }; }
};