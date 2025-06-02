#pragma once
#include "Entity.h"
#include <functional>

class StaticObstacle : public Entity {
public:
  using InteractionCallback = std::function<void()>;

  StaticObstacle(float x, float y, float width, float height,
    const std::string& texturePath, LevelManager* levelManager);

  void update(float deltaTime) override;
  void draw(sf::RenderWindow& window) const override;
  void onCollision(Entity* other) override {}
  void takeDamage(int damage) override;
  void startDying() override;
  sf::FloatRect getGlobalBounds() const override;
  void setCustomBounds(float width, float height);

  void setInteractionCallback(InteractionCallback callback);
  void checkPlayerInteraction(const sf::Vector2f& playerPosition);
  void setInteractable(bool interactable);
  sf::FloatRect getInteractionBounds() const;
private:
  InteractionCallback interactionCallback;
  bool isInteractable = false;
  sf::Text interactHint;
  sf::Font font;
  float customWidth = 320.f;
  float customHeight = 187.f;
};