#include "Slime.h"
#include "Player.h" 
#include <iostream>

Slime::Slime(float x, float y, float w, float h, std::string texturePath, Player* player, LevelManager* lvlMgr) :
  Monster(x, y, w, h, texturePath, player, lvlMgr)
{
  damage = 10;
  reward = 5;
  attackCooldown = 1.0f;
  speed = 0.05f;
  hp = 30;

  
  frames = {
        sf::IntRect(0, 0, 23, 20), 
        sf::IntRect(25, 0, 21, 20),

        sf::IntRect(0, 23, 23, 15),
        sf::IntRect(24, 21, 23, 17),

        sf::IntRect(0, 39, 24, 19), 
        sf::IntRect(27, 40, 23, 19),

        sf::IntRect(0, 59, 23, 20),
        sf::IntRect(26, 60, 22, 19), 
      
        sf::IntRect(0, 81, 23, 20),
        sf::IntRect(26, 81, 21, 20),
      
        sf::IntRect(23, 23, -23, 15),
        sf::IntRect(47, 21, -23, 17),
      
        sf::IntRect(24, 39, -24, 19),
        sf::IntRect(50, 40, -23, 19),
      
        sf::IntRect(23, 59, -23, 20),
        sf::IntRect(48, 60, -22, 19),

        sf::IntRect(0, 102, 23, 18),
        sf::IntRect(25, 102, 37, 57),
        sf::IntRect(63, 102, 37, 61),
        sf::IntRect(103, 102, 33, 57),
        sf::IntRect(137, 101, 23, 18), 
    
        sf::IntRect(0, 162, 23, 21),
        sf::IntRect(26, 163, 42, 26),
        sf::IntRect(68, 164, 41, 41),
        sf::IntRect(112, 164, 23, 41),
        sf::IntRect(0, 23, 23, 17),
     
        sf::IntRect(0, 207, 25, 19),
        sf::IntRect(29, 207, 40, 19),
        sf::IntRect(70, 207, 59, 19),
        sf::IntRect(130, 207, 36, 23),
        sf::IntRect(169, 207, 25, 19),
     
        sf::IntRect(0, 232, 23, 21),
        sf::IntRect(26, 231, 43, 26),
        sf::IntRect(69, 231, 41, 41),
        sf::IntRect(112, 231, 24, 41),
        sf::IntRect(138, 23, 23, 20),
   
        sf::IntRect(0, 272, 22, 20),
        sf::IntRect(23, 272, 37, 57),
        sf::IntRect(65, 272, 37, 61),
        sf::IntRect(103, 272, 32, 57),
        sf::IntRect(138, 271, 22, 20),
    
        sf::IntRect(23, 162, -23, 21),
        sf::IntRect(68, 163, -42, 26),
        sf::IntRect(109, 164, -41, 41),
        sf::IntRect(135, 164, -23, 41),
        sf::IntRect(137, 162, -23, 17),
      
        sf::IntRect(25, 207, -25, 19),
        sf::IntRect(69, 207, -40, 19),
        sf::IntRect(129, 207, -59, 19),
        sf::IntRect(166, 207, -36, 23),
        sf::IntRect(194, 207, 25, -19),
     
        sf::IntRect(23, 232, -23, 21),
        sf::IntRect(68, 231, -45, 26),
        sf::IntRect(110, 231, -41, 41),
        sf::IntRect(136, 231, -24, 41),
        sf::IntRect(161, 231, -23, 20),
  };
  attackOrigins = {
        sf::Vector2f{7,8},
        sf::Vector2f{12,5},
        sf::Vector2f{9,5},
        sf::Vector2f{10,4},
        sf::Vector2f{7,6},
      
        sf::Vector2f{3,13},
        sf::Vector2f{23,9},
        sf::Vector2f{24,8},
        sf::Vector2f{4,8},
        sf::Vector2f{3,9},
     
        sf::Vector2f{5,7},
        sf::Vector2f{21,7},
        sf::Vector2f{40,6},
        sf::Vector2f{17,7},
        sf::Vector2f{5,7},
     
        sf::Vector2f{7,7},
        sf::Vector2f{33,18},
        sf::Vector2f{31,34},
        sf::Vector2f{11,34},
        sf::Vector2f{11,13},
      
        sf::Vector2f{12,8},
        sf::Vector2f{22,45},
        sf::Vector2f{15,49},
        sf::Vector2f{12,45},
        sf::Vector2f{12,8},
      
        sf::Vector2f{12,13},
        sf::Vector2f{12,9},
        sf::Vector2f{12,8},
        sf::Vector2f{12,8},
        sf::Vector2f{12,9},
      
        sf::Vector2f{12,7},
        sf::Vector2f{12,7},
        sf::Vector2f{12,6},
        sf::Vector2f{12,7},
        sf::Vector2f{12,7},
      
        sf::Vector2f{12,7},
        sf::Vector2f{12,13},
        sf::Vector2f{12,29},
        sf::Vector2f{12,29},
        sf::Vector2f{12,8},
  };
}

void Slime::startDying() {
    if (isDying) return;
    isDying = true;
    canCollide = false;

    deathSprite = sprite;
    deathSprite.setOrigin(width / 2.0f, height / 2.0f);
    deathSprite.setPosition(x + width / 2.0f, y + height / 2.0f);
}



void Slime::update(float time) {
  if (!life) return;

  prevX = x;  prevY = y;

  if (isDying) {
    deathTimer += time;
    if (deathTimer >= deathDuration) {
      life = false;
      target->addMoney(reward);
    }
    return;
  }
  bool chasing = updateAI(time);

  if (!chasing) {
      dx = x - prevX;
      dy = y - prevY;

      frameTime += time;
      if (frameTime >= frameDuration) {
          frameTime = 0.f;
          currentFrame = 1.f - currentFrame;
      }
      int row = getAnimationRow();
      sprite.setTextureRect(frames[row * 2 + static_cast<int>(currentFrame)]);
      sprite.setOrigin(12, 10);

      interactWithMap();
      sprite.setPosition(x, y);
      return;
  }

  setTargetPosition(target->getPosition());
  if (currentCooldown > 0) {
    currentCooldown -= time;
  }

  updateMovement(time);
  updateAttack(time);
  interactWithMap();
  sprite.setPosition(x, y);
}

void Slime::draw(sf::RenderWindow& window) const {
    if (!life) return;

    if (isDying) {
        sf::Sprite tempSprite = deathSprite;
        float alpha = 1.0f - (deathTimer / deathDuration);
        tempSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
        window.draw(tempSprite);
    }
    else {
        Entity::draw(window);
    }
}

void Slime::updateMovement(float time) {
  if (!life || isAttacking) return;

  float distanceToPlayer = std::hypot(targetPosition.x - x, targetPosition.y - y);
  if (distanceToPlayer >= attackRange) {
    dx = (targetPosition.x - x) / distanceToPlayer;
    dy = (targetPosition.y - y) / distanceToPlayer;
    x += dx * speed * time * 1000;
    y += dy * speed * time * 1000;
  }

  frameTime += time;
  if (frameTime >= frameDuration) {
    frameTime = 0; currentFrame += 1;
    if (currentFrame >= 2) {
      currentFrame = 0;
    }
  }

  int row = getAnimationRow();
  sprite.setTextureRect(frames[row * 2 + static_cast<int>(currentFrame)]);
  sprite.setOrigin(12, 10);
}

void Slime::updateAttack(float time) {
  if (!life) return;

  float distanceToPlayer = std::hypot(targetPosition.x - x, targetPosition.y - y);

  if (!isAttacking && distanceToPlayer <= attackRange && currentCooldown <= 0) {
    isAttacking = true;
    currentAttackStage = 0;
    currentAttackStageTime = attackWindup;
  }

  if (isAttacking) {
    currentAttackStageTime -= time;

    if (currentAttackStageTime <= 0) {
      currentAttackStage++;

      switch (currentAttackStage) {
      case 1:
        currentAttackStageTime = attackStrike;
        if (Collision::PixelPerfectTest(sprite, target->getSprite())) {
          target->takeDamage(damage);
        }
        break;

      case 2:
        currentAttackStageTime = attackRecovery;
        break;

      case 3:
        isAttacking = false;
        currentCooldown = attackCooldown;
        break;
      }
    }

    int attackFrame = getAttackFrame();
    sprite.setTextureRect(frames[16 + getAnimationRow() * 5 + attackFrame]);
    sprite.setOrigin(attackOrigins[getAnimationRow() * 5 + attackFrame]);
  }
}

int Slime::getAttackFrame() const {
  float progress = 0.f;

  switch (currentAttackStage) {
  case 0:
    progress = 1.f - (currentAttackStageTime / attackWindup);
    return static_cast<int>(progress * 2);
  case 1:
    return 2;
  case 2:
    progress = 1.f - (currentAttackStageTime / attackRecovery);
    return 3 + static_cast<int>(progress * 1);
  }

  return 0;
}

void Slime::setTarget(Player* player)
{
  target = player;
}


int Slime::getAnimationRow() const {
  if (dx == 0 && dy == 0) return 0;

  float angle = std::atan2(dy, dx) * 180 / 3.14159265f;
  if (angle < 0) angle += 360.f;

  if (angle >= 112.5f && angle < 157.5f)  return 1;
  else if (angle >= 67.5f && angle < 112.5f) return 0;
  else if (angle >= 157.5f && angle < 202.5f) return 2;
  else if (angle >= 202.5f && angle < 247.5f) return 3;
  else if (angle >= 247.5f && angle < 292.5f) return 4;
  else if (angle >= 292.5f && angle < 337.5f) return 7;
  else if (angle >= 337.5f || angle < 22.5f) return 6;
  return 5;
}