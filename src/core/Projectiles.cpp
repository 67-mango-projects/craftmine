#include "Projectiles.h"
#include "Visuals.h"
#include "Player.h"
#include <spdlog.h>
#include "../renderer/Interface.h"
Projectile::Projectile(const Vector2& pos, const Vector2& dir, float spd,
    float life, Weapon* wpn, Entity* src, const Quad& q,bool canHitPLayer)
    : position(pos), direction(glm::normalize((glm::vec2)dir)), speed(spd),
    lifetime(life), weapon(wpn), owner(src), quad(q),canHitPlayer(canHitPLayer){
}

void ProjectileSystem::update(float dt)
{
    
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();) {
        it->age += dt;

        // Lifetime check
        if (it->age >= it->lifetime) {
            it = m_projectiles.erase(it);
            continue;
        }

        // --- Fade out effect ---



        float timeLeft = it->lifetime - it->age;
        if (timeLeft <= it->fadeDuration) {
            float t = timeLeft / it->fadeDuration;
            it->alpha = t * t;  

            it->alpha = std::max(t, 0.0f);
        }
        else {
            it->alpha = 1.0f;
        }

        // --- Move projectile ---
        it->position += it->direction * it->speed * dt;
        it->quad.position = it->position;

        // --- Rotate in travel direction ---
        if (glm::length(glm::vec2(it->direction)) > 0.0001f) {
            float angleRadians = std::atan2(it->direction.y, it->direction.x);
            it->quad.rotation = glm::degrees(angleRadians);
        }


        if (it->canHitPlayer &&
            Player::getCurrentPlayer()->getModel()->isCollidingWith(it->quad) &&
            !it->hasHit(Player::getCurrentPlayer()))
        {
            Player::getCurrentPlayer()->setHealth(Player::  getCurrentPlayer()->getHealth() - it->weapon->getAttackDamage());
            DamageIndicatorSystem::spawn(Player::getCurrentPlayer()->getPosition(), it->weapon->getAttackDamage());
            it->markHit(Player::getCurrentPlayer());
        }

        // --- Check entity hits ---
        if (!it->canHitPlayer) { // only if projectile *isn’t* restricted to players
            for (auto& e : Entity::getEntityList()) {
                if ((e == it->owner) || it->hasHit(e)) continue;
                if (e->getModel()->isCollidingWith(it->quad)) {
                    e->setHealth(e->getHealth() - it->weapon->getAttackDamage() * it->owner->m_dmgMultiplier);
                    DamageIndicatorSystem::spawn(e->getPosition(), it->weapon->getAttackDamage() * it->owner->m_dmgMultiplier);
                    it->markHit(e);
                }
            }
        }

        ++it;
    }
}


void ProjectileSystem::render()
{

    for (auto& p : m_projectiles) {
        Quad q = p.quad;
        q.color.w = p.alpha; // fade alpha
        m_renderer->render(&q);
    }
}
