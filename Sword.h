#pragma once
#include "Weapon.h"

class Sword : public Weapon {
public:
  Sword(float range, float damage);
  void attack(const sf::Vector2f& startPos, const sf::Vector2f& targetPos) override;
  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) override;

private:
  float range;
  float damage;
  bool isAttacking = false;
  sf::Clock attackTimer;
  sf::RectangleShape hitbox;
};