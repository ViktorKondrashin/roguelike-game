#include "Sword.h"

Sword::Sword(float range, float damage)
  : range(range), damage(damage) {
  hitbox.setFillColor(sf::Color(255, 0, 0, 100)); 
}

void Sword::attack(const sf::Vector2f& startPos, const sf::Vector2f& targetPos) {
  if (!isAttacking) {
    isAttacking = true;
    attackTimer.restart();

    sf::Vector2f direction = targetPos - startPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) direction /= length;

    hitbox.setPosition(startPos);
    hitbox.setSize(sf::Vector2f(range, 10.f));
    hitbox.setRotation(std::atan2(direction.y, direction.x) * 180.f / 3.14159265f);
  }
}

void Sword::update(float deltaTime) {
  if (isAttacking && attackTimer.getElapsedTime().asSeconds() > 0.3f) {
    isAttacking = false;
  }
}

void Sword::draw(sf::RenderWindow& window) {
  if (isAttacking) {
    window.draw(hitbox);
  }
}

