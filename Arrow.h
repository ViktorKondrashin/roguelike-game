#pragma once
#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "GameUtils.h"
#include <vector>

class Arrow : public Entity {
private:
  sf::Vector2f velocity;
  float damage;
  float spawnTimer;

public:
  Arrow(const sf::Vector2f& position, const sf::Vector2f& target, float speed, float damage, Entity* shooter);
  virtual void update(float time) override;
  bool isOutOfScreen(const sf::RenderWindow& window) const;
  void draw(sf::RenderWindow& window) const;
  void explode();
  virtual void onCollision(Entity* other) override;
  virtual bool checkCollision(Entity* other) override;
};