#include <SFML/Graphics.hpp>
#include "GameManager.h"

int main() {
  // 1. Создаём окно 800×600
  sf::RenderWindow window(sf::VideoMode(800, 600), "Roguelike");
  // 2. Получаем единственный экземпляр GameManager и инициализируем его
  GameManager& game = GameManager::getInstance();
  game.initialize(window);

  sf::Clock clock;
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    sf::Event event;

    // 3. Обрабатываем все события
    while (window.pollEvent(event)) {
      // 3.1. Если закрывают окно системным крестиком
      if (event.type == sf::Event::Closed) {
        // Перед закрытием сохраняем профиль (если он был выбран)
        game.saveProfiles();
        window.close();
        break; // выходим из внутреннего while и завершение цикла
      }

      // 3.2. Передаём все события в GameManager
      game.handleEvent(event);

      // 3.3. Дополнительная логика: Space и Escape
      if (event.type == sf::Event::KeyPressed) {
        // 3.3.1. Нажатие Space: если мы в LOBBY, стартуем раунд
        if (event.key.code == sf::Keyboard::Space &&
          game.getCurrentState() == GameManager::GameState::LOBBY) {
          game.startRun();
        }
      }

      // 3.4. Клик мыши: проверка Play/Exit в главном меню
      if (event.type == sf::Event::MouseButtonPressed) {
        // Переводим координаты пикселя в координаты default view (GUI)
        sf::Vector2f mousePos = window.mapPixelToCoords(
          sf::Mouse::getPosition(window),
          window.getDefaultView()
        );

        // Если мы на главном меню — ещё раз проверяем Play/Exit
        if (game.getCurrentState() == GameManager::GameState::MAIN_MENU) {
          if (game.isPlayClicked(mousePos)) {
            // Нажатие Play: переходим к выбору профиля
            game.startProfileSelect();
          }
          else if (game.isExitClicked(mousePos)) {
            // Нажатие Exit: сразу сохраняем и закрываем
            game.saveProfiles();
            window.close();
          }
        }
      }
    }

    // 4. Обновляем состояние игры
    game.update(deltaTime);
    // 5. Отрисовываем
    game.render(window);
    // Не вызываем window.display() — он уже вызывается внутри game.render()
  }

  return 0;
}
//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include "Player.h"
//#include "LevelManager.h"
//#include "PlayerCamera.h"
//#include "RoomManager.h"
//#include "MonsterFactory.h"
//#include "Bow.h"
//
//int main()
//{
//  // Инициализация окна
//  sf::RenderWindow window(sf::VideoMode(640, 480), "Game");
//
//  // Инициализация игровых компонентов
//  LevelManager levelManager;
//  if (!levelManager.loadFromFile("data/room3.txt")) {
//    std::cerr << "Failed to load level data!" << std::endl;
//  }
//  if (!levelManager.loadTileset("image/map.png")) {
//    std::cerr << "Failed to load tileset!" << std::endl;
//    return -1;
//  }
//
//  PlayerСamera camera;
//  Player player(500, 320, 32, 32, "image/heroTileSet.png", &levelManager);
//  player.setWeapon(std::make_unique<Bow>(50, 10, 200));
//
//
//  MonsterFactory::registerType<Slime>("SLIME");
//  auto testSlime = MonsterFactory::create(
//    "SLIME",
//    100, 100,  // x, y
//    32, 32,    // width, height
//    "image/slime.png",
//    &player
//  );
//  if (!testSlime) {
//    std::cerr << "CRITICAL ERROR: MonsterFactory cannot create SLIME!" << std::endl;
//  }
//  else {
//    std::cout << "MonsterFactory test passed" << std::endl;
//  }
//  // Настройка RoomManager
//  RoomManager roomManager;
//  roomManager.init(&player, &levelManager);
//  roomManager.loadRoom("room1");
//
//  // Настройка курсора
//  sf::Image cursorImage;
//  cursorImage.loadFromFile("image/aim.png");
//  sf::Cursor customCursor;
//  customCursor.loadFromPixels(
//    cursorImage.getPixelsPtr(),
//    cursorImage.getSize(),
//    sf::Vector2u(16, 16)
//  );
//  window.setMouseCursor(customCursor);
//
//  // Настройка шрифта
//  sf::Font font;
//  if (!font.loadFromFile("font/minecraft_0.ttf")) {
//    std::cerr << "Failed to load font!" << std::endl;
//  }
//  sf::Text hudText("", font, 20);
//
//  sf::Clock clock;
//
//  while (window.isOpen())
//  {
//    float deltaTime = clock.restart().asSeconds();
//
//    // Обработка событий
//    sf::Event event;
//    while (window.pollEvent(event)) {
//      if (event.type == sf::Event::Closed)
//        window.close();
//    }
//
//    // Обновление игрового состояния
//    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//      player.attack(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
//    }
//
//    player.update(deltaTime);
//    roomManager.update(deltaTime);
//
//
//    // Отрисовка
//    window.clear();
//
//    levelManager.drawLevel(window);
//
//    auto view = camera.getViewForPlayer(
//      player.getPlayerCoordinateX(),
//      player.getPlayerCoordinateY(),
//      levelManager,
//      window.getSize().x,
//      window.getSize().y
//    );
//    window.setView(view);
//
//    player.drawWeapon(window);
//    window.draw(player.getSprite());
//    roomManager.draw(window);
//    roomManager.drawUI(window, view);
//    // Отрисовка HUD
//    hudText.setString("hp:" + std::to_string(player.hp) +
//      "  money: " + std::to_string(player.getMoney()));
//
//    hudText.setPosition(view.getCenter().x - 300, view.getCenter().y - 200);
//    window.draw(hudText);
//
//    window.display();
//  }
//  return 0;
//}
