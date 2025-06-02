#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Button.h"
#include "ProfileManager.h"

class ProfileSelect {
public:
  ProfileSelect(sf::Font& font, ProfileManager& manager);

  void handleHover(const sf::Vector2f& mousePos);
  std::string handleClick(const sf::Vector2f& mousePos);
  void handleScroll(float delta);
  void rebuildButtons(const sf::Vector2u& windowSize);
  void draw(sf::RenderWindow& window);

private:
  sf::Font& fontRef;
  ProfileManager& profileManager;

  std::vector<std::unique_ptr<Button>> profileButtons;
  std::unique_ptr<Button> newProfileButton;
  std::unique_ptr<Button> backButton;

  sf::Vector2u windowSize;

  float scrollOffset = 0.f;
  float minScroll = 0.f;
  float maxScroll = 0.f;

  const float buttonWidth = 300.f;
  const float buttonHeight = 50.f;
  const float spacing = 20.f;
  const float topMargin = 100.f;

  void updateScrollLimits();
};
