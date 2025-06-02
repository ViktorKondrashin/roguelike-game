#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Button.h"
#include "ProfileManager.h"

class ProfileSelect {
public:
  ProfileSelect(sf::Font& font, ProfileManager& manager);

  // ѕодсветка (hover) кнопок
  void handleHover(const sf::Vector2f& mousePos);

  // ќбработка клика мыши
  // ¬озвращает:
  // Ц им€ профил€, если кликнули на соответствующую кнопку;
  // Ц "__NEW__", если кликнули New Profile;
  // Ц "__BACK__", если кликнули Back;
  // Ц "" в противном случае.
  std::string handleClick(const sf::Vector2f& mousePos);

  // ѕрокрутка колесиком: delta>0 Ц вверх, delta<0 Ц вниз
  void handleScroll(float delta);

  // ѕерестроить список кнопок с учЄтом нового размера окна
  void rebuildButtons(const sf::Vector2u& windowSize);

  // –исует экран
  void draw(sf::RenderWindow& window);

private:
  sf::Font& fontRef;
  ProfileManager& profileManager;

  std::vector<std::unique_ptr<Button>> profileButtons; // кнопки существующих профилей
  std::unique_ptr<Button> newProfileButton;            // кнопка "New Profile"
  std::unique_ptr<Button> backButton;                  // кнопка "Back"

  sf::Vector2u windowSize; // текущее окно

  float scrollOffset = 0.f; // вертикальное смещение (прокрутка)
  float minScroll = 0.f;
  float maxScroll = 0.f;

  // Ќастройки расположени€
  const float buttonWidth = 300.f;
  const float buttonHeight = 50.f;
  const float spacing = 20.f;    // рассто€ние между кнопками
  const float topMargin = 100.f; // отступ сверху перед первым профилем

  // ѕересчитывает minScroll/maxScroll и корректирует scrollOffset
  void updateScrollLimits();
};
