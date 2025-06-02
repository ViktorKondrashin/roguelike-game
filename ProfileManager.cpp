#include "ProfileManager.h"

ProfileManager::ProfileManager(const std::string& filename)
  : filename(filename) {
}

void ProfileManager::loadProfiles() {
  profiles.clear();
  std::ifstream in(filename);
  if (!in.is_open()) {
    return;
  }
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    std::istringstream iss(line);
    Profile p;
    iss >> p.name >> p.money >> p.maxHP >> p.strength >> p.speed;
    if (!p.name.empty()) {
      profiles.push_back(p);
    }
  }
  in.close();
}

void ProfileManager::saveProfiles() const {
  std::ofstream out(filename, std::ios::trunc);
  if (!out.is_open()) return;
  for (const auto& p : profiles) {
    out << p.name << " "
      << p.money << " "
      << p.maxHP << " "
      << p.strength << " "
      << p.speed << "\n";
  }
  out.close();
}

const std::vector<Profile>& ProfileManager::getProfiles() const {
  return profiles;
}

void ProfileManager::addProfile(const Profile& p) {
  profiles.push_back(p);
}

std::optional<Profile> ProfileManager::getProfile(const std::string& name) const {
  for (const auto& p : profiles) {
    if (p.name == name) {
      return p;
    }
  }
  return std::nullopt;
}
