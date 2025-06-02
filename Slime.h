#pragma once
#include "Monster.h"

class Slime : public Monster {
public:
  Slime(float x, float y, float w, float h, std::string texturePath, Player* player, LevelManager* lvlMgr);
  void update(float time) override;
  void draw(sf::RenderWindow& window) const override;
  void startDying();

private:
  float prevX = 0.f, prevY = 0.f;
  void updateMovement(float time);
  void updateAttack(float time);
  void setTarget(Player* player);
  int getAnimationRow() const;
  int getAttackFrame() const;

  // Анимационные параметры
  float frameTime = 0.0f;
  float currentFrame = 0;
  const float frameDuration = 0.2f;
  std::vector<sf::IntRect> frames;
  std::vector<sf::Vector2f> attackOrigins;

  // Параметры смерти
  sf::Texture deathTexture;
  sf::Sprite deathSprite;
  float deathTimer = 0.0f;
  const float deathDuration = 0.5f;
  bool isDying = false;

  // Параметры атаки
  float attackRange = 30.0f;
  bool isAttacking = false;
  float attackTimer = 0.0f;
  const float attackWindup = 0.2f;
  const float attackStrike = 0.1f;
  const float attackRecovery = 0.3f;
  float currentAttackStageTime = 0.f;
  int currentAttackStage = 0;
};