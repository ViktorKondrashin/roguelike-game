#include "WaveLoader.h"
#include <fstream>
#include <sstream>

std::vector<MonsterWave> WaveLoader::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<MonsterWave> wave;
    std::string line;

    while (std::getline(file, line)) {
        // Пропускаем комментарии и пустые строки
        if (line.empty() || line.find("//") == 0) continue;

        // Парсим строку вида (2,1):SLIME
        int x, y;
        char discard;
        std::string type;

        std::istringstream iss(line);
        if (iss >> discard >> x >> discard >> y >> discard >> discard >> type) {
            wave.push_back({x, y, type});
        }
    }

    return wave;
}