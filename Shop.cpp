#include "Shop.h"
#include "Player.h"
#include "GameManager.h"
#include <iostream>

Shop::Shop()
  : exitButton("", font, sf::Vector2f(0, 0)) {
}

Shop::ShopItem::ShopItem(const std::string& name, int cost, sf::Vector2f pos, const sf::Font& font)
  : cost(cost) {
  background.setSize(sf::Vector2f(300, 80));
  background.setPosition(pos);
  background.setFillColor(sf::Color(50, 50, 70));
  background.setOutlineThickness(2);
  background.setOutlineColor(sf::Color::White);

  title.setFont(font);
  title.setString(name);
  title.setCharacterSize(24);
  title.setFillColor(sf::Color::White);
  title.setPosition(pos.x + 15, pos.y + 15);

  priceText.setFont(font);
  priceText.setString("Price: " + std::to_string(cost));
  priceText.setCharacterSize(20);
  priceText.setFillColor(sf::Color::Yellow);
  priceText.setPosition(pos.x + 15, pos.y + 45);
}
void Shop::ShopItem::update(sf::Vector2f mousePos) {
  background.setOutlineColor(sf::Color::White);
  background.setFillColor(sf::Color(50, 50, 70));

  if (background.getGlobalBounds().contains(mousePos)) {
    background.setOutlineColor(sf::Color::Green);
    background.setFillColor(sf::Color(70, 70, 90));
  }
}

void Shop::ShopItem::draw(sf::RenderWindow& window) const {
  window.draw(background);
  window.draw(title);
  window.draw(priceText);
}

void Shop::loadResources() {
  if (!panelTexture.loadFromFile("image/shop_bg.png")) {
    std::cerr << "Failed to load shop background!" << std::endl;
  }
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load shop font!" << std::endl;
  }
  panel.setTexture(panelTexture);
  exitButton = Button("Exit", font, sf::Vector2f(150, 50));
  exitButton.setPosition(sf::Vector2f(50, 550));
  exitButton.setColors(sf::Color::White,
    sf::Color(100, 100, 150),
    sf::Color(150, 150, 200),
    sf::Color(80, 80, 120));

  items.emplace_back("HP Boost", 100, sf::Vector2f(100, 150), font);
  items.emplace_back("Damage Boost", 150, sf::Vector2f(100, 250), font);
  items.emplace_back("Speed Boost", 200, sf::Vector2f(100, 350), font);

  items[0].onPurchase = [](Player& player) { player.upgradeHP(10); };
  items[1].onPurchase = [](Player& player) { player.upgradeDamage(5); };
  items[2].onPurchase = [](Player& player) { player.upgradeSpeed(1.2f); };

  shopView.setSize(800, 600);
  shopView.setCenter(400, 300);
}


void Shop::update(sf::Vector2f mousePos, Player& player) {
  bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
  if (!isPressed) {
    wasPressed = false;
  }

  for (auto& item : items) {
    item.update(mousePos);
    if (isPressed && !wasPressed && item.background.getGlobalBounds().contains(mousePos)) {
      if (player.getMoney() >= item.cost) {
        player.addMoney(-item.cost);
        if (item.onPurchase) {
          item.onPurchase(player);
          wasPressed = true;
        }
      }
    }
  }
  exitButton.update(mousePos);
}

void Shop::draw(sf::RenderWindow& window) const {
  window.setView(shopView);
  window.draw(panel);

  sf::Text shopTitle("SHOP", font, 40);
  shopTitle.setPosition(200, 70);
  shopTitle.setFillColor(sf::Color::Yellow);
  window.draw(shopTitle);

  for (const auto& item : items) {
    item.draw(window);
  }
  exitButton.draw(window);

  window.setView(window.getDefaultView());
}

bool Shop::isExitPressed(const sf::Vector2f& mousePos) const {
  return exitButton.isPressed();
}

void Shop::centerShopView(sf::RenderWindow& window) {
  window.setView(shopView);
}