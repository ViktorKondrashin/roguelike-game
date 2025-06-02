#include "ProfileSelect.h"

ProfileSelect::ProfileSelect(sf::Font& font, ProfileManager& manager)
  : fontRef(font), profileManager(manager)
{
  newProfileButton = std::make_unique<Button>("New Profile", fontRef, sf::Vector2f(buttonWidth, buttonHeight));
  backButton = std::make_unique<Button>("Back", fontRef, sf::Vector2f(150.f, 40.f));
}

void ProfileSelect::rebuildButtons(const sf::Vector2u& windowSize_) {
  windowSize = windowSize_;
  profileButtons.clear();

  const auto& profiles = profileManager.getProfiles();
  float centerX = windowSize.x / 2.f - buttonWidth / 2.f;

  for (size_t i = 0; i < profiles.size(); ++i) {
    const auto& p = profiles[i];
    auto btn = std::make_unique<Button>(p.name, fontRef, sf::Vector2f(buttonWidth, buttonHeight));
    float posY = topMargin + i * (buttonHeight + spacing) + scrollOffset;
    btn->setPosition({ centerX, posY });
    profileButtons.push_back(std::move(btn));
  }

  float newPosY = topMargin + profiles.size() * (buttonHeight + spacing) + scrollOffset + 40.f;
  newProfileButton->setPosition({ centerX, newPosY });

  float backX = 10.f;
  float backY = windowSize.y - 10.f - 40.f;
  backButton->setPosition({ backX, backY });

  updateScrollLimits();
}

void ProfileSelect::updateScrollLimits() {
  size_t count = profileManager.getProfiles().size();
  float contentHeight = topMargin
    + count * (buttonHeight + spacing)
    + 40.f
    + buttonHeight
    + 20.f;

  if (contentHeight > windowSize.y) {
    minScroll = windowSize.y - contentHeight;
    maxScroll = 0.f;
  }
  else {
    minScroll = maxScroll = 0.f;
  }
  if (scrollOffset < minScroll) scrollOffset = minScroll;
  if (scrollOffset > maxScroll) scrollOffset = maxScroll;
}

void ProfileSelect::handleHover(const sf::Vector2f& mousePos) {
  for (auto& btn : profileButtons) {
    btn->update(mousePos);
  }
  newProfileButton->update(mousePos);
  backButton->update(mousePos);
}

std::string ProfileSelect::handleClick(const sf::Vector2f& mousePos) {
  for (size_t i = 0; i < profileButtons.size(); ++i) {
    if (profileButtons[i]->isPressed()) {
      return profileManager.getProfiles()[i].name;
    }
  }
  if (newProfileButton->isPressed()) {
    return "__NEW__";
  }
  if (backButton->isPressed()) {
    return "__BACK__";
  }
  return "";
}

void ProfileSelect::handleScroll(float delta) {
  scrollOffset += delta * 20.f;
  if (scrollOffset < minScroll) scrollOffset = minScroll;
  if (scrollOffset > maxScroll) scrollOffset = maxScroll;
  rebuildButtons(windowSize);
}

void ProfileSelect::draw(sf::RenderWindow& window) {
  sf::Text title("Select Profile", fontRef, 36);
  title.setFillColor(sf::Color::White);
  float titleX = windowSize.x / 2.f - title.getLocalBounds().width / 2.f;
  title.setPosition({ titleX, 20.f });
  window.draw(title);

  for (auto& btn : profileButtons) {
    btn->draw(window);
  }
  newProfileButton->draw(window);
  backButton->draw(window);
}
