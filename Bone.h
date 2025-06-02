#pragma once
#include "Entity.h"
#include "SFML/Graphics.hpp"

class Bone : public Entity {
private:
    sf::Vector2f velocity;
    float  damage;
    float  spawnTimer;
    float  rotationSpeed;

    sf::Vector2f startPosition;
    static constexpr float maxDistance = 450.f;

public:
    Bone(const sf::Vector2f& position,
        const sf::Vector2f& target,
        float speed,
        float damage,
        Entity* shooter,
        LevelManager* lvlMgr);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const;
    bool isOutOfScreen(const sf::RenderWindow& window) const;
    void explode();

    void onCollision(Entity* other) override;
    bool checkCollision(Entity* other) override;
};
