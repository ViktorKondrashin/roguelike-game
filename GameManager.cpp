#include "GameManager.h"
#include "PlayerCamera.h"
#include <iostream>
#include "MonsterFactory.h"

GameManager::GameManager()
  : currentState(GameState::MAIN_MENU),
  shopLoaded(false),
  currentRoomIndex(0),
  allRoomsCompleted(false),
  windowPtr(nullptr),
  escWasPressedLastFrame(false) {}

GameManager& GameManager::getInstance() {
  static GameManager instance;
  return instance;
}

void GameManager::returnToLobby() {
  saveProfiles();
  currentState = GameState::LOBBY;

  player->reset();
  player->hp = player->maxHP;

  const sf::Vector2f p = lobby.getSpawnPoint();
  player->x = p.x;
  player->y = p.y;
  player->sprite.setPosition(p);

  player->setLevelManager(&lobby.getLevelManager());
  roomManager->init(player.get(), &lobby.getLevelManager());
  if (allRoomsCompleted) {
    std::cout << "All rooms completed! Congratulations!" << std::endl;
    allRoomsCompleted = false;
  }
  roomManager->resetWaves();
  currentRoomIndex = 0;
}

void GameManager::togglePause() {
  static sf::Clock cooldown;
  if (cooldown.getElapsedTime().asMilliseconds() < 200) return;

  if (currentState == GameState::INGAME) {
    currentState = GameState::PAUSED;
    std::cout << "Game PAUSED" << std::endl;
  }
  else if (currentState == GameState::PAUSED) {
    currentState = GameState::INGAME;
    std::cout << "Game RESUMED" << std::endl;
  }
}

void GameManager::initialize(sf::RenderWindow& window) {
  windowPtr = &window;

  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }

  sf::Image cursorImage;
  cursorImage.loadFromFile("image/aim.png");
  if (
      !customCursor.loadFromPixels(
          cursorImage.getPixelsPtr(),
          cursorImage.getSize(),
          sf::Vector2u(16, 16)
      )) {
      std::cerr << "Failed to load cursor!" << std::endl;
  }
  window.setMouseCursor(customCursor);

  hudText.setFont(font);
  hudText.setCharacterSize(24);
  hudText.setFillColor(sf::Color::White);

  MonsterFactory::registerType<Slime>("SLIME");
  MonsterFactory::registerType<Ghost>("GHOST");
  MonsterFactory::registerType<Skeleton>("SKELETON");

  mainMenu.loadResources();
  lobby.loadResources(font);
  shop.loadResources();

  profileManager = std::make_unique<ProfileManager>("profiles.txt");
  profileManager->loadProfiles();
  profileSelectScreen = std::make_unique<ProfileSelect>(font, *profileManager);
  profileNewInputScreen = std::make_unique<ProfileNewInput>(font);
  profileSelectScreen->rebuildButtons(window.getSize());

  levelManager = std::make_unique<LevelManager>();
  roomManager = std::make_unique<RoomManager>();

  player = std::make_unique<Player>(500, 320, 32, 32, "image/heroTileSet.png", levelManager.get());
  player->setWeapon(std::make_unique<Bow>(50, 10, 200));

  levelManager->loadFromFile("data/lobby.txt");
  levelManager->loadTileset("image/map.png");

  returnToLobbyButton = std::make_unique<Button>("Return to Lobby", font,
    sf::Vector2f(200.f, 40.f));
  returnToLobbyButton->setPosition({ 0.f, 0.f });
  returnToLobbyButton->setColors(
    sf::Color::White,
    sf::Color::Transparent,
    sf::Color::Transparent,
    sf::Color(80, 80, 120)
  );

  roomManager->init(player.get(), levelManager.get());

  currentView = window.getDefaultView();

  lobby.setExitZone({ 380, 0, 130, 160 });
  lobby.setSpawnPoint({ 450, 330 });
}

void GameManager::drawUI(const sf::View& view) {
    if (!player || !windowPtr) return;

    sf::View oldView = windowPtr->getView();

    windowPtr->setView(windowPtr->getDefaultView());

    hudText.setString(
        "HP: " + std::to_string(player->getHP()) +
        "  Money: " + std::to_string(player->getMoney())
    );
    hudText.setPosition(600.f, 10.f);
    windowPtr->draw(hudText);
    hudText.setCharacterSize(15);
    hudText.setFillColor(sf::Color::White);

    windowPtr->setView(oldView);
}

void GameManager::update(float deltaTime) {
  if (!windowPtr) return;
  sf::Vector2f mousePosWorld = windowPtr->mapPixelToCoords(
    sf::Mouse::getPosition(*windowPtr),
    currentView
  );

  switch (currentState) {
  case GameState::MAIN_MENU: {
    mainMenu.update(mousePosWorld);
    break;
  }

  case GameState::PROFILE_SELECT: {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    profileSelectScreen->handleHover(mouseScreenPos);
    break;
  }

  case GameState::PROFILE_NEW_INPUT: {
    break;
  }

  case GameState::LOBBY:
    player->update(deltaTime);
    lobby.update(deltaTime, *player);

    currentView = PlayerCamera::getViewForPlayer(
      player->getPlayerCoordinateX(),
      player->getPlayerCoordinateY(),
      lobby.getLevelManager(),
      windowPtr->getSize().x,
      windowPtr->getSize().y
    );

    {
      sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
        sf::Mouse::getPosition(*windowPtr),
        windowPtr->getDefaultView()
      );
      if (lobby.isExitToMenuPressed(mouseScreenPos)) {
        if (!currentProfileName.empty() && player) {
          for (auto& pr : const_cast<std::vector<Profile>&>(
            profileManager->getProfiles())) {
            if (pr.name == currentProfileName) {
              pr.money = player->getMoney();
              pr.maxHP = player->getHP();
              if (player->getWeapon())
                pr.strength = player->getWeapon()->damage;
              pr.speed = player->getSpeedMultiplier();
              break;
            }
          }
          profileManager->saveProfiles();
        }
        currentState = GameState::MAIN_MENU;
        return;
      }
    }

    if (lobby.isPlayerInExitZone(player->getPosition()) && sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
      startRun();
    }
    break;

  case GameState::INGAME:
    player->update(deltaTime);
    roomManager->update(deltaTime);

    currentView = PlayerCamera::getViewForPlayer(
      player->getPlayerCoordinateX(),
      player->getPlayerCoordinateY(),
      *levelManager,
      windowPtr->getSize().x,
      windowPtr->getSize().y
    );

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      player->attack(mousePosWorld);
    }

    if (!player->life) {
      returnToLobby();
    }
    else if (roomManager->isRoomCleared() &&
        roomManager->isPlayerNearExit() &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        currentRoomIndex++;
        loadNextRoom();
    }
    break;

  case GameState::SHOP:
    shop.update(mousePosWorld, *player);
    if (shop.isExitPressed(mousePosWorld)) {
      saveProfiles();
      currentState = GameState::LOBBY;
    }
    break;

  case GameState::PAUSED: {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    returnToLobbyButton->update(mouseScreenPos);
    if (returnToLobbyButton->isPressed()) {
      returnToLobby();
    }
    break;
  }

  default:
    break;
  }
}

void GameManager::render(sf::RenderWindow& window) {
  window.clear();

  switch (currentState) {
  case GameState::MAIN_MENU: {
    window.setView(window.getDefaultView());
    mainMenu.draw(window);
    break;
  }

  case GameState::PROFILE_SELECT: {
    window.setView(window.getDefaultView());
    profileSelectScreen->draw(window);
    break;
  }

  case GameState::PROFILE_NEW_INPUT: {
    window.setView(window.getDefaultView());
    profileNewInputScreen->draw(window);
    break;
  }

  case GameState::LOBBY: {
    window.setView(currentView);
    lobby.draw(window);
    player->draw(window);
    drawUI(currentView);
    break;
  }

  case GameState::SHOP: {
    window.setView(window.getDefaultView());
    shop.draw(window);
    drawUI(currentView);
    break;
  }

  case GameState::INGAME: {
    window.setView(currentView);
    levelManager->drawLevel(window);
    player->draw(window);
    roomManager->draw(window);
    player->drawWeapon(window);
    roomManager->drawUI(window, currentView);
    drawUI(currentView);
    break;
  }

  case GameState::PAUSED: {
    window.setView(currentView);
    levelManager->drawLevel(window);
    player->draw(window);
    roomManager->draw(window);
    player->drawWeapon(window);
    roomManager->drawUI(window, currentView);
    drawUI(currentView);

    sf::View prevView = window.getView();
    window.setView(window.getDefaultView());

    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    overlay.setPosition(0, 0);
    window.draw(overlay);

    sf::Text pausedText("PAUSED (Press ESC to resume)", font, 40);
    sf::FloatRect textBounds = pausedText.getLocalBounds();
    pausedText.setOrigin(
      textBounds.left + textBounds.width / 2.f,
      textBounds.top + textBounds.height / 2.f
    );
    pausedText.setPosition(
      window.getSize().x / 2.f,
      window.getSize().y / 2.f - 30.f
    );
    window.draw(pausedText);

    const float btnWidth = 200.f;
    const float btnHeight = 40.f;
    float btnX = (window.getSize().x - btnWidth) / 2.f;
    float btnY = (window.getSize().y / 2.f) + 10.f;
    returnToLobbyButton->setPosition({ btnX, btnY });
    returnToLobbyButton->draw(window);

    window.setView(prevView);
    break;
  }

  default:
    break;
  }

  window.display();
}

void GameManager::handleEvent(const sf::Event& event) {
  if (currentState == GameState::PROFILE_NEW_INPUT) {
    if (event.type == sf::Event::TextEntered) {
      bool ready = profileNewInputScreen->handleTextEvent(event);
      if (ready) {
        newProfileName = profileNewInputScreen->getCurrentString();
        Profile newP;
        newP.name = newProfileName;
        newP.money = 0;
        newP.maxHP = 100;
        newP.strength = 10;
        newP.speed = 0.1f;
        profileManager->addProfile(newP);
        profileManager->saveProfiles();

        applyProfile(newProfileName);
        currentState = GameState::LOBBY;
        profileSelectScreen->rebuildButtons(windowPtr->getSize());
      }
    }
    return;
  }

  if (currentState == GameState::PROFILE_SELECT) {
    if (event.type == sf::Event::MouseWheelScrolled) {
      profileSelectScreen->handleScroll(event.mouseWheelScroll.delta);
      return;
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
        sf::Mouse::getPosition(*windowPtr),
        windowPtr->getDefaultView()
      );
      profileSelectScreen->handleHover(mouseScreenPos);
      std::string selection = profileSelectScreen->handleClick(mouseScreenPos);
      if (!selection.empty()) {
        if (selection == "__NEW__") {
          newProfileName.clear();
          profileNewInputScreen = std::make_unique<ProfileNewInput>(font);
          currentState = GameState::PROFILE_NEW_INPUT;
        }
        else if (selection == "__BACK__") {
          currentState = GameState::MAIN_MENU;
        }
        else {
          applyProfile(selection);
          currentState = GameState::LOBBY;
        }
        profileSelectScreen->rebuildButtons(windowPtr->getSize());
      }
    }
    return;
  }

  if (currentState == GameState::MAIN_MENU && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    if (mainMenu.isPlayClicked(mouseScreenPos)) {
      startProfileSelect();
      profileSelectScreen->rebuildButtons(windowPtr->getSize());
      return;
    }
    if (mainMenu.isExitClicked(mouseScreenPos)) {
      saveProfiles();
      windowPtr->close();
      return;
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape && !escWasPressedLastFrame) {
      togglePause();
      escWasPressedLastFrame = true;
      return;
    }
    if (event.key.code == sf::Keyboard::Space &&
      currentState == GameState::LOBBY) {
      startRun();
      return;
    }
  }

  if (currentState == GameState::INGAME && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePosWorld = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    player->attack(mousePosWorld);
    return;
  }

  if (currentState == GameState::SHOP && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    if (shop.isExitPressed(mouseScreenPos)) {
      startProfileSelect();
      currentState = GameState::LOBBY;
      return;
    }
  }

  if (currentState == GameState::PAUSED && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    returnToLobbyButton->update(mouseScreenPos);
    if (returnToLobbyButton->isPressed()) {
      returnToLobby();
      return;
    }
  }
}

void GameManager::applyProfile(const std::string& profileName) {
  auto opt = profileManager->getProfile(profileName);
  if (!opt.has_value()) return;
  Profile p = opt.value();

  currentProfileName = profileName;
  if (!player) {
    player = std::make_unique<Player>(500, 320, 32, 32,
      "image/heroTileSet.png",
      levelManager.get());
    player->setWeapon(std::make_unique<Bow>(50, 10, 200));
  }
  player->setMoney(p.money);
  player->setMaxHP(p.maxHP);
  player->setStrength(p.strength);
  player->setSpeedMultiplier(p.speed);
  player->reset();
}

void GameManager::startProfileSelect() {
  profileManager->loadProfiles();
  profileSelectScreen->rebuildButtons(windowPtr->getSize());
  currentState = GameState::PROFILE_SELECT;
}

void GameManager::startGame() {
  player->setLevelManager(&lobby.getLevelManager());
  roomManager->init(player.get(), &lobby.getLevelManager());
  currentState = GameState::LOBBY;
  sf::sleep(sf::milliseconds(100));
  std::cout << "Switched to LOBBY" << std::endl;
}

void GameManager::startRun() {
  currentRoomIndex = 0;
  allRoomsCompleted = false;
  player->setLevelManager(levelManager.get());
  roomManager->init(player.get(), levelManager.get());
  loadNextRoom();
}

void GameManager::loadNextRoom() {
  if (currentRoomIndex >= roomList.size()) {
    allRoomsCompleted = true;
    returnToLobby();
    return;
  }

  if (!levelManager->loadFromFile("data/" + roomList[currentRoomIndex])) {
    std::cerr << "Failed to load room: " << roomList[currentRoomIndex] << std::endl;
    return;
  }

  std::string roomName = roomList[currentRoomIndex];
  roomName = roomName.substr(0, roomName.find_last_of('.'));

  roomManager->loadRoom(roomName);

  roomManager->resetWaves();
  if (auto pos = levelManager->findTile('r')) {
      sf::Vector2f spawn((pos->x + 0.5f) * 32.f, (pos->y + 0.5f) * 32.f);
      player->x = spawn.x;
      player->y = spawn.y;
      player->sprite.setPosition(spawn);
      levelManager->setTile(pos->x, pos->y, '=');
  }

  currentState = GameState::INGAME;
  std::cout << "Loading room: " << roomName << std::endl;
}

void GameManager::enterShop() {
  currentState = GameState::SHOP;
  player->sprite.setPosition(500, 320);
  if (!shopLoaded) {
    shop.loadResources();
    shopLoaded = true;
  }
}

void GameManager::returnToMainMenu() {
  currentState = GameState::MAIN_MENU;
  player->reset();
  currentRoomIndex = 0;
  allRoomsCompleted = false;
}

bool GameManager::isPlayClicked(const sf::Vector2f& mousePos) {
  return mainMenu.isPlayClicked(mousePos);
}

bool GameManager::isExitClicked(const sf::Vector2f& mousePos) {
  return mainMenu.isExitClicked(mousePos);
}

void GameManager::saveProfiles() {
  if (!profileManager) return;

  if (!currentProfileName.empty() && player) {
    for (auto& pr : const_cast<std::vector<Profile>&>(
      profileManager->getProfiles())) {
      if (pr.name == currentProfileName) {
        pr.money = player->getMoney();
        pr.maxHP = player->maxHP;
        if (player->getWeapon())
          pr.strength = player->getWeapon()->damage;
        pr.speed = player->getSpeedMultiplier();
        break;
      }
    }
  }
  profileManager->saveProfiles();
}