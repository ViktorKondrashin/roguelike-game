#pragma once
#include "StatusEffect.h"
#include <vector>

class Entity;

class StatusSystem {
private:
  Entity* owner;
  std::vector<StatusEffect> activeEffects;

public:
  explicit StatusSystem(Entity* owner) : owner(owner) {}

  void update(sf::Time deltaTime);
  void applyEffect(StatusEffectType type, sf::Time duration, float intensity = 1.0f);
  void removeEffect(StatusEffectType type);
  bool hasEffect(StatusEffectType type) const;
  void clearAllEffects();

  bool isBurning() const { return hasEffect(StatusEffectType::Burning); }
  bool isFrozen() const { return hasEffect(StatusEffectType::Freezing); }
  bool isPoisoned() const { return hasEffect(StatusEffectType::Poisoned); }
  bool isStunned() const { return hasEffect(StatusEffectType::Stunned); }
};