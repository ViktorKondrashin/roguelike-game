#include "ProfileNewInput.h"

ProfileNewInput::ProfileNewInput(sf::Font& font)
  : fontRef(font)
{
  textDisplay.setFont(fontRef);
  textDisplay.setCharacterSize(28);
  textDisplay.setFillColor(sf::Color::White);
  textDisplay.setPosition(100.f, 200.f);
  textDisplay.setString("Enter name: ");
}

bool ProfileNewInput::handleTextEvent(const sf::Event& event) {
  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode == '\b') {
      if (!currentInput.empty()) {
        currentInput.pop_back();
      }
    }
    else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
      return !currentInput.empty();
    }
    else if (event.text.unicode < 128) {
      char entered = static_cast<char>(event.text.unicode);
      currentInput.push_back(entered);
    }
    textDisplay.setString("Enter name: " + currentInput);
  }
  return false;
}

void ProfileNewInput::draw(sf::RenderWindow& window) {
  window.draw(textDisplay);
}

const std::string& ProfileNewInput::getCurrentString() const {
  return currentInput;
}
