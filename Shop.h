#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "Button.h"

class Player;

class Shop {
public:
  struct ShopItem {
    sf::RectangleShape background;
    sf::Text title;
    sf::Text priceText;
    int cost;
    std::function<void(Player&)> onPurchase;

    ShopItem(const std::string& name, int cost, sf::Vector2f pos, const sf::Font& font);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window) const;
  };

  Shop();
  void loadResources();
  void update(sf::Vector2f mousePos, Player& player);
  void draw(sf::RenderWindow& window) const;
  bool isExitPressed(const sf::Vector2f& mousePos) const;
  void centerShopView(sf::RenderWindow& window); 

private:
  std::vector<ShopItem> items;
  sf::Font font;
  sf::Sprite panel;
  sf::Texture panelTexture;
  Button exitButton;
  sf::View shopView;
  bool wasPressed = false;
  sf::Clock clickCooldown;
  float cooldownTime = 0.3f;
};