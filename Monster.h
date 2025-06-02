#pragma once
#include "Entity.h"
#include <random>
#include <cmath>

class Player;
class LevelManager;

class Monster : public Entity {
public:
    Monster(float x, float y, float w, float h,
        std::string texturePath,
        Player* player,
        LevelManager* lvlMgr);
    virtual ~Monster() = default;

    void  setTargetPosition(const sf::Vector2f& target);

    int   damage = 0;
    int   reward = 0;
    float attackCooldown = 0.f;
    float currentCooldown = 0.f;

    Player* target = nullptr;
    sf::Vector2f targetPosition;

    enum class AIState { Waiting, Chasing, Returning };
    bool  updateAI(float dt);
    AIState getAIState() const { return aiState; }

protected:
    static constexpr float DETECT_RADIUS = 400.f;
    static constexpr float LOST_RADIUS = 500.f;
    static constexpr float WANDER_RADIUS = 120.f;
    static constexpr float MOVE_PHASE = 3.f;
    static constexpr float IDLE_PHASE = 3.f;

    AIState      aiState = AIState::Waiting;
    sf::Vector2f homePos;
    sf::Vector2f wanderTarget;
    bool  movePhase = false;
    float phaseTimer = 0.f;

    std::mt19937 rng{ std::random_device{}() };

    void chooseNewWanderTarget();

private:
    bool updateWaiting(float dt);
    bool updateReturning(float dt);
};
