#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ProfileNewInput {
public:
  ProfileNewInput(sf::Font& font);

  bool handleTextEvent(const sf::Event& event);
  void draw(sf::RenderWindow& window);
  const std::string& getCurrentString() const;

private:
  sf::Font& fontRef;
  std::string currentInput;
  sf::Text textDisplay;
};
