#include "Skeleton.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

namespace {
    constexpr float PI = 3.14159265f;
}

Skeleton::Skeleton(float x, float y, float w, float h,
    const std::string& texturePath,
    Player* player, LevelManager* lvlMgr)
    : Monster(x, y, w, h, texturePath, player, lvlMgr)
{
    hp = 25;
    damage = 15;
    reward = 7;
    attackCooldown = 1.5f;
    speed = 0.05f;
    sprite.setOrigin(w / 2.f, h / 2.f);

    attackFrames = {
        sf::IntRect(95, 32, 30, 32),
        sf::IntRect(130, 32, 25, 62),
        sf::IntRect(159, 29, 35, 65),
        sf::IntRect(196, 32, 23, 62),
        
        sf::IntRect(111,160, 23, 32),
        sf::IntRect(138,128, 25, 61),
        sf::IntRect(167,128, 27, 61),
        sf::IntRect(197,128, 23, 61)
    };

    attackOrigins = {
        { attackFrames[0].width / 2.f, 16.f },
        { attackFrames[1].width / 2.f, 16.f },
        { attackFrames[2].width / 2.f, 16.f },
        { attackFrames[3].width / 2.f, 16.f },
        
        { attackFrames[4].width / 2.f, 15.f },
        { attackFrames[5].width / 2.f, 45.f },
        { attackFrames[6].width / 2.f, 45.f },
        { attackFrames[7].width / 2.f, 45.f }
    };

    moveDownFrames = {
        sf::IntRect(0 * 32, 1 * 32, 32, 32),
        sf::IntRect(1 * 32, 1 * 32, 32, 32)
    };
    moveDownLeftFrames = {
        sf::IntRect(0 * 32, 2 * 32, 32, 32),
        sf::IntRect(1 * 32, 2 * 32, 32, 32),
        sf::IntRect(2 * 32, 2 * 32, 32, 32),
        sf::IntRect(3 * 32, 2 * 32, 32, 32)
    };
    moveLeftFrames = {
        sf::IntRect(0 * 32, 3 * 32, 32, 32),
        sf::IntRect(1 * 32, 3 * 32, 32, 32),
        sf::IntRect(2 * 32, 3 * 32, 32, 32),
        sf::IntRect(3 * 32, 3 * 32, 32, 32)
    };
    moveUpLeftFrames = {
        sf::IntRect(0 * 32, 4 * 32, 32, 32),
        sf::IntRect(1 * 32, 4 * 32, 32, 32),
        sf::IntRect(2 * 32, 4 * 32, 32, 32),
        sf::IntRect(3 * 32, 4 * 32, 32, 32)
    };
    moveUpFrames = {
        sf::IntRect(0 * 32, 5 * 32, 32, 32),
        sf::IntRect(1 * 32, 5 * 32, 32, 32)
    };
}

void Skeleton::startDying() {
    if (isDying) return;
    isDying = true;
    canCollide = false;
    deathSprite = sprite;
    deathTimer = 0.0f;
}

void Skeleton::update(float dt)
{
    if (!life) return;

    if (isDying) {
        deathTimer += dt;
        if (deathTimer >= deathDuration) {
            life = false;
            target->addMoney(reward);
        }
        return;
    }

    updateAI(dt);

    for (auto& b : bones) {
        if (b->life) b->update(dt);
        if (b->life && b->checkCollision(target)) b->onCollision(target);
    }
    bones.erase(std::remove_if(bones.begin(), bones.end(),
        [](auto& b) { return !b->life; }),
        bones.end());

    setTargetPosition(target->getPosition());
    if (currentCooldown > 0.f)
        currentCooldown -= dt;

    updateAttack(dt);
    updateMovement(dt);

    interactWithMap();
    sprite.setPosition(x, y);
}

void Skeleton::updateMovement(float dt)
{
    if (isAttacking)
        return;

    AIState state = getAIState();

    if (state == AIState::Waiting) {
        if (movePhase) {
            float dirX, dirY;
            dirX = wanderTarget.x - x;
            dirY = wanderTarget.y - y;
            float distW = std::hypot(dirX, dirY);
            if (distW > 0.1f) {
                dirX /= distW;
                dirY /= distW;
            }

            moveAnimTimer += dt;
            if (moveAnimTimer >= moveAnimPeriod) {
                moveAnimTimer = 0.f;
                ++currentMoveFrame;
            }

            float ang = std::atan2(dirY, dirX) * 180.f / PI;
            if (ang < 0) ang += 360.f;

            bool isMirror = false;
            sf::IntRect walkRect;

            if (ang > 67.5f && ang <= 112.5f) {
                int cnt = static_cast<int>(moveDownFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownFrames[idx];
            }
            else if (ang > 22.5f && ang <= 67.5f) {
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
                isMirror = true;
            }
            else if (ang > 112.5f && ang <= 157.5f) {
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
            }
            else if (ang > 157.5f && ang <= 202.5f) {
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
            }
            else if (ang > 202.5f && ang <= 247.5f) {
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
            }
            else if (ang > 247.5f && ang <= 292.5f) {
                int cnt = static_cast<int>(moveUpFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpFrames[idx];
            }
            else if (ang > 292.5f && ang <= 337.5f) {
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
                isMirror = true;
            }
            else {
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
                isMirror = true;
            }

            sprite.setTextureRect(walkRect);
            sprite.setOrigin(16.f, 16.f);
            sprite.setScale(isMirror ? -1.f : 1.f, 1.f);
        }
        else {
            frameTime += dt;
            if (frameTime >= frameDuration) {
                frameTime = 0.f;
                currentFrame = 1.f - currentFrame;

                sprite.setTextureRect(
                    sf::IntRect(
                        static_cast<int>(currentFrame) * static_cast<int>(width),
                        0,
                        static_cast<int>(width),
                        static_cast<int>(height)
                    )
                );
                sprite.setOrigin(width / 2.f, height / 2.f);
            }
        }
        return;
    }

    if (state == AIState::Returning) {
        float deltaX = homePos.x - x;
        float deltaY = homePos.y - y;
        float distH = std::hypot(deltaX, deltaY);

        if (distH > 2.f) {
            moveAnimTimer += dt;
            if (moveAnimTimer >= moveAnimPeriod) {
                moveAnimTimer = 0.f;
                ++currentMoveFrame;
            }

            float dirX = deltaX / distH;
            float dirY = deltaY / distH;

            float ang = std::atan2(dirY, dirX) * 180.f / PI;
            if (ang < 0) ang += 360.f;

            bool isMirror = false;
            sf::IntRect walkRect;

            if (ang > 67.5f && ang <= 112.5f) {
                int cnt = static_cast<int>(moveDownFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownFrames[idx];
            }
            else if (ang > 22.5f && ang <= 67.5f) {
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
                isMirror = true;
            }
            else if (ang > 112.5f && ang <= 157.5f) {
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
            }
            else if (ang > 157.5f && ang <= 202.5f) {
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
            }
            else if (ang > 202.5f && ang <= 247.5f) {
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
            }
            else if (ang > 247.5f && ang <= 292.5f) {
                int cnt = static_cast<int>(moveUpFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpFrames[idx];
            }
            else if (ang > 292.5f && ang <= 337.5f) {
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
                isMirror = true;
            }
            else {
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
                isMirror = true;
            }

            sprite.setTextureRect(walkRect);
            sprite.setOrigin(16.f, 16.f);
            sprite.setScale(isMirror ? -1.f : 1.f, 1.f);
        }
        return;
    }

    float dxToPlayer = targetPosition.x - x;
    float dyToPlayer = targetPosition.y - y;
    float distToPlayer = std::hypot(dxToPlayer, dyToPlayer);

    if (distToPlayer > attackRange) {
        moveAnimTimer += dt;
        if (moveAnimTimer >= moveAnimPeriod) {
            moveAnimTimer = 0.f;
            ++currentMoveFrame;
        }

        float vx = dxToPlayer / distToPlayer;
        float vy = dyToPlayer / distToPlayer;
        x += vx * speed * dt * 1000.f;
        y += vy * speed * dt * 1000.f;

        float ang = std::atan2(vy, vx) * 180.f / PI;
        if (ang < 0) ang += 360.f;

        bool isMirror = false;
        sf::IntRect walkRect;

        if (ang > 67.5f && ang <= 112.5f) {
            int cnt = static_cast<int>(moveDownFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownFrames[idx];
        }
        else if (ang > 22.5f && ang <= 67.5f) {
            int cnt = static_cast<int>(moveDownLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownLeftFrames[idx];
            isMirror = true;
        }
        else if (ang > 112.5f && ang <= 157.5f) {
            int cnt = static_cast<int>(moveDownLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownLeftFrames[idx];
        }
        else if (ang > 157.5f && ang <= 202.5f) {
            int cnt = static_cast<int>(moveLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveLeftFrames[idx];
        }
        else if (ang > 202.5f && ang <= 247.5f) {
            int cnt = static_cast<int>(moveUpLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpLeftFrames[idx];
        }
        else if (ang > 247.5f && ang <= 292.5f) {
            int cnt = static_cast<int>(moveUpFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpFrames[idx];
        }
        else if (ang > 292.5f && ang <= 337.5f) {
            int cnt = static_cast<int>(moveUpLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpLeftFrames[idx];
            isMirror = true;
        }
        else {
            int cnt = static_cast<int>(moveLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveLeftFrames[idx];
            isMirror = true;
        }

        sprite.setTextureRect(walkRect);
        sprite.setOrigin(16.f, 16.f);
        sprite.setScale(isMirror ? -1.f : 1.f, 1.f);
        return;
    }

    frameTime += dt;
    if (frameTime >= frameDuration) {
        frameTime = 0.f;
        currentFrame = 1.f - currentFrame;
        sprite.setTextureRect(
            sf::IntRect(
                static_cast<int>(currentFrame) * static_cast<int>(width),
                0,
                static_cast<int>(width),
                static_cast<int>(height)
            )
        );
        sprite.setOrigin(width / 2.f, height / 2.f);
    }
}

void Skeleton::updateAttack(float dt)
{
    float dist = std::hypot(targetPosition.x - x, targetPosition.y - y);

    if (!isAttacking && dist <= attackRange && currentCooldown <= 0.f)
    {
        float vx = targetPosition.x - x;
        float vy = targetPosition.y - y;

        upperGroup = (vy < 0.f);
        mirror = upperGroup
            ? (vx < 0.f)
            : (vx > 0.f);

        isAttacking = true;
        boneFired = false;
        currentAttackStage = 0;
        stageTimer = attackWindup;
    }

    if (!isAttacking)
        return;

    stageTimer -= dt;
    if (stageTimer <= 0.f) {
        ++currentAttackStage;
        switch (currentAttackStage)
        {
        case 1: {
            stageTimer = attackStrike;
            if (!boneFired) {
                bones.emplace_back(std::make_shared<Bone>(
                    sf::Vector2f{ x, y },
                    targetPosition,
                    boneSpeed, boneDamage,
                    this,
                    levelManager
                ));
                boneFired = true;
            }
            break;
        }
        case 2:
            stageTimer = attackRecovery;
            break;
        default:
            isAttacking = false;
            currentCooldown = attackCooldown;
            return;
        }
    }

    int baseIndex = upperGroup ? 4 : 0;
    int localIdx;
    if (currentAttackStage == 0) {
        float p = 1.f - (stageTimer / attackWindup);
        localIdx = (p < 0.5f ? 0 : 1);
    }
    else if (currentAttackStage == 1) {
        localIdx = 2;
    }
    else {
        localIdx = 3;
    }

    int idx = baseIndex + localIdx;
    sprite.setTextureRect(attackFrames[idx]);
    sprite.setOrigin(attackOrigins[idx]);
    sprite.setScale(mirror ? -1.f : 1.f, 1.f);
}

void Skeleton::draw(sf::RenderWindow& window) const {
    if (!life) return;

    if (isDying) {
        sf::Sprite temp = deathSprite;
        float alpha = 1.0f - (deathTimer / deathDuration);
        temp.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
        window.draw(temp);
    }
    else {
        for (auto& b : bones) {
            b->draw(window);
        }
        Entity::draw(window);
    }
}
