#pragma once
#include "Monster.h"
#include "LevelManager.h"
#include <array>
#include <SFML/Graphics.hpp>

/**
 * Призрак — 8-направленный моб без коллизии.
 * 5 базовых строк спрайтов (↓ ↙ ← ↖ ↑); правые направления получаем scale.x = -1.
 */
class Ghost : public Monster {
public:
    Ghost(float x, float y, float w, float h,
        const std::string& texturePath,
        Player* player,
        LevelManager* lvlMgr);

    /* --- цикл --- */
    void update(float dt) override;
    void draw(sf::RenderWindow& w) const override;
    void startDying() override;

    /* --- коллизии --- */
    bool checkCollision(Entity* other) override;     // только для пуль
    void onCollision(Entity* other) override;

private:
    float prevX = 0.f, prevY = 0.f;   // положение на прошлом кадре
    float moveX = 0.f, moveY = 0.f;  // сдвиг текущего кадра

    // флаг и таймер, отвечающие за паузу после атаки
    bool  postAttackIdle = false;
    float postAttackIdleTimer = 0.f;
    const float postAttackIdleDuration = 1.0f; // длительность паузы в секундах

    //  в private-секции
    void applyProjectileHit(Entity* other);

    /* ---------- ИИ ---------- */
    void updateChase(float dt);
    void updateDash(float dt);

    /* ---------- анимация ---------- */
    enum Row : int { DOWN, DL, LEFT, UL, UP };               // 0‥4
    void  updateAnimation(float dt);
    Row   angleToRow(float ang, bool& mirror) const;         // возвращает mirror

    std::array<std::array<sf::IntRect, 3>, 5> frames{};        // 5×3
    int   animCol = 0;                                     // 0/1 шаг, 2 удар
    float animTimer = 0.f;
    const float animPeriod = 0.18f;

    /* ---------- параметры поведения ---------- */
    const float baseSpeed = 0.1f;   // px/мс
    const float dashSpeed = 0.35f;
    const float dashDur = 0.45f;
    const float waitDur = 0.35f;
    const float visionR = 120.f;   // радиус «зрения»

    bool  isWaiting = false;
    bool  isDashing = false;
    bool  hitThisDash = false;
    float waitTimer = 0.f;
    float dashTimer = 0.f;
    float dashDirX = 0.f, dashDirY = 0.f;

    bool         hasLastSeen = false;
    sf::Vector2f lastSeenPos;

    /* ---------- растворение ---------- */
    sf::Sprite deathSprite;
    float deathTimer = 0.f;
    const float deathDur = 0.40f;
};
