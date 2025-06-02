#pragma once
#include <SFML/Graphics.hpp>

class MainMenu {
public:
  void loadResources();
  void update(sf::Vector2f mousePos);
  void draw(sf::RenderWindow& window);
  bool isPlayClicked(sf::Vector2f mousePos) const;
  bool isExitClicked(sf::Vector2f mousePos) const;

private:
  struct MenuButton {
    sf::RectangleShape rect;
    sf::Text text;
    float defaultScale = 1.0f;
  };

  sf::Sprite background;
  sf::Texture backgroundTex;
  MenuButton playButton;
  MenuButton exitButton;
  sf::Font font;
};