#pragma once

#include <SFML/Graphics.hpp>
#include "LevelManager.h"
#include "RoomManager.h"
#include "Player.h"
#include "MainMenu.h"
#include "Lobby.h"
#include "Shop.h"
#include "Button.h"
#include "ProfileManager.h"
#include "ProfileSelect.h"
#include "ProfileNewInput.h"
#include <memory>
#include <vector>

class GameManager {
public:
  enum class GameState {
    MAIN_MENU,
    PROFILE_SELECT,
    PROFILE_NEW_INPUT,
    LOBBY,
    INGAME,
    PAUSED,
    SHOP
  };

  static GameManager& getInstance();

  void initialize(sf::RenderWindow& window);
  void update(float deltaTime);
  void render(sf::RenderWindow& window);

  void startProfileSelect();

  void startGame();
  void startRun();
  void returnToLobby();
  void togglePause();
  void enterShop();
  void returnToMainMenu();

  void handleEvent(const sf::Event& event);
  void applyProfile(const std::string & profileName);

  GameState getCurrentState() const { return currentState; }
  sf::RenderWindow* getWindowPtr() const { return windowPtr; }

  ProfileManager& getProfileManager() { return *profileManager; }
  Player* getPlayer() { return player.get(); }

  bool isPlayClicked(const sf::Vector2f& mousePos);
  bool isExitClicked(const sf::Vector2f& mousePos);

  void saveProfiles();

private:
  GameManager();
  ~GameManager() = default;

  void loadNextRoom();
  void drawUI(const sf::View& view);

  GameState currentState = GameState::MAIN_MENU;
  std::unique_ptr<Player> player;
  std::unique_ptr<LevelManager> levelManager;
  std::unique_ptr<RoomManager> roomManager;
  Shop shop;
  bool shopLoaded = false;
  MainMenu mainMenu;
  Lobby lobby;
  sf::Font font;
  sf::Text hudText;
  sf::View currentView;
  sf::RenderWindow* windowPtr = nullptr;

  std::vector<std::string> roomList = { "room1.txt", "room2.txt", "room3.txt" };
  size_t currentRoomIndex = 0;
  bool allRoomsCompleted = false;

  std::unique_ptr<Button> returnToLobbyButton;

  // Флаг для однократной обработки ESC в update()
  bool escWasPressedLastFrame = false;

  std::unique_ptr<ProfileManager> profileManager;
  std::unique_ptr<ProfileSelect> profileSelectScreen;
  std::unique_ptr<ProfileNewInput> profileNewInputScreen;
  std::string newProfileName;      // временно хранит ввод из ProfileNewInput
  std::string currentProfileName;
};