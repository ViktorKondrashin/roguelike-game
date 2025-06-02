#include "MainMenu.h"
#include <iostream>

void MainMenu::loadResources() {
  // Загрузка фона
  if (!backgroundTex.loadFromFile("image/menu_bg.png")) {
    std::cerr << "Failed to load menu background!" << std::endl;
  }
  background.setTexture(backgroundTex);

  // Загрузка шрифта
  if (!font.loadFromFile("font/minecraft_0.ttf")) {
    std::cerr << "Failed to load font!" << std::endl;
  }

  // Настройка кнопки Play
  playButton.rect.setSize(sf::Vector2f(200, 60));
  playButton.rect.setPosition(300, 300);
  playButton.rect.setFillColor(sf::Color(70, 70, 70));
  playButton.rect.setOutlineThickness(2);
  playButton.rect.setOutlineColor(sf::Color::White);

  playButton.text.setFont(font);
  playButton.text.setString("Play");
  playButton.text.setCharacterSize(30);
  playButton.text.setFillColor(sf::Color::White);
  playButton.text.setPosition(
    playButton.rect.getPosition().x + playButton.rect.getSize().x / 2 - playButton.text.getLocalBounds().width / 2,
    playButton.rect.getPosition().y + playButton.rect.getSize().y / 2 - playButton.text.getLocalBounds().height / 2 - 5
  );

  // Настройка кнопки Exit (аналогично)
  exitButton.rect = playButton.rect;
  exitButton.rect.setPosition(300, 400);

  exitButton.text.setFont(font);
  exitButton.text.setString("Exit");
  exitButton.text.setCharacterSize(30);
  exitButton.text.setFillColor(sf::Color::White);
  exitButton.text.setPosition(
    exitButton.rect.getPosition().x + exitButton.rect.getSize().x / 2 - exitButton.text.getLocalBounds().width / 2,
    exitButton.rect.getPosition().y + exitButton.rect.getSize().y / 2 - exitButton.text.getLocalBounds().height / 2 - 5
  );
}

void MainMenu::update(sf::Vector2f mousePos) {
  auto animateButton = [](MenuButton& button, sf::Vector2f mousePos) {
    if (button.rect.getGlobalBounds().contains(mousePos)) {
      button.rect.setScale(1.05f, 1.05f); // Увеличение на 5%
      button.text.setScale(1.05f, 1.05f);
    }
    else {
      button.rect.setScale(1.0f, 1.0f);
      button.text.setScale(1.0f, 1.0f);
    }
    };

  animateButton(playButton, mousePos);
  animateButton(exitButton, mousePos);
}

void MainMenu::draw(sf::RenderWindow& window) {
  window.draw(background);
  window.draw(playButton.rect);
  window.draw(playButton.text);
  window.draw(exitButton.rect);
  window.draw(exitButton.text);
}

bool MainMenu::isPlayClicked(sf::Vector2f mousePos) const {
  return playButton.rect.getGlobalBounds().contains(mousePos);
}

bool MainMenu::isExitClicked(sf::Vector2f mousePos) const {
  return exitButton.rect.getGlobalBounds().contains(mousePos);
}