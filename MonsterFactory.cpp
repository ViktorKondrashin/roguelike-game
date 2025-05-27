#include "MonsterFactory.h"

std::unique_ptr<Monster> MonsterFactory::create(
  const std::string& type,
  float x, float y,
  float w, float h,
  const std::string& texturePath,
  Player* player)
{
  auto it = creators.find(type);
  if (it != creators.end()) {
    return it->second(x, y, w, h, texturePath, player);
  }
  return nullptr;
}
