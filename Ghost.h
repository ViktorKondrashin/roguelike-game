#pragma once
#include "Monster.h"
#include "LevelManager.h"
#include <array>
#include <SFML/Graphics.hpp>

class Ghost : public Monster {
public:
    Ghost(float x, float y, float w, float h,
        const std::string& texturePath,
        Player* player,
        LevelManager* lvlMgr);

    void update(float dt) override;
    void draw(sf::RenderWindow& w) const override;
    void startDying() override;

    bool checkCollision(Entity* other) override;
    void onCollision(Entity* other) override;

private:
    float prevX = 0.f, prevY = 0.f;
    float moveX = 0.f, moveY = 0.f;

    bool  postAttackIdle = false;
    float postAttackIdleTimer = 0.f;
    const float postAttackIdleDuration = 1.0f;

    void applyProjectileHit(Entity* other);

    void updateChase(float dt);
    void updateDash(float dt);

    enum Row : int { DOWN, DL, LEFT, UL, UP };
    void  updateAnimation(float dt);
    Row   angleToRow(float ang, bool& mirror) const;

    std::array<std::array<sf::IntRect, 3>, 5> frames{};
    int   animCol = 0;
    float animTimer = 0.f;
    const float animPeriod = 0.18f;

    const float baseSpeed = 0.1f;
    const float dashSpeed = 0.35f;
    const float dashDur = 0.45f;
    const float waitDur = 0.35f;
    const float visionR = 120.f;

    bool  isWaiting = false;
    bool  isDashing = false;
    bool  hitThisDash = false;
    float waitTimer = 0.f;
    float dashTimer = 0.f;
    float dashDirX = 0.f, dashDirY = 0.f;

    bool         hasLastSeen = false;
    sf::Vector2f lastSeenPos;

    sf::Sprite deathSprite;
    float deathTimer = 0.f;
    const float deathDur = 0.40f;
};
