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
    // ----------------- логика движения / атаки -----------------
    void updateMovement(float dt);
    void updateAttack(float dt);

    // ----------------- базовые параметры -----------------
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
    bool  upperGroup = false;  // true → атака вверх
    bool  mirror = false;  // зеркалить при атаке

    // --- анимация броска (как раньше) ---
    std::vector<sf::IntRect>   attackFrames;   // 8 IntRect, индексы: { 0..3 = бросок ↓ }, { 4..7 = бросок ↑ }
    std::vector<sf::Vector2f>  attackOrigins;  // для выравнивания «ступней»

    // ----------------- поля для анимации ходьбы -----------------
    // В этой таблице хранятся IntRect’ы, которые вырезаются из общей текстуры:
    //  - ходьба ↓  (2 кадра), 
    //  - ходьба ↙ (4 кадра), 
    //  - ходьба ← (4 кадра), 
    //  - ходьба ↖ (4 кадра), 
    //  - ходьба ↑  (2 кадра).
    // Правые направления →/↘/↗ мы будем рисовать зеркально, взяв левый набор и установив setScale(-1,1).
    std::vector<sf::IntRect> moveDownFrames;     // (кадры 4,5)
    std::vector<sf::IntRect> moveDownLeftFrames; // (кадры 8,9,10,11)
    std::vector<sf::IntRect> moveLeftFrames;     // (кадры 12,13,14,15)
    std::vector<sf::IntRect> moveUpLeftFrames;   // (кадры 16,17,18,19)
    std::vector<sf::IntRect> moveUpFrames;       // (кадры 20,21)

    // Таймер и индекс для переключения шагов при ходьбе:
    float moveAnimTimer = 0.f;
    float moveAnimPeriod = 0.18f;  // задержка между переключением кадров (ms)
    int   currentMoveFrame = 0;      // текущее значение индекса в массиве конкретного направления

    // ----------------- прочее для «покачивания» idling -----------------
    float frameTime = 0.f;     // для 2-кадровой idle-анимации (ожидание)
    float currentFrame = 0.f;     // 0 или 1
    const float frameDuration = 0.25f; // период «покачивания» (сек)

    // ----------------- кости -----------------
    std::vector<std::shared_ptr<Bone>> bones;

    // ---------- параметры «растворения» (как у Ghost) ----------
    sf::Sprite deathSprite;
    float      deathTimer = 0.0f;
    const float deathDuration = 0.5f;

    // если хотим, можно явно объявить override:
    void startDying() override;
};
