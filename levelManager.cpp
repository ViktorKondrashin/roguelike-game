#include "LevelManager.h"
#include <cstdlib>
#include <ctime>

sf::IntRect LevelManager::getPathVariant(int x, int y) const
{
  bool left = (x > 0) && (getTile(x - 1, y) == '=');
  bool right = (x < width_ - 1) && (getTile(x + 1, y) == '=');
  bool top = (y > 0) && (getTile(x, y - 1) == '=');
  bool bot = (y < height_ - 1) && (getTile(x, y + 1) == '=');

  if (top && left)  return { 0, 128, 32, 32 };  // ?
  if (top && right) return { 128, 128, 32, 32 };  // ?
  if (bot && left) return { 0, 256, 32, 32 }; // ?
  if (bot && right) return { 128, 256, 32, 32 }; // ?

  // Границы
  if (left) return { 0, 192, 32, 32 };   // ?
  if (right) return { 128, 192, 32, 32 };  // ?
  if (top) return { 64, 128, 32, 32 };   // ?
  if (bot) return { 64, 256, 32, 32 };// ?

  // Одиночная тропинка
  return { 64, 192, 32, 32 };
}

LevelManager::LevelManager(const std::vector<std::string>& levelData)
{
  if (!levelData.empty()) {
    levelData_ = levelData;
    height_ = levelData_.size();
    width_ = height_ > 0 ? levelData_[0].size() : 0;
  }
  else {
    levelData_ = {
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwww                      wwwww",
      "wwwww   =    fff    =      wwwww",
      "wwwww  fff  =   =  fff     wwwww",
      "wwwww =   =======    =     wwwww",
      "wwwww  fff    ==    fff    wwwww",
      "wwwww         ==           wwwww",
      "wwwww         =            wwwww",
      "wwwww   =    fff    =      wwwww",
      "wwwww  fff  =   =  fff     wwwww",
      "wwwww =   ========    =    wwwww",
      "wwwww  fff    =     fff    wwwww",
      "wwwww                      wwwww",
      "wwwww         =            wwwww",
      "wwwww   =    fff    =      wwwww",
      "wwwww  fff  =   =  fff     wwwww",
      "wwwww =   ========    =    wwwww",
      "wwwww  fff    =     fff    wwwww",
      "wwwww                      wwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
      "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
    };
    height_ = levelData_.size();
    width_ = levelData_[0].size();
  }
}



char LevelManager::getTile(int x, int y) const
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) return '0';
  return levelData_[y][x];
}

void LevelManager::setTile(int x, int y, char tile)
{
  if (x >= 0 && y >= 0 && x < width_ && y < height_) {
    levelData_[y][x] = tile;
  }
}

bool LevelManager::loadTileset(const std::string& path)
{
  if (!tileset_.loadFromFile(path)) return false;

  tileTextures_ = {
      {'0', {0, 0, 32, 32}},  
      {'w',{160,128,32,32}},
      {' ', {0, 32, 32, 32}},  
      {'=', {64, 192, 32, 32}},
      {'f', {0, 96, 32, 32}},  
      {'s', {0, 128, 32, 32}}, 
      {'l', {0, 160, 32, 32}}  
  };
  return true;
}

void LevelManager::drawLevel(sf::RenderWindow& window) const
{
  sf::Sprite tileSprite(tileset_);

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      char tile = getTile(x, y);
      tileSprite.setPosition(x * 32, y * 32);

      if (tile == '=') {
        tileSprite.setTextureRect(getPathVariant(x, y));
      }

      else {
        tileSprite.setTextureRect(tileTextures_.at(tile));
      }

      window.draw(tileSprite);
    }
  }
}

bool LevelManager::isWall(int x, int y) const
{
  return getTile(x,y)=='0';
}

bool LevelManager::isWalkable(int x, int y) const
{
  char tile = getTile(x, y);
  return tile == ' ' || tile == 's' || tile == '=' || tile == 'f';
}

bool LevelManager::isShootable(int x, int y) const
{
  return !isWall(x, y);
}

std::pair<int, int> LevelManager::findNearbyWalkable(int x, int y, int radius) const
{
  for (int dy = -radius; dy <= radius; ++dy) {
    for (int dx = -radius; dx <= radius; ++dx) {
      int nx = x + dx;
      int ny = y + dy;
      if (isSpawnable(nx, ny)) {
        return { nx, ny };
      }
    }
  }
  return { -1, -1 }; // Если не нашли
}
