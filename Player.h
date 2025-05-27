#pragma once
#include <SFML/Graphics.hpp>
#include "LevelManager.h"
#include "Entity.h"
#include "Weapon.h"
#include <vector>
#include <memory>
class Player:public Entity {
private:
  LevelManager *LevelManager_;
   std::unique_ptr<Weapon> currentWeapon;
  float currentFrame = 0;
  int money = 0;
public:
  Player(float _x, float _y, float _height, float _width, std::string way, LevelManager *lvlMgr);
  void control(float time);
  void setPlayerSpeed(float _speed) { speed = _speed; }
  void setLevelManager(LevelManager* lvlMgr);
  void interactWithMap();
  void setWeapon(std::unique_ptr<Weapon> weapon);
  void attack(const sf::Vector2f& targetPos);
  void update(float deltaTime) override;
  void drawWeapon(sf::RenderWindow& window);
  void reset();
  Weapon* getWeapon() { return currentWeapon.get(); }
  float getPlayerCoordinateX() const { return x; }
  float getPlayerCoordinateY() const { return y; }
  sf::Vector2f getPosition() const { return { x, y }; }
  void addMoney(int _money);
  int getMoney() const { return money; }
};