#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ProfileNewInput {
public:
  ProfileNewInput(sf::Font& font);

  // Вызываем при событии TextEntered: возвращает true, если Enter и имя непустое → готово к созданию
  bool handleTextEvent(const sf::Event& event);

  // Также нужно обновить визуальную кнопку Back, если хотим добавить её сюда (опционально)
  // Для простоты — только ввод имени

  // Рисуем поле ввода
  void draw(sf::RenderWindow& window);

  // Возвращает текущее содержимое строки
  const std::string& getCurrentString() const;

private:
  sf::Font& fontRef;
  std::string currentInput;
  sf::Text textDisplay;
};
