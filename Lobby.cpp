#include "Lobby.h"
#include "GameManager.h"
#include "Entity.h" 
#include <iostream>


void Lobby::loadResources(const sf::Font& sharedFont) {
  if (!levelManager.loadFromFile("data/lobby.txt")) {
    std::cerr << "Failed to load lobby level!" << std::endl;
  }
  if (!levelManager.loadTileset("image/map.png")) {
    std::cerr << "Failed to load lobby tileset!" << std::endl;
  }

  returnToMenuButton = std::make_unique<Button>("Main Menu", sharedFont, sf::Vector2f(150.f, 40.f));
  returnToMenuButton->setPosition({ 0.f, 0.f });
  returnToMenuButton->setColors(
    sf::Color::White,
    sf::Color::Transparent,
    sf::Color::Transparent,
    sf::Color(80, 80, 120)
  );

  shopObject = std::make_shared<StaticObstacle>(400, 400, 364, 213, "image/shop_prot.png", &levelManager);
  shopObject->setCustomBounds(310.f, 187.f);
  shopObject->setInteractable(true);
  shopObject->setInteractionCallback([]() {
    GameManager::getInstance().enterShop();
    });

  interactionHint.loadResources();

  exitZone = sf::FloatRect(500.f, 0.f, 100.f, 50.f);
}

void Lobby::update(float deltaTime, Player& player) {
  sf::RenderWindow* windowPtr = GameManager::getInstance().getWindowPtr();
  if (!windowPtr) return;

  sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
    sf::Mouse::getPosition(*windowPtr),
    windowPtr->getDefaultView()
  );
  returnToMenuButton->update(mouseScreenPos);

  if (returnToMenuButton->isPressed()) {
    GameManager::getInstance().returnToMainMenu();
  }

  if (shopObject->checkCollision(&player))
    {
        sf::FloatRect p = player.getGlobalBounds();
        sf::FloatRect s = shopObject->getGlobalBounds();
        sf::FloatRect inter;

        if (p.intersects(s, inter))
        {
            if (inter.width < inter.height)
            {
                float push = inter.width - SHOP_PENETRATION;
                if (push > 0.f)
                {
                    player.x += (p.left < s.left ? -push : push);
                    player.dx = 0;
                }
            }
            else
            {
                float push = inter.height - SHOP_PENETRATION;
                if (push > 0.f)
                {
                    player.y += (p.top < s.top ? -push : push);
                    player.dy = 0;
                }
            }
            player.sprite.setPosition(player.x, player.y);
        }
    }
    
  bool showHint = false;
  bool canInteract = false;

  if (isPlayerInShopZone(player.getPosition())) {
    showHint = true;
    canInteract = true;
    interactionHint.setCustomText("Enter Shop");
  }
  else if (isPlayerInExitZone(player.getPosition())) {
    showHint = true;
    canInteract = true;
    interactionHint.setCustomText("Start game");
  }

  interactionHint.setCanInteract(canInteract);
  interactionHint.update(deltaTime, showHint);

  if (showHint && canInteract && sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    if (isPlayerInExitZone(player.getPosition())) {
      GameManager::getInstance().startRun();
    }
    else if (isPlayerInShopZone(player.getPosition())) {
      shopObject->checkPlayerInteraction(player.getPosition());
    }
  }
}

void Lobby::draw(sf::RenderWindow& window) {
  levelManager.drawLevel(window);
  shopObject->draw(window);
  interactionHint.draw(window, window.getView());

  sf::View prevView = window.getView();

  window.setView(window.getDefaultView());

  returnToMenuButton->setPosition({ 20.f, 50.f });
  returnToMenuButton->draw(window);
  window.setView(prevView);
}

bool Lobby::isPlayerInShopZone(sf::Vector2f playerPos) const {
  sf::FloatRect bounds = shopObject->getGlobalBounds();
  bounds.left -= SHOP_INTERACTION_RANGE;
  bounds.top -= SHOP_INTERACTION_RANGE;
  bounds.width += SHOP_INTERACTION_RANGE * 2;
  bounds.height += SHOP_INTERACTION_RANGE * 2;
  return bounds.contains(playerPos);
}

bool Lobby::isPlayerInExitZone(sf::Vector2f playerPos) const {
  return exitZone.contains(playerPos);
}

bool Lobby::isExitToMenuPressed(const sf::Vector2f& mousePos) const {
  return false;
}

void Lobby::setExitZone(const sf::FloatRect& zone) {
    exitZone = zone;
}

void Lobby::setSpawnPoint(const sf::Vector2f& point) {
    spawnPoint_ = point;
}