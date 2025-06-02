#pragma once
#include <SFML/Graphics.hpp>
#include "LevelManager.h"
#include "Entity.h"
#include "Weapon.h"
#include <vector>
#include <memory>
class Player :public Entity {
private:
  std::unique_ptr<Weapon> currentWeapon;
  float currentFrame = 0;
  int money = 0;
  float baseSpeed = 0.1f;
  float profileSpeedMultiplier = 0.1f;
public:
  Player(float _x, float _y, float _height, float _width, std::string way, LevelManager* lvlMgr);
  void control(float time);
  void setPlayerSpeed(float _speed) { speed = _speed; }
  void setLevelManager(LevelManager* lvlMgr);

  void loadProfileStats(int hp, int damage, float speedMultiplier, int startingMoney);
  void setMoney(int m) { money = m; }

  void setWeapon(std::unique_ptr<Weapon> weapon);
  void attack(const sf::Vector2f& targetPos);
  void update(float deltaTime) override;
  void drawWeapon(sf::RenderWindow& window);
  void upgradeHP(int amount);
  void upgradeDamage(int amount);
  void upgradeSpeed(float amount);
  void reset();
  Weapon* getWeapon() { return currentWeapon.get(); }
  int getHP() const { return hp; }
  LevelManager* getLevelManager() const { return levelManager; }
  float getPlayerCoordinateX() const { return x; }
  float getPlayerCoordinateY() const { return y; }
  sf::Vector2f getPosition() const { return { x, y }; }
  void addMoney(int _money);
  int getMoney() const { return money; }
  float getSpeedMultiplier() const { return profileSpeedMultiplier; }

  // Сеттеры для загрузки профиля
  void setMaxHP(int hpValue) { maxHP = hpValue; hp = hpValue; }
  void setStrength(int strengthValue) {
    if (currentWeapon) currentWeapon->damage = strengthValue;
  }
  void setSpeedMultiplier(float mult) {
    profileSpeedMultiplier = mult;
  }
};

//#pragma once
//#include <SFML/Graphics.hpp>
//#include "LevelManager.h"
//#include "Entity.h"
//#include "Weapon.h"
//#include <vector>
//#include <memory>
//
//class Player : public Entity {
//private:
//  std::unique_ptr<Weapon> currentWeapon;
//  float currentFrame = 0;
//  int money = 0;                      // Будет загружаться из профиля
//  float profileSpeedMultiplier = 0.1f; // Будет загружаться из профиля
//  int maxHP = 100;                    // Будет загружаться из профиля
//
//public:
//  // Конструктор: задаёт стартовые значения, текстуру, привязывает LevelManager
//  Player(float _x, float _y, float _height, float _width, const std::string& way, LevelManager* lvlMgr);
//
//  // Основное управление игроком
//  void control(float deltaTime);
//  void setPlayerSpeed(float _speed) { speed = _speed; }
//  void setLevelManager(LevelManager* lvlMgr);
//  void setWeapon(std::unique_ptr<Weapon> weapon);
//  void attack(const sf::Vector2f& targetPos);
//  void update(float deltaTime) override;
//  void drawWeapon(sf::RenderWindow& window);
//
//  // Улучшения
//  void upgradeHP(int amount);
//  void upgradeDamage(int amount);
//  void upgradeSpeed(float amount);
//
//  // Сброс состояния (при выходе из комнаты или перезапуске)
//  void reset();
//
//  // Геттеры для получения текущих характеристик
//  int getMoney() const { return money; }
//  int getHP() const { return hp; }
//  int getMaxHP() const { return maxHP; }
//  int getStrength() const { return currentWeapon ? currentWeapon->damage : 0; }
//  float getSpeedMultiplier() const { return profileSpeedMultiplier; }
//
//  // Сеттеры для загрузки из профиля
//  void setMoney(int m) { money = m; }
//  void setMaxHP(int hpValue) { maxHP = hpValue; hp = hpValue; }
//  void setStrength(int strengthValue) {
//    if (currentWeapon) currentWeapon->damage = strengthValue;
//  }
//  void setSpeedMultiplier(float mult) { profileSpeedMultiplier = mult; }
//
//  // Для передачи в Shop и другие подсистемы
//  Weapon* getWeapon() const { return currentWeapon.get(); }
//  LevelManager* getLevelManager() const { return levelManager; }
//  float getPlayerCoordinateX() const { return x; }
//  float getPlayerCoordinateY() const { return y; }
//  sf::Vector2f getPosition() const { return { x, y }; }
//};
