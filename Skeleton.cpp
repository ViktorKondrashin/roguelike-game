#include "Skeleton.h"
#include "Player.h"
#include <cmath>
#include <algorithm>

/* Вспомогательная константа для углов (фундаментально не меняется) */
namespace {
    constexpr float PI = 3.14159265f;
}

/****************************  ctor  *************************************/
Skeleton::Skeleton(float x, float y, float w, float h,
    const std::string& texturePath,
    Player* player, LevelManager* lvlMgr)
    : Monster(x, y, w, h, texturePath, player, lvlMgr)
{
    /* базовые характеристики */
    hp = 25;
    damage = 15;
    reward = 7;
    attackCooldown = 1.5f;
    speed = 0.05f;
    sprite.setOrigin(w / 2.f, h / 2.f);

    // --------------------------------------------
    // 1) Заполняем attackFrames (8 кадров броска):
    //    Индексы 0…3 = «бросок ↓» (кадры 7–10 вашего png),
    //    Индексы 4…7 = «бросок ↑» (кадры 25–28 вашего png).
    attackFrames = {
        // ↓ (вниз/влево/↙)
        sf::IntRect(95, 32, 30, 32),  // индекс 0  (кадр 7)
        sf::IntRect(130, 32, 25, 62),  // индекс 1  (кадр 8)
        sf::IntRect(159, 29, 35, 65),  // индекс 2  (кадр 9)
        sf::IntRect(196, 32, 23, 62),  // индекс 3  (кадр 10)
        // ↑ (вверх/↗/↖)
        sf::IntRect(111,160, 23, 32),  // индекс 4  (кадр 25)
        sf::IntRect(138,128, 25, 61),  // индекс 5  (кадр 26)
        sf::IntRect(167,128, 27, 61),  // индекс 6  (кадр 27)
        sf::IntRect(197,128, 23, 61)   // индекс 7  (кадр 28)
    };

    // ---------------------------------------------------
    // 2) Правильные attackOrigins для каждого из 8 кадров:
    //    Мы уже подсчитали «нижнюю непрозрачную строку» (bottommost_row) для каждого из них:
    //      – для «↓» (индексы 0–3) bottommost_row = {61, 31, 31, 31};
    //      – для «↑» (индексы 4–7) bottommost_row = 60.
    //    Поскольку у базового idle-кадра (32×32) origin.y = 16 и его нижний непрозрачный пиксель = 31,
    //    мы используем формулу origin.y = bottommost_row − 15, чтобы «ступня» у всех кадров лежала ровно в y+16.
    //
    //    ↓-кадры:
    //      index 0 : bottommost_row = 61 ⇒ origin.y = 61−15 = 46
    //      index 1 : bottommost_row = 31 ⇒ origin.y = 31−15 = 16
    //      index 2 : bottommost_row = 31 ⇒ origin.y = 16
    //      index 3 : bottommost_row = 31 ⇒ origin.y = 16
    //    ↑-кадры:
    //      indices 4–7 : bottommost_row = 60 ⇒ origin.y = 60−15 = 45

    attackOrigins = {
        // ↓  (индексы 0…3)
        { attackFrames[0].width / 2.f, 16.f },  // кадр 7 (индек 0)
        { attackFrames[1].width / 2.f, 16.f },  // кадр 8 (индек 1)
        { attackFrames[2].width / 2.f, 16.f },  // кадр 9 (индек 2)
        { attackFrames[3].width / 2.f, 16.f },  // кадр 10 (индек 3)
        // ↑  (индексы 4…7)
        { attackFrames[4].width / 2.f, 15.f },  // кадр 25 (индек 4)
        { attackFrames[5].width / 2.f, 45.f },  // кадр 26 (индек 5)
        { attackFrames[6].width / 2.f, 45.f },  // кадр 27 (индек 6)
        { attackFrames[7].width / 2.f, 45.f }   // кадр 28 (индек 7)
    };

    // ---------------------------------------------------
    // 3) Заполняем 8-направленную анимацию ходьбы (32×32-кадры):
    //    – moveDownFrames     (row 1, col 0–1: y=32…63, x=0;32),
    //    – moveDownLeftFrames (row 2, col 0–3: y=64…95, x=0;32;64;96),
    //    – moveLeftFrames     (row 3, col 0–3: y=96…127, x=0;32;64;96),
    //    – moveUpLeftFrames   (row 4, col 0–3: y=128…159, x=0;32;64;96),
    //    – moveUpFrames       (row 5, col 0–1: y=160…191, x=0;32).
    //
    //  Правые направления (→, ↘, ↗) мы не дублируем – просто зеркалим «левых» (←, ↙, ↖).
    moveDownFrames = {
        sf::IntRect(0 * 32, 1 * 32, 32, 32),  // (0,32)
        sf::IntRect(1 * 32, 1 * 32, 32, 32)   // (32,32)
    };
    moveDownLeftFrames = {
        sf::IntRect(0 * 32, 2 * 32, 32, 32),  // (0,64)
        sf::IntRect(1 * 32, 2 * 32, 32, 32),  // (32,64)
        sf::IntRect(2 * 32, 2 * 32, 32, 32),  // (64,64)
        sf::IntRect(3 * 32, 2 * 32, 32, 32)   // (96,64)
    };
    moveLeftFrames = {
        sf::IntRect(0 * 32, 3 * 32, 32, 32),  // (0,96)
        sf::IntRect(1 * 32, 3 * 32, 32, 32),  // (32,96)
        sf::IntRect(2 * 32, 3 * 32, 32, 32),  // (64,96)
        sf::IntRect(3 * 32, 3 * 32, 32, 32)   // (96,96)
    };
    moveUpLeftFrames = {
        sf::IntRect(0 * 32, 4 * 32, 32, 32),  // (0,128)
        sf::IntRect(1 * 32, 4 * 32, 32, 32),  // (32,128)
        sf::IntRect(2 * 32, 4 * 32, 32, 32),  // (64,128)
        sf::IntRect(3 * 32, 4 * 32, 32, 32)   // (96,128)
    };
    moveUpFrames = {
        sf::IntRect(0 * 32, 5 * 32, 32, 32),  // (0,160)
        sf::IntRect(1 * 32, 5 * 32, 32, 32)   // (32,160)
    };
}

void Skeleton::startDying() {
    if (isDying) return;
    isDying = true;
    canCollide = false;
    // копируем текущий спрайт
    deathSprite = sprite;
    deathTimer = 0.0f;
}


/****************************  update  ***********************************/
void Skeleton::update(float dt)
{
    if (!life) return;                 // если мы уже «мертвые» – выходим

    // 1) Если умираем – обрабатываем смерть
    if (isDying) {
        deathTimer += dt;
        if (deathTimer >= deathDuration) {
            life = false;
            target->addMoney(reward);
        }
        return;
    }

    // 2) Сначала «AI»: ожидание, преследование, возвращение
    //    updateAI(dt) вернёт true, если мы должны «продолжить бежать» (Chasing),
    //    false, если «застыть» (Waiting/Returning сделали своё смещение).
    updateAI(dt);

    // 3) Обновляем «кости», которые уже вылетели:
    for (auto& b : bones) {
        if (b->life) b->update(dt);
        if (b->life && b->checkCollision(target)) b->onCollision(target);
    }
    bones.erase(std::remove_if(bones.begin(), bones.end(),
        [](auto& b) { return !b->life; }),
        bones.end());

    // 4) Двигаемся / атакуем
    setTargetPosition(target->getPosition());
    if (currentCooldown > 0.f)
        currentCooldown -= dt;

    updateAttack(dt);    // сама анимация броска и спавн кости
    updateMovement(dt);  // ходьба/idle/возврат

    // 5) Коллизии со стенами и финальная позиция sprite
    interactWithMap();
    sprite.setPosition(x, y);
}

/************************  movement =============================== */
void Skeleton::updateMovement(float dt)
{
    // 1) Если сейчас мы в стадии броска (isAttacking == true), не двигаемся вовсе
    if (isAttacking)
        return;

    // 2) Узнаём, в каком состоянии AI сейчас (Waiting, Chasing или Returning)
    AIState state = getAIState();

    // === 2a) STATE = Waiting: монстр либо «блуждает» (movePhase == true), либо стоит (movePhase == false) ===
    if (state == AIState::Waiting) {
        // Monster::updateWaiting(dt) уже сдвинул x,y, если movePhase == true
        if (movePhase) {
            // — монстр бежит к текущему wanderTarget, а мы лишь «надеваем» на него walk-кадры:
            float dirX, dirY;
            dirX = wanderTarget.x - x;
            dirY = wanderTarget.y - y;
            float distW = std::hypot(dirX, dirY);
            if (distW > 0.1f) {
                dirX /= distW;
                dirY /= distW;
            }

            // переключаем кадр ходьбы
            moveAnimTimer += dt;
            if (moveAnimTimer >= moveAnimPeriod) {
                moveAnimTimer = 0.f;
                ++currentMoveFrame;
            }

            // вычисляем угол (в градусах) движения к wanderTarget
            float ang = std::atan2(dirY, dirX) * 180.f / PI;
            if (ang < 0) ang += 360.f;

            bool isMirror = false;
            sf::IntRect walkRect;

            // 8 направлений:
            if (ang > 67.5f && ang <= 112.5f) {
                // ↓
                int cnt = static_cast<int>(moveDownFrames.size()); // = 2
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownFrames[idx];
            }
            else if (ang > 22.5f && ang <= 67.5f) {
                // ↘ (зеркало «↙»)
                int cnt = static_cast<int>(moveDownLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
                isMirror = true;
            }
            else if (ang > 112.5f && ang <= 157.5f) {
                // ↙
                int cnt = static_cast<int>(moveDownLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
            }
            else if (ang > 157.5f && ang <= 202.5f) {
                // ←
                int cnt = static_cast<int>(moveLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
            }
            else if (ang > 202.5f && ang <= 247.5f) {
                // ↖
                int cnt = static_cast<int>(moveUpLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
            }
            else if (ang > 247.5f && ang <= 292.5f) {
                // ↑
                int cnt = static_cast<int>(moveUpFrames.size()); // = 2
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpFrames[idx];
            }
            else if (ang > 292.5f && ang <= 337.5f) {
                // ↗ (зеркало «↖»)
                int cnt = static_cast<int>(moveUpLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
                isMirror = true;
            }
            else {
                // → (зеркало «←»)
                int cnt = static_cast<int>(moveLeftFrames.size()); // = 4
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
                isMirror = true;
            }

            sprite.setTextureRect(walkRect);
            sprite.setOrigin(16.f, 16.f);
            sprite.setScale(isMirror ? -1.f : 1.f, 1.f);
        }
        else {
            // — фаза “стоим” (movePhase == false): двухкадровый idle (Row 0, Col 0/1)
            frameTime += dt;
            if (frameTime >= frameDuration) {
                frameTime = 0.f;
                currentFrame = 1.f - currentFrame; // 0 ⇔ 1

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

    // === 2b) STATE = Returning: монстр возвращается к homePos (обновление x,y уже сделал updateReturning) ===
    if (state == AIState::Returning) {
        // определяем, далеко ли до homePos:
        float deltaX = homePos.x - x;
        float deltaY = homePos.y - y;
        float distH = std::hypot(deltaX, deltaY);

        if (distH > 2.f) {
            // переключаем кадр ходьбы
            moveAnimTimer += dt;
            if (moveAnimTimer >= moveAnimPeriod) {
                moveAnimTimer = 0.f;
                ++currentMoveFrame;
            }

            // нормируем направление и выбираем кадр walk-animation
            float dirX = deltaX / distH;
            float dirY = deltaY / distH;

            float ang = std::atan2(dirY, dirX) * 180.f / PI;
            if (ang < 0) ang += 360.f;

            bool isMirror = false;
            sf::IntRect walkRect;

            if (ang > 67.5f && ang <= 112.5f) {
                // ↓
                int cnt = static_cast<int>(moveDownFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownFrames[idx];
            }
            else if (ang > 22.5f && ang <= 67.5f) {
                // ↘
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
                isMirror = true;
            }
            else if (ang > 112.5f && ang <= 157.5f) {
                // ↙
                int cnt = static_cast<int>(moveDownLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveDownLeftFrames[idx];
            }
            else if (ang > 157.5f && ang <= 202.5f) {
                // ←
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
            }
            else if (ang > 202.5f && ang <= 247.5f) {
                // ↖
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
            }
            else if (ang > 247.5f && ang <= 292.5f) {
                // ↑
                int cnt = static_cast<int>(moveUpFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpFrames[idx];
            }
            else if (ang > 292.5f && ang <= 337.5f) {
                // ↗
                int cnt = static_cast<int>(moveUpLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveUpLeftFrames[idx];
                isMirror = true;
            }
            else {
                // → 
                int cnt = static_cast<int>(moveLeftFrames.size());
                int idx = currentMoveFrame % cnt;
                walkRect = moveLeftFrames[idx];
                isMirror = true;
            }

            sprite.setTextureRect(walkRect);
            sprite.setOrigin(16.f, 16.f);
            sprite.setScale(isMirror ? -1.f : 1.f, 1.f);
        }
        // если дошли до homePos (distH ≤ 2), то updateReturning изменил aiState на Waiting,
        // и в следующем кадре мы снова попадём в ветку Waiting.
        return;
    }

    // === 2c) STATE = Chasing: монстр бежит к игроку, пока dist > attackRange ===
    float dxToPlayer = targetPosition.x - x;
    float dyToPlayer = targetPosition.y - y;
    float distToPlayer = std::hypot(dxToPlayer, dyToPlayer);

    if (distToPlayer > attackRange) {
        // переключаем кадр ходьбы
        moveAnimTimer += dt;
        if (moveAnimTimer >= moveAnimPeriod) {
            moveAnimTimer = 0.f;
            ++currentMoveFrame;
        }

        // нормируем и двигаемся
        float vx = dxToPlayer / distToPlayer;
        float vy = dyToPlayer / distToPlayer;
        x += vx * speed * dt * 1000.f;
        y += vy * speed * dt * 1000.f;

        // выбираем walk-кадр по углу
        float ang = std::atan2(vy, vx) * 180.f / PI;
        if (ang < 0) ang += 360.f;

        bool isMirror = false;
        sf::IntRect walkRect;

        if (ang > 67.5f && ang <= 112.5f) {
            // ↓
            int cnt = static_cast<int>(moveDownFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownFrames[idx];
        }
        else if (ang > 22.5f && ang <= 67.5f) {
            // ↘
            int cnt = static_cast<int>(moveDownLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownLeftFrames[idx];
            isMirror = true;
        }
        else if (ang > 112.5f && ang <= 157.5f) {
            // ↙
            int cnt = static_cast<int>(moveDownLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveDownLeftFrames[idx];
        }
        else if (ang > 157.5f && ang <= 202.5f) {
            // ←
            int cnt = static_cast<int>(moveLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveLeftFrames[idx];
        }
        else if (ang > 202.5f && ang <= 247.5f) {
            // ↖
            int cnt = static_cast<int>(moveUpLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpLeftFrames[idx];
        }
        else if (ang > 247.5f && ang <= 292.5f) {
            // ↑
            int cnt = static_cast<int>(moveUpFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpFrames[idx];
        }
        else if (ang > 292.5f && ang <= 337.5f) {
            // ↗
            int cnt = static_cast<int>(moveUpLeftFrames.size());
            int idx = currentMoveFrame % cnt;
            walkRect = moveUpLeftFrames[idx];
            isMirror = true;
        }
        else {
            // → 
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

    // ► Если до игрока ≤ attackRange, просто «застываем» в Idle,
    //   а Attack-анимация будет отрисована в updateAttack().
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

/************************  attack ================================ */
void Skeleton::updateAttack(float dt)
{
    float dist = std::hypot(targetPosition.x - x, targetPosition.y - y);

    // 1) Если можем начать атаку (dist ≤ attackRange и кулдаун = 0)
    if (!isAttacking && dist <= attackRange && currentCooldown <= 0.f)
    {
        float vx = targetPosition.x - x;
        float vy = targetPosition.y - y;

        upperGroup = (vy < 0.f);  // цель выше? true → группа 4…7 (атака ↑)
        mirror = upperGroup
            ? (vx < 0.f)   // если ↑ и цель левее → зеркалим «↖», иначе остаём «↗»
            : (vx > 0.f);  // если ↓ и цель правее → зеркалим «↘», иначе остаём «↙»

        isAttacking = true;
        boneFired = false;
        currentAttackStage = 0;
        stageTimer = attackWindup;
    }

    if (!isAttacking)
        return;

    // 2) Отсчитываем таймер текущей стадии (Wind-up → Strike → Recovery)
    stageTimer -= dt;
    if (stageTimer <= 0.f) {
        ++currentAttackStage;
        switch (currentAttackStage)
        {
        case 1: {
            // STRIKE: при замахе спавним кость
            stageTimer = attackStrike;
            if (!boneFired) {
                bones.emplace_back(std::make_shared<Bone>(
                    sf::Vector2f{ x, y },
                    targetPosition,
                    boneSpeed, boneDamage,
                    this,
                    levelManager  // чтобы кости знали карту
                ));
                boneFired = true;
            }
            break;
        }
        case 2:
            // RECOVERY: ещё один кадр отпуска
            stageTimer = attackRecovery;
            break;
        default:
            // конец атаки: возвращаемся к обычному бегу/ожиданию
            isAttacking = false;
            currentCooldown = attackCooldown;
            return;
        }
    }

    // 3) В зависимости от стадии (Wind-up, Strike, Recovery) выбираем локальный кадр:
    int baseIndex = upperGroup ? 4 : 0;  // 0…3 = ↓, 4…7 = ↑
    int localIdx;
    if (currentAttackStage == 0) {
        // WIND-UP: плавно переключаемся между кадрами 0 и 1 (или 4 и 5):
        float p = 1.f - (stageTimer / attackWindup);
        localIdx = (p < 0.5f ? 0 : 1);
    }
    else if (currentAttackStage == 1) {
        // STRIKE (замах) → локальный индекс = 2
        localIdx = 2;
    }
    else {
        // RECOVERY (бросок + возврат) → локальный индекс = 3
        localIdx = 3;
    }

    int idx = baseIndex + localIdx;
    sprite.setTextureRect(attackFrames[idx]);
    sprite.setOrigin(attackOrigins[idx]);
    sprite.setScale(mirror ? -1.f : 1.f, 1.f);
}

/****************************  draw  ***********************************/
void Skeleton::draw(sf::RenderWindow& window) const {
    if (!life) return;

    if (isDying) {
        // fade-out «ghost-style»
        sf::Sprite temp = deathSprite;
        float alpha = 1.0f - (deathTimer / deathDuration);
        temp.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
        window.draw(temp);
    }
    else {
        // сначала кости, чтобы скелет отрисовывался поверх
        for (auto& b : bones) {
            b->draw(window);
        }
        Entity::draw(window);
    }
}
