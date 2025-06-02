#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <functional>
#include <memory>
#include <vector>

enum class StatusEffectType {
  None,
  Burning,
  Freezing,
  Poisoned,
  Stunned,
};

struct StatusEffect {
  StatusEffectType type;
  sf::Time duration;
  sf::Time timeSinceApplied;
  float intensity;
  std::function<void()> onExpire;

  StatusEffect(StatusEffectType t, sf::Time dur, float inten = 1.0f)
    : type(t), duration(dur), timeSinceApplied(sf::Time::Zero), intensity(inten) {}

  bool isExpired() const { return timeSinceApplied >= duration; }
};