#pragma once
#include <vector>
#include "../renderer/QuadRenderer.h"
#include "Math.h"
#include "Weapon.h"
#include "Entity.h"
#include<unordered_set>
#include<spdlog.h>
struct Projectile {
    bool canHitPlayer;

    Vector2 position;
    Vector2 direction;

    float speed;
    float lifetime;
    float age = 0.0f;

    float fadeDuration = 0.3f; 
    float alpha = 1.0f;  

    Weapon* weapon;
    Entity* owner;
    Quad quad;

    Projectile(const Vector2& pos, const Vector2& dir, float spd,
        float life, Weapon* wpn, Entity* src, const Quad& q,bool canHitPLayer);



    std::unordered_set<Entity*> hitEntities;

    bool hasHit(Entity* e) const {
        return hitEntities.find(e) != hitEntities.end();
    }

    void markHit(Entity* e) {
        hitEntities.insert(e);
    }
};



class ProjectileSystem {
    inline static ProjectileSystem* m_currentPSystem;
    World* m_world;
    QuadRenderer* m_renderer;
    std::vector<Projectile> m_projectiles;

public:
    ProjectileSystem(World* world, QuadRenderer* renderer)
        : m_world(world), m_renderer(renderer) {
        m_currentPSystem = this;
    }
    static ProjectileSystem* getCurrentProjectileSystem() { return m_currentPSystem; }
    void spawn(const Vector2& pos, const Quad& model, const Vector2& dir,
        Weapon* weapon, Entity* owner, float duration, float speed = 0.0f, bool canHitPlayer = false)
    {
        m_projectiles.emplace_back(pos, dir, speed, duration, weapon, owner, model, canHitPlayer);
    }

    void update(float dt);
    void render();
};
