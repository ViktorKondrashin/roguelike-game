#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "LevelManager.h"
#include "PlayerCamera.h"
#include "RoomManager.h"
#include "MonsterFactory.h"
#include "Bow.h"

int main()
{
  // ������������� ����
  sf::RenderWindow window(sf::VideoMode(640, 480), "Game");

  // ������������� ������� �����������
  LevelManager levelManager;
  if (!levelManager.loadTileset("image/map.png")) {
    std::cerr << "Failed to load tileset!" << std::endl;
    return -1;
  }

  Player�amera camera;
  Player player(500, 320, 32, 32, "image/heroTileSet.png", &levelManager);
  player.setWeapon(std::make_unique<Bow>(50, 10, 200));


  MonsterFactory::registerType<Slime>("SLIME");
  auto testSlime = MonsterFactory::create(
    "SLIME",
    100, 100,  // x, y
    32, 32,    // width, height
    "image/slime.png",
    &player
  );
  if (!testSlime) {
    std::cerr << "CRITICAL ERROR: MonsterFactory cannot create SLIME!" << std::endl;
  }
  else {
    std::cout << "MonsterFactory test passed" << std::endl;
  }
  // ��������� RoomManager
  RoomManager roomManager;
  roomManager.init(&player, &levelManager);
  roomManager.loadRoom("room1");

  // ��������� �������
  sf::Image cursorImage;
  cursorImage.loadFromFile("image/aim.png");
  sf::Cursor customCursor;
  customCursor.loadFromPixels(
    cursorImage.getPixelsPtr(),
    cursorImage.getSize(),
    sf::Vector2u(16, 16)
  );
  window.setMouseCursor(customCursor);

  // ��������� ������
  sf::Font font;
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }
  sf::Text hudText("", font, 20);

  sf::Clock clock;

  while (window.isOpen())
  {
    float deltaTime = clock.restart().asSeconds();

    // ��������� �������
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // ���������� �������� ���������
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      player.attack(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
    }

    player.update(deltaTime);
    roomManager.update(deltaTime);


    // ���������
    window.clear();

    levelManager.drawLevel(window);

    auto view = camera.getViewForPlayer(
      player.getPlayerCoordinateX(),
      player.getPlayerCoordinateY(),
      levelManager,
      window.getSize().x,
      window.getSize().y
    );
    window.setView(view);

    player.drawWeapon(window);
    window.draw(player.getSprite());
    roomManager.draw(window);
    roomManager.drawUI(window, view);
    // ��������� HUD
    hudText.setString("hp:" + std::to_string(player.hp) +
      "  money: " + std::to_string(player.getMoney()));

    hudText.setPosition(view.getCenter().x - 300, view.getCenter().y - 200);
    window.draw(hudText);

    window.display();
  }
  return 0;
}
