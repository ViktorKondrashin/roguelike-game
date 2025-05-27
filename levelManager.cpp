#include "LevelManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

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

bool LevelManager::loadFromFile(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open level file: " << filePath << std::endl;
    return false;
  }

  levelData_.clear();
  std::string line;

  while (std::getline(file, line)) {
    if (!line.empty()) {
      levelData_.push_back(line);
    }
  }

  if (levelData_.empty()) {
    std::cerr << "Level file is empty!" << std::endl;
    return false;
  }

  height_ = levelData_.size();
  width_ = levelData_[0].size();

  // Проверка, что все строки одинаковой длины
  for (const auto& row : levelData_) {
    if (row.size() != width_) {
      std::cerr << "Inconsistent level width!" << std::endl;
      return false;
    }
  }

  return true;
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
      {'b',{340, 192, 32, 32}},
      {' ', {0, 32, 32, 32}},  
      {'=', {64, 192, 32, 32}},
      {'f', {0, 96, 32, 32}},  
      {'s', {0, 128, 32, 32}}, 
      {'l', {0, 160, 32, 32}}  
  };
  randomTileVariants_[' '] = {
        {0, 0, 32, 32},   // row 1
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {0, 32, 32, 32},    // row 2
        {32, 32, 32, 32},
        {64, 32, 32, 32},
        {96, 32, 32, 32},
        {0, 64, 32, 32},    // row 3
        {32, 64, 32, 32},
        {64, 64, 32, 32},
        {96, 64, 32, 32},
        {0, 96, 32, 32},    // row 4
        {32, 96, 32, 32},
        {64, 96, 32, 32},
        {96, 96, 32, 32}
        
  };
  randomTileVariants_['f'] = {
        {128, 0, 32, 32},   // row 1
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32},
        {128, 32, 32, 32},    // row 2
        {160, 32, 32, 32},
        {192, 32, 32, 32},
        {224, 32, 32, 32},
        {128, 64, 32, 32},    // row 3
        {160, 64, 32, 32},
        {192, 64, 32, 32},
        {224, 64, 32, 32},
        {128, 96, 32, 32},    // row 4
        {160, 96, 32, 32},
        {192, 96, 32, 32},
        {224, 96, 32, 32}
  };
  return true;
}

sf::IntRect LevelManager::getRandomTileVariant(char tileType) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  if (randomTileVariants_.count(tileType)) {
    const auto& variants = randomTileVariants_.at(tileType);
    std::uniform_int_distribution<> distr(0, variants.size() - 1);
    return variants[distr(gen)];
  }
  return tileTextures_.at(tileType); // Возвращаем базовый вариант, если нет случайных
}

void LevelManager::drawLevel(sf::RenderWindow& window) const {
  sf::Sprite tileSprite(tileset_);
  static std::map<std::pair<int, int>, sf::IntRect> tileCache; // Кэш для постоянства

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      char tile = getTile(x, y);
      tileSprite.setPosition(x * 32, y * 32);

      if (tile == '=') {
        tileSprite.setTextureRect(getPathVariant(x, y));
      }
      else if (tile == ' ' || tile == 'f') {
        // Используем кэш, чтобы тайлы не менялись каждый кадр
        auto pos = std::make_pair(x, y);
        if (!tileCache.count(pos)) {
          tileCache[pos] = getRandomTileVariant(tile);
        }
        tileSprite.setTextureRect(tileCache[pos]);
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
