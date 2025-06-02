#pragma once
#include "Monster.h"
#include "Slime.h"
#include "Ghost.h"
#include "Skeleton.h"
#include <memory>
#include <unordered_map>
#include <functional>

class MonsterFactory {
private:
  static inline std::unordered_map<std::string,
    std::function<std::unique_ptr<Monster>(float, float, float, float,
      const std::string&, Player*, LevelManager*)>> creators;

public: 
  template<typename T>
  static void registerType(const std::string& name) {
    creators[name] = [](float x, float y, float w, float h,
      const std::string& path, Player* p, LevelManager* lvlMgr) {
        return std::make_unique<T>(x, y, w, h, path, p, lvlMgr);
      };
  }

  static std::unique_ptr<Monster> create(
    const std::string& type,
    float x, float y,
    float w, float h,
    const std::string& texturePath,
    Player* player, LevelManager* lvlMgr
  );
};