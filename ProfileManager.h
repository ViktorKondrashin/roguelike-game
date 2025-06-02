#pragma once
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <sstream>

struct Profile {
  std::string name;
  int money;
  int maxHP;
  int strength;  // будем интерпретировать как «дополнительный урон»
  float speed;   // множитель скорости (1.0 = базовая)
};

class ProfileManager {
public:
  ProfileManager(const std::string& filename);

  // Загружает все профиль из файла в вектор profiles
  void loadProfiles();

  // Сохраняет текущий список profiles в файл (перезапись)
  void saveProfiles() const;

  // Возвращает константную ссылку на все профили
  const std::vector<Profile>& getProfiles() const;

  // Добавить новый профиль (не сохраняет сразу в файл!)
  void addProfile(const Profile& p);

  // Поиск профиля по имени
  std::optional<Profile> getProfile(const std::string& name) const;

private:
  std::string filename;
  std::vector<Profile> profiles;
};
