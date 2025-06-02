#include "Monster.h"
#include "Player.h"
#include "LevelManager.h"
#include <random>
#include <cmath>

/*======================== ctor: запоминаем homePos ======================*/
Monster::Monster(float x,float y,float w,float h,
                 std::string texPath,Player* p,LevelManager* lvl)
  : Entity(x,y,w,h,texPath,lvl), damage(0), reward(0),
    attackCooldown(0), currentCooldown(0), target(p)
{
    homePos = { x, y };                 // ⬅️  ТОЧКА ОЖИДАНИЯ
    setTargetPosition(p->getPosition());
}

void Monster::setTargetPosition(const sf::Vector2f& pos)
{
    targetPosition = pos;
}

/*===================== вспом. точка блуждания ===========================*/
void Monster::chooseNewWanderTarget()
{
    std::uniform_real_distribution<float> angDist(0.f, 6.2831853f);
    std::uniform_real_distribution<float> rDist(20.f, WANDER_RADIUS);

    for (int tries = 0; tries < 10; ++tries) {          // ищем проходимую
        float ang = angDist(rng);
        float rad = rDist(rng);
        sf::Vector2f p = homePos + sf::Vector2f(std::cos(ang)*rad,
                                                std::sin(ang)*rad);
        int tx = static_cast<int>(p.x / 32);
        int ty = static_cast<int>(p.y / 32);
        if (!levelManager || levelManager->isWalkable(tx,ty)) {
            wanderTarget = p;
            return;
        }
    }
    wanderTarget = homePos;           // fallback
}

/*====================== Waiting-фаза (бродим) ===========================*/
bool Monster::updateWaiting(float dt)
{
    float distToPl = std::hypot(target->x - x, target->y - y);
    if (distToPl <= DETECT_RADIUS) { aiState = AIState::Chasing; return true; }

    phaseTimer -= dt;

    if (movePhase) {                  // 3 с двигаемся
        sf::Vector2f dir = wanderTarget - sf::Vector2f(x, y);
        float d = std::hypot(dir.x, dir.y);
        if (d > 2.f) { dir /= d; x += dir.x*speed*dt*1000; y += dir.y*speed*dt*1000; }
        if (d <= 2.f || phaseTimer <= 0.f) {
            movePhase   = false;
            phaseTimer  = IDLE_PHASE;
        }
    }
    else {                            // 3 с стоим
        if (phaseTimer <= 0.f) {
            movePhase  = true;
            phaseTimer = MOVE_PHASE;
            chooseNewWanderTarget();
        }
    }
    return false;                     // преследовать НЕ надо
}

/*===================== Returning-фаза (домой) ===========================*/
bool Monster::updateReturning(float dt)
{
    float distToPl = std::hypot(target->x - x, target->y - y);
    if (distToPl <= DETECT_RADIUS) { aiState = AIState::Chasing; return true; }

    sf::Vector2f dir = homePos - sf::Vector2f(x, y);
    float d = std::hypot(dir.x, dir.y);
    if (d > 2.f) { dir /= d; x += dir.x*speed*dt*1000; y += dir.y*speed*dt*1000; }
    else {                             // дошли
        aiState   = AIState::Waiting;
        movePhase = false;
        phaseTimer=0.f;
    }
    return false;
}

/*====================== общий апдейт AI =================================*/
bool Monster::updateAI(float dt)
{
    switch (aiState)
    {
        case AIState::Waiting:    return updateWaiting(dt);
        case AIState::Chasing: {
            float dist = std::hypot(target->x - x, target->y - y);
            if (dist > LOST_RADIUS) { aiState = AIState::Returning; return updateReturning(dt); }
            return true;          // продолжаем обычное поведение
        }
        case AIState::Returning:  return updateReturning(dt);
    }
    return true;
}
