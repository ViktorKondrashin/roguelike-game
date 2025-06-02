#include "ProfileSelect.h"

ProfileSelect::ProfileSelect(sf::Font& font, ProfileManager& manager)
  : fontRef(font), profileManager(manager)
{
  // Кнопку «New Profile» создаём один раз (размер задаётся в конструкторе)
  newProfileButton = std::make_unique<Button>("New Profile", fontRef, sf::Vector2f(buttonWidth, buttonHeight));
  // Кнопку «Back» ставим размером 150×40
  backButton = std::make_unique<Button>("Back", fontRef, sf::Vector2f(150.f, 40.f));
}

// --------------------------------------------------
// Перестраиваем кнопки (вызывается при создании/изменении размера окна и при обновлении списка профилей)
void ProfileSelect::rebuildButtons(const sf::Vector2u& windowSize_) {
  windowSize = windowSize_;
  profileButtons.clear();

  const auto& profiles = profileManager.getProfiles();
  float centerX = windowSize.x / 2.f - buttonWidth / 2.f;

  // Создаём одну кнопку на каждый профиль
  for (size_t i = 0; i < profiles.size(); ++i) {
    const auto& p = profiles[i];
    auto btn = std::make_unique<Button>(p.name, fontRef, sf::Vector2f(buttonWidth, buttonHeight));
    float posY = topMargin + i * (buttonHeight + spacing) + scrollOffset;
    btn->setPosition({ centerX, posY });
    profileButtons.push_back(std::move(btn));
  }

  // Кнопка «New Profile» под списком
  float newPosY = topMargin + profiles.size() * (buttonHeight + spacing) + scrollOffset + 40.f;
  newProfileButton->setPosition({ centerX, newPosY });

  // Кнопка «Back» в левом нижнем углу (отступ 10 пикселей справа/снизу)
  float backX = 10.f;
  float backY = windowSize.y - 10.f - 40.f; // высота backButton = 40
  backButton->setPosition({ backX, backY });

  // После размещения всех кнопок пересчитываем границы прокрутки
  updateScrollLimits();
}

// --------------------------------------------------
// Пересчёт minScroll и maxScroll
void ProfileSelect::updateScrollLimits() {
  size_t count = profileManager.getProfiles().size();
  // Общая высота контента:
  //   topMargin + count*(buttonHeight + spacing) + 40 (пробел перед NewProfile) + buttonHeight (NewProfile) + bottom padding
  float contentHeight = topMargin
    + count * (buttonHeight + spacing)
    + 40.f    // отступ перед «New Profile»
    + buttonHeight
    + 20.f;   // дополнительный нижний отступ

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

// --------------------------------------------------
// Обновление подсветки кнопок при наведении
void ProfileSelect::handleHover(const sf::Vector2f& mousePos) {
  for (auto& btn : profileButtons) {
    btn->update(mousePos);
  }
  newProfileButton->update(mousePos);
  backButton->update(mousePos);
}

// --------------------------------------------------
// Обработка клика: проверяем, какая кнопка была нажата
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

// --------------------------------------------------
// Прокрутка списка при вращении колесика мыши
void ProfileSelect::handleScroll(float delta) {
  // delta>0 (прокрутка вверх) → список идёт вниз (увеличиваем offset)
  // delta<0 (прокрутка вниз) → список идёт вверх (уменьшаем offset)
  scrollOffset += delta * 20.f;
  if (scrollOffset < minScroll) scrollOffset = minScroll;
  if (scrollOffset > maxScroll) scrollOffset = maxScroll;
  // После изменения скролла перестраиваем кнопки
  rebuildButtons(windowSize);
}

// --------------------------------------------------
// Рисуем все элементы экрана
void ProfileSelect::draw(sf::RenderWindow& window) {
  // Рисуем заголовок по центру сверху
  sf::Text title("Select Profile", fontRef, 36);
  title.setFillColor(sf::Color::White);
  float titleX = windowSize.x / 2.f - title.getLocalBounds().width / 2.f;
  title.setPosition({ titleX, 20.f });
  window.draw(title);

  // Кнопки профилей
  for (auto& btn : profileButtons) {
    btn->draw(window);
  }
  // Кнопка «New Profile»
  newProfileButton->draw(window);
  // Кнопка «Back»
  backButton->draw(window);
}
