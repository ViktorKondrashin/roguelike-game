#include "StatusSystem.h"
#include "Entity.h"

void StatusSystem::update(sf::Time deltaTime) {
  for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
    it->timeSinceApplied += deltaTime;

    switch (it->type) {
    case StatusEffectType::Burning:
    case StatusEffectType::Poisoned:
      if (it->timeSinceApplied.asSeconds() >= 1.0f) {
        owner->hp -= it->intensity;
        it->timeSinceApplied = sf::Time::Zero;
      }
      break;
    default:
      break;
    }

    if (it->isExpired()) {
      it = activeEffects.erase(it);
    }
    else {
      ++it;
    }
  }
}


void StatusSystem::applyEffect(StatusEffectType type, sf::Time duration, float intensity) {
  for (auto& effect : activeEffects) {
    if (effect.type == type) {
      effect.duration = duration;
      effect.timeSinceApplied = sf::Time::Zero;
      effect.intensity = intensity;
      return;
    }
  }

  activeEffects.emplace_back(type, duration, intensity);

}

void StatusSystem::removeEffect(StatusEffectType type) {
  activeEffects.erase(
    std::remove_if(activeEffects.begin(), activeEffects.end(),
      [type](const StatusEffect& effect) { return effect.type == type; }),
    activeEffects.end());
}

bool StatusSystem::hasEffect(StatusEffectType type) const {
  return std::any_of(activeEffects.begin(), activeEffects.end(),
    [type](const StatusEffect& effect) { return effect.type == type; });
}

void StatusSystem::clearAllEffects() {
  activeEffects.clear();
}