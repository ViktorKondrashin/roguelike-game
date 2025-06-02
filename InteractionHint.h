#pragma once
#include <SFML/Graphics.hpp>

class InteractionHint {
public:
  InteractionHint();
  void loadResources();
  void update(float deltaTime, bool shouldShow);
  void draw(sf::RenderWindow& window, const sf::View& view) const;
  void setCustomText(const std::string& text);
  void setCanInteract(bool canInteract);
  bool getCanInteract() const;

private:
  sf::Texture eKeyTexture;
  mutable sf::Sprite eKeySprite;
  mutable sf::Text hintText;
  sf::Font font;

  bool isShowing = false;
  bool canInteract = false;
  float animationTime = 0.f;
  float fadeAlpha = 0.f; // Добавлено для плавного появления
  const float frameTime = 0.5f;
  const float fadeSpeed = 3.0f; // Скорость появления/исчезновения
  const int frameCount = 2;
  int currentFrame = 0;
  std::string customText = "Press E";
};