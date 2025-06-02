#include "InteractionHint.h"

InteractionHint::InteractionHint() {
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    throw std::runtime_error("Failed to load font");
  }
  hintText.setFont(font);
  hintText.setCharacterSize(20);
  hintText.setFillColor(sf::Color::White);
}

void InteractionHint::loadResources() {
  if (!eKeyTexture.loadFromFile("image/e_key.png")) {
    throw std::runtime_error("Failed to load E key texture");
  }
  eKeySprite.setTexture(eKeyTexture);
  eKeySprite.setTextureRect(sf::IntRect(0, 0, 15, 15));
  eKeySprite.setScale(1.5f, 1.5f);
}

void InteractionHint::update(float deltaTime, bool shouldShow) {
  bool shouldBeVisible = shouldShow && canInteract;

  // Плавное появление/исчезновение
  if (shouldBeVisible) {
    fadeAlpha = std::min(1.0f, fadeAlpha + fadeSpeed * deltaTime);
  }
  else {
    fadeAlpha = std::max(0.0f, fadeAlpha - fadeSpeed * deltaTime);
  }

  isShowing = fadeAlpha > 0.01f;

  if (isShowing) {
    animationTime += deltaTime;
    if (animationTime >= frameTime) {
      animationTime = 0;
      currentFrame = (currentFrame + 1) % frameCount;
      eKeySprite.setTextureRect(sf::IntRect(0, currentFrame * 15, 15, 15));
    }

    float pulse = std::sin(animationTime * 5) * 0.1f + 1.0f;
    eKeySprite.setScale(1.5f * pulse, 1.5f * pulse);
  }
}

void InteractionHint::draw(sf::RenderWindow& window, const sf::View& view) const {
  if (!isShowing) return;

  sf::Vector2f viewCenter = view.getCenter();
  sf::Vector2f viewSize = view.getSize();
  sf::Vector2f hintPosition(
    viewCenter.x + viewSize.x / 2 - 70,
    viewCenter.y + viewSize.y / 2 - 70
  );

  // Применяем прозрачность
  sf::Sprite drawSprite = eKeySprite;
  drawSprite.setPosition(hintPosition);
  drawSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * fadeAlpha)));

  sf::Text drawText = hintText;
  drawText.setString(customText);
  drawText.setPosition(hintPosition.x - 150, hintPosition.y);
  drawText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * fadeAlpha)));

  window.draw(drawSprite);
  window.draw(drawText);
}

void InteractionHint::setCustomText(const std::string& text) {
  customText = text;
}

void InteractionHint::setCanInteract(bool canInteract) {
  this->canInteract = canInteract;
}

bool InteractionHint::getCanInteract() const {
  return canInteract;
}