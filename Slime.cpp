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
    //кадры движения
      //Вниз
        sf::IntRect(0, 0, 23, 20), 
        sf::IntRect(25, 0, 21, 20),
      //Влево-вниз
        sf::IntRect(0, 23, 23, 15),
        sf::IntRect(24, 21, 23, 17),
      //Влево
        sf::IntRect(0, 39, 24, 19), 
        sf::IntRect(27, 40, 23, 19),
      //Влево-вверх
        sf::IntRect(0, 59, 23, 20),
        sf::IntRect(26, 60, 22, 19), 
      //Вверх
        sf::IntRect(0, 81, 23, 20),
        sf::IntRect(26, 81, 21, 20),
      //Вправо-вниз
        sf::IntRect(23, 23, -23, 15),
        sf::IntRect(47, 21, -23, 17),
      //Вправо
        sf::IntRect(24, 39, -24, 19),
        sf::IntRect(50, 40, -23, 19),
      //Вправо-вверх
        sf::IntRect(23, 59, -23, 20),
        sf::IntRect(48, 60, -22, 19),

    //кадры атаки
      //вниз
        sf::IntRect(0, 102, 23, 18),
        sf::IntRect(25, 102, 37, 57),
        sf::IntRect(63, 102, 37, 61),
        sf::IntRect(103, 102, 33, 57),
        sf::IntRect(137, 101, 23, 18), 
      //вниз-влево
        sf::IntRect(0, 162, 23, 21),
        sf::IntRect(26, 163, 42, 26),
        sf::IntRect(68, 164, 41, 41),
        sf::IntRect(112, 164, 23, 41),
        sf::IntRect(0, 23, 23, 17),
      //влево
        sf::IntRect(0, 207, 25, 19),
        sf::IntRect(29, 207, 40, 19),
        sf::IntRect(70, 207, 59, 19),
        sf::IntRect(130, 207, 36, 23),
        sf::IntRect(169, 207, 25, 19),
      //вверх-влево
        sf::IntRect(0, 232, 23, 21),
        sf::IntRect(26, 231, 43, 26),
        sf::IntRect(69, 231, 41, 41),
        sf::IntRect(112, 231, 24, 41),
        sf::IntRect(138, 23, 23, 20),
      //вверх
        sf::IntRect(0, 272, 22, 20),
        sf::IntRect(23, 272, 37, 57),
        sf::IntRect(65, 272, 37, 61),
        sf::IntRect(103, 272, 32, 57),
        sf::IntRect(138, 271, 22, 20),
      //вниз-вправо
        sf::IntRect(23, 162, -23, 21),
        sf::IntRect(68, 163, -42, 26),
        sf::IntRect(109, 164, -41, 41),
        sf::IntRect(135, 164, -23, 41),
        sf::IntRect(137, 162, -23, 17),
      //вправо
        sf::IntRect(25, 207, -25, 19),
        sf::IntRect(69, 207, -40, 19),
        sf::IntRect(129, 207, -59, 19),
        sf::IntRect(166, 207, -36, 23),
        sf::IntRect(194, 207, 25, -19),
      //вверх-вправо
        sf::IntRect(23, 232, -23, 21),
        sf::IntRect(68, 231, -45, 26),
        sf::IntRect(110, 231, -41, 41),
        sf::IntRect(136, 231, -24, 41),
        sf::IntRect(161, 231, -23, 20),
  };
  attackOrigins = {
     //вниз
        sf::Vector2f{7,8},
        sf::Vector2f{12,5},
        sf::Vector2f{9,5},
        sf::Vector2f{10,4},
        sf::Vector2f{7,6},
      //вниз-влево
        sf::Vector2f{3,13},
        sf::Vector2f{23,9},
        sf::Vector2f{24,8},
        sf::Vector2f{4,8},
        sf::Vector2f{3,9},
     //влево
        sf::Vector2f{5,7},
        sf::Vector2f{21,7},
        sf::Vector2f{40,6},
        sf::Vector2f{17,7},
        sf::Vector2f{5,7},
      //вверх-влево
        sf::Vector2f{7,7},
        sf::Vector2f{33,18},
        sf::Vector2f{31,34},
        sf::Vector2f{11,34},
        sf::Vector2f{11,13},
      //вверх
        sf::Vector2f{12,8},
        sf::Vector2f{22,45},
        sf::Vector2f{15,49},
        sf::Vector2f{12,45},
        sf::Vector2f{12,8},
      //вниз-вправо
        sf::Vector2f{12,13},
        sf::Vector2f{12,9},
        sf::Vector2f{12,8},
        sf::Vector2f{12,8},
        sf::Vector2f{12,9},
      //вправо
        sf::Vector2f{12,7},
        sf::Vector2f{12,7},
        sf::Vector2f{12,6},
        sf::Vector2f{12,7},
        sf::Vector2f{12,7},
      //вверх-вправо
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

    // копируем именно текущий спрайт (с анимационной рамкой)
    deathSprite = sprite;
    // origin уже должен быть настроен у sprite, но убедимся:
    deathSprite.setOrigin(width / 2.0f, height / 2.0f);
    // позиция — тоже взять из текущей:
    deathSprite.setPosition(x + width / 2.0f, y + height / 2.0f);
    // (если хотим — сюда же можно добавить звук смерти)
}



void Slime::update(float time) {
  if (!life) return;

  prevX = x;  prevY = y;

  if (isDying) {
    deathTimer += time;
    if (deathTimer >= deathDuration) {
      life = false;
      // Здесь можно добавить дроп предметов
      target->addMoney(reward);
    }
    return;
  }
  bool chasing = updateAI(time);          // вызвали базовую машину состояний

  if (!chasing) {
      /* в базовом AI монстр уже мог пройти пару-тройку пикселей —
         берём вектор фактического сдвига */
      dx = x - prevX;
      dy = y - prevY;

      /* анимация (два шага) */
      frameTime += time;
      if (frameTime >= frameDuration) {
          frameTime = 0.f;
          currentFrame = 1.f - currentFrame;          // 0 ↔ 1
      }
      int row = getAnimationRow();
      sprite.setTextureRect(frames[row * 2 + static_cast<int>(currentFrame)]);
      sprite.setOrigin(12, 10);

      interactWithMap();
      sprite.setPosition(x, y);
      return;                     // ничего больше не делаем
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
        // Анимация «растворения» (ghost-style)
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

  // Анимация движения
  frameTime += time;
  if (frameTime >= frameDuration) {
    frameTime = 0; currentFrame += 1;
    if (currentFrame >= 2) {
      currentFrame = 0;
    }   // Цикл 0-1
  }

  int row = getAnimationRow();
  sprite.setTextureRect(frames[row * 2 + static_cast<int>(currentFrame)]);
  sprite.setOrigin(12, 10);
}

void Slime::updateAttack(float time) {
  if (!life) return;

  float distanceToPlayer = std::hypot(targetPosition.x - x, targetPosition.y - y);

  // Начало атаки
  if (!isAttacking && distanceToPlayer <= attackRange && currentCooldown <= 0) {
    isAttacking = true;
    currentAttackStage = 0;
    currentAttackStageTime = attackWindup;
  }

  // Обработка атаки
  if (isAttacking) {
    currentAttackStageTime -= time;

    // Переход между стадиями атаки
    if (currentAttackStageTime <= 0) {
      currentAttackStage++;

      switch (currentAttackStage) {
      case 1: // Переход в стадию удара
        currentAttackStageTime = attackStrike;
        if (Collision::PixelPerfectTest(sprite, target->getSprite())) {
          target->takeDamage(damage);
        }
        break;

      case 2: // Переход в стадию восстановления
        currentAttackStageTime = attackRecovery;
        break;

      case 3: // Завершение атаки
        isAttacking = false;
        currentCooldown = attackCooldown;
        break;
      }
    }

    // Анимация атаки
    int attackFrame = getAttackFrame();
    sprite.setTextureRect(frames[16 + getAnimationRow() * 5 + attackFrame]);
    sprite.setOrigin(attackOrigins[getAnimationRow() * 5 + attackFrame]);
  }
}

int Slime::getAttackFrame() const {
  float progress = 0.f;

  switch (currentAttackStage) {
  case 0: // Windup
    progress = 1.f - (currentAttackStageTime / attackWindup);
    return static_cast<int>(progress * 2); // Кадры 0-1
  case 1: // Strike
    return 2; // Главный кадр удара
  case 2: // Recovery
    progress = 1.f - (currentAttackStageTime / attackRecovery);
    return 3 + static_cast<int>(progress * 1); // Кадры 3-4
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

  // Определяем направление (как у игрока)
  if (angle >= 112.5f && angle < 157.5f)  return 1;  // Вниз-влево 
  else if (angle >= 67.5f && angle < 112.5f) return 0;  // Вниз 
  else if (angle >= 157.5f && angle < 202.5f) return 2; // Влево 
  else if (angle >= 202.5f && angle < 247.5f) return 3; // Вверх-влево   
  else if (angle >= 247.5f && angle < 292.5f) return 4; // Вверх 
  else if (angle >= 292.5f && angle < 337.5f) return 7;  // Вверх-вправо  
  else if (angle >= 337.5f || angle < 22.5f) return 6; // вправо 
  return 5; // Вниз-Вправо
}