#include "Monster.h"
#include "Player.h" 

Monster::Monster(float x, float y, float w, float h, std::string texturePath, Player* player): Entity(x, y, w, h, texturePath), damage(0), reward(0), attackCooldown(0), currentCooldown(0), target(player) {
  setTargetPosition(player->getPosition());
}

void Monster::setTargetPosition(const sf::Vector2f& target)
{
  targetPosition = target;
}
