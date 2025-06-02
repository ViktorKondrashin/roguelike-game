#include "GameManager.h"
#include "PlayerCamera.h"
#include <iostream>
#include "MonsterFactory.h"

// ========================
// Конструктор и Singleton
// ========================
GameManager::GameManager()
  : currentState(GameState::MAIN_MENU),
  shopLoaded(false),
  currentRoomIndex(0),
  allRoomsCompleted(false),
  windowPtr(nullptr),
  escWasPressedLastFrame(false)
{
}

GameManager& GameManager::getInstance() {
  static GameManager instance;
  return instance;
}

// ========================
// Инициализация
// ========================
void GameManager::initialize(sf::RenderWindow& window) {
  windowPtr = &window;

  // Загрузка шрифта
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }

  // HUD-текст
  hudText.setFont(font);
  hudText.setCharacterSize(24);
  hudText.setFillColor(sf::Color::White);

  // Регистрация монстров
  MonsterFactory::registerType<Slime>("SLIME");
  MonsterFactory::registerType<Ghost>("GHOST");
  MonsterFactory::registerType<Skeleton>("SKELETON");

  // Загрузка ресурсов для MainMenu, Lobby и Shop
  mainMenu.loadResources();
  lobby.loadResources(font);
  shop.loadResources();

  // Инициализация системы профилей
  profileManager = std::make_unique<ProfileManager>("profiles.txt");
  profileManager->loadProfiles();
  profileSelectScreen = std::make_unique<ProfileSelect>(font, *profileManager);
  profileNewInputScreen = std::make_unique<ProfileNewInput>(font);
  // Сразу построим кнопки списка под размер окна
  profileSelectScreen->rebuildButtons(window.getSize());

  // Инициализация уровня и комнат
  levelManager = std::make_unique<LevelManager>();
  roomManager = std::make_unique<RoomManager>();

  // Создание игрока и выдача оружия
  player = std::make_unique<Player>(500, 320, 32, 32,
    "image/heroTileSet.png",
    levelManager.get());
  player->setWeapon(std::make_unique<Bow>(50, 10, 200));

  // Загрузка лобби в levelManager
  levelManager->loadFromFile("data/lobby.txt");
  levelManager->loadTileset("image/map.png");

  // Кнопка «Return to Lobby» (позицию задаём при отрисовке)
  returnToLobbyButton = std::make_unique<Button>("Return to Lobby", font,
    sf::Vector2f(200.f, 40.f));
  returnToLobbyButton->setPosition({ 0.f, 0.f });
  returnToLobbyButton->setColors(
    sf::Color::White,
    sf::Color::Transparent,
    sf::Color::Transparent,
    sf::Color(80, 80, 120)
  );

  // Инициализация менеджера комнат
  roomManager->init(player.get(), levelManager.get());

  // Начальный вид
  currentView = window.getDefaultView();
}

// ========================
// Построение HUD
// ========================
void GameManager::drawUI(const sf::View& view) {
  if (!player) return;
  hudText.setString(
    "HP: " + std::to_string(player->getHP()) +
    "  Money: " + std::to_string(player->getMoney())
  );
  hudText.setPosition(view.getCenter().x - 380.f,
    view.getCenter().y - 270.f);
  windowPtr->draw(hudText);
}

// ========================
// Обновление (каждый кадр)
// ========================
void GameManager::update(float deltaTime) {
  if (!windowPtr) return;

  // Глобальная позиция курсора в мире
  sf::Vector2f mousePosWorld = windowPtr->mapPixelToCoords(
    sf::Mouse::getPosition(*windowPtr),
    currentView
  );

  switch (currentState) {
    // ---- Главное меню ----
  case GameState::MAIN_MENU: {
    mainMenu.update(mousePosWorld);
    break;
  }

                           // ---- Выбор профиля ----
  case GameState::PROFILE_SELECT: {
    sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    profileSelectScreen->handleHover(mouseScreenPos);
    break;
  }

                                // ---- Ввод нового профиля ----
  case GameState::PROFILE_NEW_INPUT: {
    // Обработка ввода текста в handleEvent
    break;
  }

                                   // ---- Лобби ----
  case GameState::LOBBY: {
    player->update(deltaTime);
    lobby.update(deltaTime, *player);

    // Пересчёт вида камеры
    currentView = PlayerCamera::getViewForPlayer(
      player->getPlayerCoordinateX(),
      player->getPlayerCoordinateY(),
      lobby.getLevelManager(),
      windowPtr->getSize().x,
      windowPtr->getSize().y
    );

    // Обработка кнопки «Exit to Menu»
    {
      sf::Vector2f mouseScreenPos = windowPtr->mapPixelToCoords(
        sf::Mouse::getPosition(*windowPtr),
        windowPtr->getDefaultView()
      );
      if (lobby.isExitToMenuPressed(mouseScreenPos)) {
        if (!currentProfileName.empty() && player) {
          // Обновляем профиль в памяти
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

    // Клавиша «E» → старт бега
    if (lobby.isPlayerInExitZone(player->getPosition()) &&
      sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
      startRun();
    }
    break;
  }

                       // ---- InGame ----
  case GameState::INGAME: {
    player->update(deltaTime);
    roomManager->update(deltaTime);

    currentView = PlayerCamera::getViewForPlayer(
      player->getPlayerCoordinateX(),
      player->getPlayerCoordinateY(),
      *levelManager,
      windowPtr->getSize().x,
      windowPtr->getSize().y
    );

    // Стрельба левой кнопкой
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      player->attack(mousePosWorld);
    }

    if (!player->life) {
      returnToLobby();
    }
    else if (roomManager->isRoomCleared() && !roomManager->isWaiting()) {
      currentRoomIndex++;
      loadNextRoom();
    }
    break;
  }

                        // ---- Shop ----
  case GameState::SHOP: {
    shop.update(mousePosWorld, *player);
    if (shop.isExitPressed(mousePosWorld)) {
      saveProfiles();
      currentState = GameState::LOBBY;
    }
    break;
  }

                      // ---- Paused ----
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

// ========================
// Отрисовка (каждый кадр)
// ========================
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

  case GameState::SHOP: {
    window.setView(window.getDefaultView());
    shop.draw(window);
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

// ========================
// Обработчик событий
// ========================
void GameManager::handleEvent(const sf::Event& event) {
  // --- 1) Ввод имени нового профиля ---
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

  // --- 2) Экран выбора профиля ---
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

  // --- 3) Главное меню: клик Play/Exit ---
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

  // --- 4) Клавиши ESC / Space для паузы и старта ---
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      togglePause();
      return;
    }
    if (event.key.code == sf::Keyboard::Space &&
      currentState == GameState::LOBBY) {
      startRun();
      return;
    }
  }

  // --- 5) InGame: клик мыши → атака ---
  if (currentState == GameState::INGAME && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePosWorld = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    player->attack(mousePosWorld);
    return;
  }

  // --- 6) Магазин: клик Exit ---
  if (currentState == GameState::SHOP && event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePosWorld = windowPtr->mapPixelToCoords(
      sf::Mouse::getPosition(*windowPtr),
      windowPtr->getDefaultView()
    );
    if (shop.isExitPressed(mousePosWorld)) {
      currentState = GameState::LOBBY;
      return;
    }
  }

  // --- 7) Пауза: клик Return to Lobby ---
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

// ========================
// Загрузка выбранного профиля
// ========================
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

// ========================
// Переходы состояний
// ========================
void GameManager::startProfileSelect() {
  profileManager->loadProfiles();
  profileSelectScreen->rebuildButtons(windowPtr->getSize());
  currentState = GameState::PROFILE_SELECT;
}

void GameManager::startGame() {
  currentState = GameState::LOBBY;
  sf::sleep(sf::milliseconds(100));
  std::cout << "Switched to LOBBY" << std::endl;
}

void GameManager::startRun() {
  currentRoomIndex = 0;
  allRoomsCompleted = false;
  loadNextRoom();
}

void GameManager::returnToLobby() {
  saveProfiles();
  currentState = GameState::LOBBY;
  player->reset();
  player->hp=player->maxHP;
  if (allRoomsCompleted) {
    std::cout << "All rooms completed! Congratulations!" << std::endl;
    allRoomsCompleted = false;
  }
  currentRoomIndex = 0;
}

void GameManager::togglePause() {
  if (currentState == GameState::INGAME) {
    currentState = GameState::PAUSED;
    std::cout << "Game PAUSED" << std::endl;
  }
  else if (currentState == GameState::PAUSED) {
    saveProfiles();
    currentState = GameState::INGAME;
    std::cout << "Game RESUMED" << std::endl;
  }
}

void GameManager::enterShop() {
  currentState = GameState::SHOP;
  player->sprite.setPosition(500.f, 320.f);
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

// ========================
// Загрузка следующей комнаты
// ========================
void GameManager::loadNextRoom() {
  if (currentRoomIndex >= roomList.size()) {
    allRoomsCompleted = true;
    returnToLobby();
    return;
  }

  if (!levelManager->loadFromFile("data/" + roomList[currentRoomIndex])) {
    std::cerr << "Failed to load room: "
      << roomList[currentRoomIndex] << std::endl;
    return;
  }
  std::string roomName = roomList[currentRoomIndex];
  roomName = roomName.substr(0, roomName.find_last_of('.'));
  roomManager->loadRoom(roomName);
  currentState = GameState::INGAME;
  std::cout << "Loading room: " << roomName << std::endl;
}

// ========================
// Сохранение профилей
// ========================
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