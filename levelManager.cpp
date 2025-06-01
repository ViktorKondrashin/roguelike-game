#include "LevelManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

sf::IntRect LevelManager::getPathVariant(char tileType, int x, int y) const
{
  switch (tileType) {
  case '=': {
    bool left = (x > 0) && (getTile(x - 1, y) == '=');
    bool right = (x < width_ - 1) && (getTile(x + 1, y) == '=');
    bool top = (y > 0) && (getTile(x, y - 1) == '=');
    bool bot = (y < height_ - 1) && (getTile(x, y + 1) == '=');
    bool topLeft = (x > 0 && y > 0) && (getTile(x - 1, y - 1) == '=');
    bool topRight = (x < width_ - 1 && y > 0) && (getTile(x + 1, y - 1) == '=');
    bool bottomLeft = (x > 0 && y < height_ - 1) && (getTile(x - 1, y + 1) == '=');
    bool bottomRight = (x < width_ - 1 && y < height_ - 1) && (getTile(x + 1, y + 1) == '=');

    if (!top && !left && right && bot) return { 0, 128, 32, 32 };
    if (!top && !right && left && bot) return { 128, 128, 32, 32 };
    if (!bot && !left && right && top) return { 0, 256, 32, 32 };
    if (!bot && !right && left && top) return { 128, 256, 32, 32 };

    if (top && left && !topLeft)  return { 96, 224, 32, 32 };
    if (top && right && !topRight) return { 32, 224, 32, 32 };
    if (bot && left && !bottomLeft)    return { 96, 160, 32, 32 };
    if (bot && right && !bottomRight)  return { 32, 160, 32, 32 };

    if (left && right && top && bot) {
      int variant = (x + y) % 5;
      switch (variant) {
      case 0: return { 0, 288, 32, 32 };
      case 1: return { 32, 288, 32, 32 };
      case 2: return { 64, 288, 32, 32 };
      case 3: return { 96, 288, 32, 32 };
      case 4: return { 128, 288, 32, 32 };
      }
    }

    if (!bot && top) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 32, 256, 32, 32 };
      case 1: return { 64, 256, 32, 32 };
      case 2: return { 96, 256, 32, 32 };
      case 3: return { 64, 160, 32, 32 };
      }
    }

    if (!top && bot) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 32, 128, 32, 32 };
      case 1: return { 64, 128, 32, 32 };
      case 2: return { 96, 128, 32, 32 };
      case 3: return { 64, 224, 32, 32 };
      }
    }

    if (left && !right) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 128, 160, 32, 32 };
      case 1: return { 128, 192, 32, 32 };
      case 2: return { 128, 224, 32, 32 };
      case 3: return { 32, 192, 32, 32 };
      }
    }

    if (right && !left) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 0, 192, 32, 32 };
      case 1: return { 0, 224, 32, 32 };
      case 2: return { 0, 160, 32, 32 };
      case 3: return { 96, 192, 32, 32 };
      }
    }

    return { 64, 192, 32, 32 };
  }

  case 'w': {
    bool left = (x > 0) && (getTile(x - 1, y) == 'w');
    bool right = (x < width_ - 1) && (getTile(x + 1, y) == 'w');
    bool top = (y > 0) && (getTile(x, y - 1) == 'w');
    bool bot = (y < height_ - 1) && (getTile(x, y + 1) == 'w');
    bool topLeft = (x > 0 && y > 0) && (getTile(x - 1, y - 1) == 'w');
    bool topRight = (x < width_ - 1 && y > 0) && (getTile(x + 1, y - 1) == 'w');
    bool bottomLeft = (x > 0 && y < height_ - 1) && (getTile(x - 1, y + 1) == 'w');
    bool bottomRight = (x < width_ - 1 && y < height_ - 1) && (getTile(x + 1, y + 1) == 'w');

    if (!top && !left && right && bot) return { 0, 320, 32, 32 };
    if (!top && !right && left && bot) return { 128, 320, 32, 32 };
    if (!bot && !left && right && top) return { 0, 448, 32, 32 };
    if (!bot && !right && left && top) return { 128, 448, 32, 32 };

    if (top && left && !topLeft)          return { 96, 416, 32, 32 };
    if (top && right && !topRight)        return { 32, 416, 32, 32 };
    if (bot && left && !bottomLeft)    return { 96, 352, 32, 32 };
    if (bot && right && !bottomRight)  return { 32, 352, 32, 32 };

    if (left && right && top && bot) {
      int variant = (x + y) % 5;
      switch (variant) {
      case 0: return { 0, 480, 32, 32 };
      case 1: return { 32, 480, 32, 32 };
      case 2: return { 64, 480, 32, 32 };
      case 3: return { 96, 480, 32, 32 };
      case 4: return { 128, 480, 32, 32 };
      }
    }

    if (!bot && top) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 32, 448, 32, 32 };
      case 1: return { 64, 448, 32, 32 };
      case 2: return { 96, 448, 32, 32 };
      case 3: return { 64, 352, 32, 32 };
      }
    }

    if (!top && bot) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 32, 320, 32, 32 };
      case 1: return { 64, 320, 32, 32 };
      case 2: return { 96, 320, 32, 32 };
      case 3: return { 64, 416, 32, 32 };
      }
    }

    if (left && !right) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 128, 352, 32, 32 };
      case 1: return { 128, 384, 32, 32 };
      case 2: return { 128, 416, 32, 32 };
      case 3: return { 32, 384, 32, 32 };
      }
    }

    if (right && !left) {
      int variant = (x + y) % 4;
      switch (variant) {
      case 0: return { 0, 352, 32, 32 };
      case 1: return { 0, 384, 32, 32 };
      case 2: return { 0, 416, 32, 32 };
      case 3: return { 96, 384, 32, 32 };
      }
    }

    return { 64, 384, 32, 32 };
  }

  case 'b': {
    bool left = (x > 0) && (getTile(x - 1, y) == 'b');
    bool right = (x < width_ - 1) && (getTile(x + 1, y) == 'b');
    bool top = (y > 0) && (getTile(x, y - 1) == 'b');
    bool bottom = (y < height_ - 1) && (getTile(x, y + 1) == 'b');
    bool topLeft = (x > 0 && y > 0) && (getTile(x - 1, y - 1) == 'b');
    bool topRight = (x < width_ - 1 && y > 0) && (getTile(x + 1, y - 1) == 'b');
    bool bottomLeft = (x > 0 && y < height_ - 1) && (getTile(x - 1, y + 1) == 'b');
    bool bottomRight = (x < width_ - 1 && y < height_ - 1) && (getTile(x + 1, y + 1) == 'b');

    if (!top && !left && right && bottom)  return { 256, 0, 32, 32 };
    if (!top && !right && left && bottom) return { 352, 0, 32, 32 };
    if (!bottom && !left && right && top) return { 256, 64, 32, 32 };
    if (!bottom && !right && left && top) return { 352, 64, 32, 32 };

    if (top && left && !topLeft)          return { 288, 160, 32, 32 };
    if (top && right && !topRight)        return { 256, 160, 32, 32 };
    if (bottom && left && !bottomLeft)    return { 288, 128, 32, 32 };
    if (bottom && right && !bottomRight)  return { 256, 128, 32, 32 };

    if (!left && right)  return { 256, 32, 32, 32 };
    if (!right && left) return { 352, 32, 32, 32 };

    if (!top && bottom) {
      int variant = (x + y) % 2;
      switch (variant) {
      case 0: return { 288, 0, 32, 32 };
      case 1: return { 320, 0, 32, 32 };
      }
    }

    if (!bottom && top) {
      int variant = (x + y) % 2;
      switch (variant) {
      case 0: return { 288, 64, 32, 32 };
      case 1: return { 320, 64, 32, 32 };
      }
    }

    if (left && right && top && bottom) {
      int variant = (x + y) % 2;
      switch (variant) {
      case 0: return { 288, 32, 32, 32 };
      case 1: return { 320, 32, 32, 32 };
      }
    }

    if (!left && !right && top && bottom) {
      int variant = (x + y) % 2;
      switch (variant) {
      case 0: return { 224, 128, 32, 32 };
      case 1: return { 224, 160, 32, 32 };
      }
    }
    if (!left && !right && !top && bottom)
      return { 196, 128, 32, 32 };
    if (!left && !right && top && !bottom)
      return { 196, 160, 32, 32 };
    if (!left && right && top && !bottom)
      return { 256, 96, 32, 32 };
    if (left && !right && top && !bottom)
      return { 288, 96, 32, 32 };
    if (!left && right && top && !bottom)
      return { 320, 96, 32, 32 };
    if (left && !right && top && !bottom)
      return { 352, 96, 32, 32 };
  }

  default:
    return tileTextures_.at(tileType);
  }
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
      {'b',{320, 32, 32, 32}},
      {' ', {0, 32, 32, 32}},  
      {'=', {64, 192, 32, 32}},
      {'f', {0, 96, 32, 32}},  
      {'s', {0, 128, 32, 32}}, 
      {'l', {0, 160, 32, 32}}  
  };
  randomTileVariants_[' '] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 32, 32, 32},
        {64, 32, 32, 32},
        {96, 32, 32, 32},
        {0, 64, 32, 32},
        {32, 64, 32, 32},
        {64, 64, 32, 32},
        {96, 64, 32, 32},
        {0, 96, 32, 32},
        {32, 96, 32, 32},
        {64, 96, 32, 32},
        {96, 96, 32, 32}
        
  };
  randomTileVariants_['f'] = {
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32},
        {128, 32, 32, 32},
        {160, 32, 32, 32},
        {192, 32, 32, 32},
        {224, 32, 32, 32},
        {128, 64, 32, 32},
        {160, 64, 32, 32},
        {192, 64, 32, 32},
        {224, 64, 32, 32},
        {128, 96, 32, 32},
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
  return tileTextures_.at(tileType);
}

void LevelManager::drawLevel(sf::RenderWindow& window) const {
  sf::Sprite tileSprite(tileset_);
  static std::map<std::pair<int, int>, sf::IntRect> tileCache;

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      char tile = getTile(x, y);
      tileSprite.setPosition(x * 32, y * 32);

      if (tile == '=' || tile == 'w' || tile == 'b') {
        tileSprite.setTextureRect(getPathVariant(tile, x, y));
      }
      else if (tile == ' ' || tile == 'f') {
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
  return { -1, -1 };
}
