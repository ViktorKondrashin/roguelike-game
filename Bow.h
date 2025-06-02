#pragma once
#include "Weapon.h"
#include "Arrow.h"
#include <vector>
#include <memory>

class Bow : public Weapon {
public:
  Bow(float attackSpeed, float damage, float arrowSpeed);
  void attack(const sf::Vector2f& startPos, const sf::Vector2f& targetPos) override;
  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) override;
  int getDamage() { return damage; }
  // ¬озвращает константную ссылку на стрелы
  const std::vector<std::shared_ptr<Arrow>>& getArrows() const { return arrows; }
  std::vector<std::shared_ptr<Arrow>> arrows;

private:
  float arrowSpeed;
};