#pragma once
#include "Entity.h"
#include <memory>

class Player;

class Monster : public Entity {
public:
  Monster(float x, float y, float w, float h, std::string texturePath, Player* player);
  virtual ~Monster() = default;
  void setTargetPosition(const sf::Vector2f& target);
  Player* target;
  sf::Vector2f targetPosition;
  virtual void update(float time) = 0;
  int damage;
  int reward;
  float attackCooldown;
  float currentCooldown = 0;
};