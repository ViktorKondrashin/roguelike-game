#include "Ghost.h"
#include "Player.h"
#include <cmath>

namespace {
    constexpr float PI = 3.14159265f;
}

Ghost::Ghost(float x, float y, float w, float h,
    const std::string& texPath,
    Player* player,
    LevelManager* lvlMgr)
    : Monster(x, y, w, h, texPath, player, lvlMgr)
{
    damage = 8;
    reward = 10;
    attackCooldown = 1.0f;
    hp = 25;
    canCollide = false;

    speed = baseSpeed;

    sprite.setOrigin(w / 2.f, h / 2.f);
    sprite.setColor({ 255,255,255,200 });
    sprite.setTexture(texture);

    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 3; ++c)
            frames[r][c] = { c * 32, r * 40, 32, 40 };

    sprite.setTextureRect(frames[DOWN][0]);
}

void Ghost::update(float dt)
{
    if (!life) return;

    prevX = x;  prevY = y;

    if (isDying) {
        deathTimer += dt;
        if (deathTimer >= deathDur) life = false;
        return;
    }
    if (postAttackIdle) {
        postAttackIdleTimer -= dt;
        if (postAttackIdleTimer <= 0.f) {
            postAttackIdle = false;
        }
        return;
    }
    bool chasing = updateAI(dt);

    if (!chasing) {
        interactWithMap();
        sprite.setPosition(x, y);

        moveX = x - prevX;
        moveY = y - prevY;
        updateAnimation(dt);
        return;
    }
    if (currentCooldown > 0.f) currentCooldown -= dt;

    if (isDashing) updateDash(dt);
    else           updateChase(dt);

    updateAnimation(dt);

    sprite.setPosition(x + width / 2.f, y + height / 2.f);

    moveX = x - prevX;
    moveY = y - prevY;
}

void Ghost::draw(sf::RenderWindow& w) const
{
    if (!life) return;

    if (isDying) {
        sf::Sprite fade = deathSprite;
        float k = 1.f - (deathTimer / deathDur);
        fade.setColor({ 255,255,255,static_cast<sf::Uint8>(255 * k) });
        w.draw(fade);
    }
    else {
        w.draw(sprite);
    }
}

bool Ghost::checkCollision(Entity* other)
{
    if (!life || isDying || !other || other == this) return false;

    if (dynamic_cast<Player*>(other) || dynamic_cast<Monster*>(other))
        return false;

    if (AABBCollision(other))
        return true;

    return false;
}

void Ghost::onCollision(Entity* other)
{
    applyProjectileHit(other);
}

void Ghost::startDying()
{
    if (isDying) return;
    isDying = true;
    deathSprite = sprite;
    deathTimer = 0.f;
}

void Ghost::updateChase(float dt)
{
    sf::Vector2f pp = target->getPosition();
    float dx = pp.x - x;
    float dy = pp.y - y;
    float dist = std::hypot(dx, dy);

    if (dist < visionR && currentCooldown <= 0.f) {
        if (!isWaiting) {
            isWaiting = true;
            waitTimer = waitDur;
            lastSeenPos = pp;
        }
    }

    if (isWaiting) {
        waitTimer -= dt;
        if (waitTimer <= 0.f) {
            isWaiting = false;
            isDashing = true;
            dashTimer = dashDur;
            hitThisDash = false;

            dashDirX = lastSeenPos.x - x;
            dashDirY = lastSeenPos.y - y;
            float len = std::hypot(dashDirX, dashDirY);
            if (len) { dashDirX /= len; dashDirY /= len; }
        }
        return;
    }

    if (dist > 1.f) {
        dx /= dist; dy /= dist;
        x += dx * baseSpeed * dt * 1000.f;
        y += dy * baseSpeed * dt * 1000.f;
    }
}

void Ghost::updateDash(float dt)
{
    x += dashDirX * dashSpeed * dt * 1000.f;
    y += dashDirY * dashSpeed * dt * 1000.f;

    dashTimer -= dt;

    if (!hitThisDash && Collision::BoundingBoxTest(sprite, target->getSprite())) {
        target->takeDamage(damage);
        hitThisDash = true;
    }
    if (dashTimer <= 0.f) {
        isDashing = false;
        currentCooldown = attackCooldown;

        postAttackIdle = true;
        postAttackIdleTimer = postAttackIdleDuration;

        float ang = std::atan2(dashDirY, dashDirX) * 180.f / PI;
        if (ang < 0) ang += 360.f;

        bool mir = false;
        Row row = angleToRow(ang, mir);
        sprite.setTextureRect(frames[row][0]);
        sprite.setScale(mir ? -1.f : 1.f, 1.f);
    }
}

Ghost::Row Ghost::angleToRow(float ang, bool& mirror) const
{
    mirror = false;

    if (ang < 22.5f || ang >= 337.5f) { mirror = true;  return LEFT; }
    if (ang < 67.5f) { mirror = true;  return DL; }
    if (ang < 112.5f) { return DOWN; }
    if (ang < 157.5f) { return DL; }
    if (ang < 202.5f) { return LEFT; }
    if (ang < 247.5f) { mirror = true;  return UL; }
    if (ang < 292.5f) { return UP; }
    {                return UL;  }
}



void Ghost::updateAnimation(float dt)
{
    animTimer += dt;
    if (animTimer < animPeriod && !isDashing) return;
    animTimer = 0.f;

    float vx, vy;
    if (isDashing) { vx = dashDirX; vy = dashDirY; }
    else if (std::abs(moveX) > 0.1f || std::abs(moveY) > 0.1f) {
        vx = moveX;    vy = moveY;
    }
    else {
        vx = target->x - x;
        vy = target->y - y;
    }

    float ang = std::atan2(vy, vx) * 180.f / PI;
    if (ang < 0) ang += 360.f;

    bool mirror = false;
    Row row = angleToRow(ang, mirror);

    if (isDashing)          animCol = 2;
    else                    animCol = (animCol == 0 ? 1 : 0);
    sprite.setTextureRect(frames[row][animCol]);
    sprite.setScale(mirror ? -1.f : 1.f, 1.f);
}

void Ghost::applyProjectileHit(Entity* other)
{
    struct WithDmg { float damage; };
    float dmg = 0.f;

    if (auto* w = dynamic_cast<WithDmg*>(other))        dmg = w->damage;
    else if (auto* m = dynamic_cast<Monster*>(other))   dmg = 0.f;
    else if (auto* p = dynamic_cast<Player*>(other))    dmg = 0.f;

    if (dmg == 0.f) return;

    hp -= static_cast<int>(dmg);
    other->life = false;

    if (hp <= 0) startDying();
}