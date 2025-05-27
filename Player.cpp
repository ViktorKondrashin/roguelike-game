#include "Player.h"
#include <iostream>
Player::Player(float _x, float _y, float _height, float _width, std::string way, LevelManager* lvlMgr) :Entity(_x, _y, _height, _width, way) { 
  LevelManager_ = lvlMgr; 
  usePixelPerfect = true;
  if (!texture.loadFromFile(way)) {
    std::cerr << "Failed to load player texture: " << way << std::endl;
  }
}

void Player::control(float deltaTime)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    dir = 1;
    currentFrame += 10 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0 + 32 * int(currentFrame), 128, 32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    dir = 3;
    currentFrame += 10 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(127 - 32 * int(currentFrame), 64, -32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    dir = 5;
    currentFrame += 10 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0 + 32 * int(currentFrame), 0, 32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    dir = 7;
    currentFrame += 10 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0 + 32 * int(currentFrame), 64, 32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    dir = 2;
    currentFrame += 5 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(127 - 32 * int(currentFrame), 96, -32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    dir = 8;
    currentFrame += 5 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0 + 32 * int(currentFrame), 96, 32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    dir = 4;
    currentFrame += 5 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(127 - 32 * int(currentFrame), 32, -32, 32));
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    dir = 6;
    currentFrame += 5 * deltaTime;
    if (currentFrame > 4) currentFrame = 0;
    sprite.setTextureRect(sf::IntRect(0 + 32 * int(currentFrame), 32, 32, 32));
  }

}

void Player::update(float deltaTime)
{
  if (life) {// Обновляем статус-эффекты
    statusSystem->update(sf::seconds(deltaTime));

    // Если не оглушен - обрабатываем управление
    if (!hasStatusEffect(StatusEffectType::Stunned)) {
      control(deltaTime);
    }
  }
  // Применяем модификаторы скорости
  float speedModifier = hasStatusEffect(StatusEffectType::Freezing) ? 0.5f : 1.0f;
  
  if (dir != 0) setPlayerSpeed(0.1 * speedModifier);

  switch (dir) {
  case 0: dx = 0; dy = 0; break;
  case 1: dx = 0; dy = -1.41*speed; break;
  case 2: dx = speed; dy = -speed; break;
  case 3: dx = 1.41 *speed; dy = 0; break;
  case 4: dx = speed; dy = speed; break;
  case 5: dx = 0; dy = 1.41 *speed; break;
  case 6: dx = -speed; dy = speed; break;
  case 7: dx = -1.41 *speed; dy = 0; break;
  case 8: dx = -speed; dy = -speed; break;
  }
  x += dx * 1000 * deltaTime;
  y += dy * 1000 * deltaTime;

  speed = 0;
  dir = 0;
  interactWithMap();
  if (hp<= 0) {life=false;}
  sprite.setPosition(x, y);
  if (currentWeapon) {
    currentWeapon->update(deltaTime);
  }
  
}

void Player::drawWeapon(sf::RenderWindow& window)
{
  if (currentWeapon) {
    currentWeapon->draw(window);
  }
}

void Player::reset()
{
  hp = maxHp;
}

void Player::addMoney(int _money)
{
  money += _money;
}




void Player::setLevelManager(LevelManager* lvlMgr)
{
  LevelManager_ = lvlMgr;
}

void Player::interactWithMap() {
  for (int i = y / 32; i <= (y + height - 1) / 32; i++) {
    for (int j = x / 32; j <= (x + width - 1) / 32; j++) {
      char tile = LevelManager_->getTile(j, i);

      if (tile == 's') {
        money++;
        LevelManager_->setTile(j, i, ' ');
      }
      else if (tile == 'l') {
        hp--;
      }
      else if (!LevelManager_->isWalkable(j, i)) {

        float mapWidth = LevelManager_->getWidth() * 32.0f;
        float mapHeight = LevelManager_->getHeight() * 32.0f;

        x = std::clamp(x, 0.0f, mapWidth - width);
        y = std::clamp(y, 0.0f, mapHeight - height);


        float obctacleLeft = j * 32;
        float obctacleRight = (j + 1) * 32;
        float obctacleTop = i * 32;
        float obctacleBottom = (i + 1) * 32;

        float PlayerLeft = x;
        float PlayerRight = x + width;
        float PlayerTop = y;
        float PlayerBottom = y + height;

        float overlapLeft = PlayerRight - obctacleLeft;
        float overlapRight = obctacleRight - PlayerLeft;
        float overlapTop = PlayerBottom - obctacleTop;
        float overlapBottom = obctacleBottom - PlayerTop;

        float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

        if (minOverlap == overlapLeft) {
          x = obctacleLeft - width;
        }
        else if (minOverlap == overlapRight) {
          x = obctacleRight;
        }
        else if (minOverlap == overlapTop) {
          y = obctacleTop - height;
        }
        else if (minOverlap == overlapBottom) {
          y = obctacleBottom;
        }
      }
    }
  }
}

void Player::setWeapon(std::unique_ptr<Weapon> weapon)
{
  weapon->setOwner(this);
  currentWeapon = std::move(weapon);
}

void Player::attack(const sf::Vector2f& targetPos)
{
  if (currentWeapon) {
    // Смещаем точку спавна стрелы вперёд по направлению выстрела
    sf::Vector2f direction = targetPos - sf::Vector2f(x, y);
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) direction /= length;

    sf::Vector2f spawnPos = sf::Vector2f(x, y) + direction * 20.f; // 20 пикселей от центра
    currentWeapon->attack(spawnPos, targetPos);
  }
}
