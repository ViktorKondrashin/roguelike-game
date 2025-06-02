#pragma once
#include "Monster.h"
#include "Bone.h"
#include <vector>
#include <memory>
#include <cmath>

class Skeleton : public Monster {
public:
    Skeleton(float x, float y, float w, float h,
        const std::string& texturePath,
        Player* player, LevelManager* lvlMgr);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

private:
    void updateMovement(float dt);
    void updateAttack(float dt);

    float attackRange = 200.f;
    const float attackWindup = 0.25f;
    const float attackStrike = 0.25f;
    const float attackRecovery = 0.25f;
    float boneSpeed = 180.f;
    int   boneDamage = 15;
    bool  isAttacking = false;
    bool  boneFired = false;
    int   currentAttackStage = 0;
    float stageTimer = 0.f;
    bool  upperGroup = false;
    bool  mirror = false;

    std::vector<sf::IntRect>   attackFrames;
    std::vector<sf::Vector2f>  attackOrigins;

    std::vector<sf::IntRect> moveDownFrames;
    std::vector<sf::IntRect> moveDownLeftFrames;
    std::vector<sf::IntRect> moveLeftFrames;
    std::vector<sf::IntRect> moveUpLeftFrames;
    std::vector<sf::IntRect> moveUpFrames;

    float moveAnimTimer = 0.f;
    float moveAnimPeriod = 0.18f;
    int   currentMoveFrame = 0;

    float frameTime = 0.f;
    float currentFrame = 0.f;
    const float frameDuration = 0.25f;

    std::vector<std::shared_ptr<Bone>> bones;

    sf::Sprite deathSprite;
    float      deathTimer = 0.0f;
    const float deathDuration = 0.5f;

    void startDying() override;
};
