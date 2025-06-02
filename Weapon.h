#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Entity;
class Monster; // Форвард-декларация

class Weapon {
public:
  float attackSpeed;
  int damage;
  float cooldown = 0.f;
  virtual ~Weapon() = default;
  virtual void attack(const sf::Vector2f& startPos, const sf::Vector2f& targetPos) = 0;
  virtual void update(float deltaTime) = 0;
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual void setOwner(Entity* owner) { this->owner = owner; }
  Entity* getOwner() const { return owner; }
protected:
  Entity* owner = nullptr;
};