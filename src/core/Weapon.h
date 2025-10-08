#pragma once
#include "../renderer/QuadRenderer.h"

class Player;
class World;

class Weapon {
public:
    virtual ~Weapon() = default;
    virtual void attack(class Entity& player, class World& world, bool canHitPlayer = false) = 0;
    virtual float getAttackSpeed() = 0;
    virtual float getAttackDamage() = 0;
    virtual const Quad& getHitbox() = 0;
    virtual float getProjectileSpeed() = 0;
};


class MeleeWeapon : public Weapon {
    float damage;
    float slashOffset;
    float attackSpeed;
    float precision;
    Quad hitbox;
    int hits;
    float lifeTime;
public:
    MeleeWeapon(float dmg, float atkSpd, float slashOff, Quad q,float xprecision = 0,int amountofHits = 1, float lifeTime = 0.3)
        : damage(dmg), attackSpeed(atkSpd), hitbox(q), slashOffset(slashOff), precision(xprecision), hits(amountofHits), lifeTime(lifeTime) {
    }

    void attack(Entity& player, World& world, bool canHitPlayer) override;
    float getAttackSpeed() override { return attackSpeed; }
    float getAttackDamage() override { return damage; }
    float getProjectileSpeed() override { return 0; }
    const Quad& getHitbox() override { return hitbox; }
};




class BlastWeapon : public Weapon {
    float damage;
    float slashOffset;
    float attackSpeed;
    Quad hitbox;
public:
    BlastWeapon(float dmg, float atkSpd, float slashOff, Quad q)
        : damage(dmg), attackSpeed(atkSpd), hitbox(q), slashOffset(slashOff) {
    }

    void attack(Entity& player, World& world, bool canHitPlayer) override;
    float getAttackSpeed() override { return attackSpeed; }
    float getAttackDamage() override { return damage; }
    float getProjectileSpeed() override { return 0; }
    const Quad& getHitbox() override { return hitbox; }
};

class ScytheWeapon : public Weapon {
    float damage;
    int cycles;
    bool forwardAttack;
    float forwardAttackLength;
    float attackPrecision;
    float currentCycle = 0;
    float attackSpeed;
    Quad hitbox;
public:
    ScytheWeapon(float dmg, float atkSpd, int cyclles, Quad q,bool forwardAttack, float forwardAttackScale,float attackPrecision)
        : damage(dmg), attackSpeed(atkSpd), hitbox(q), cycles(cyclles), forwardAttack(forwardAttack), forwardAttackLength(forwardAttackScale), attackPrecision(attackPrecision) {
    }

    void attack(Entity& player, World& world, bool canHitPlayer) override;
    float getAttackSpeed() override { return attackSpeed; }
    float getAttackDamage() override { return damage; }
    float getProjectileSpeed() override { return 0; }
    const Quad& getHitbox() override { return hitbox; }
};

class ProjectileWeapon : public Weapon {
    float damage;
    Quad hitbox;
    float attackSpeed;
    float projectileSpeed;
    float precision = 0;
    int amountOfProj = 1;
public:

    ProjectileWeapon(float dmg, float atkSpd, float prokectileSpeed, Quad q, float prekision = 0,int amountProj = 1)
        : damage(dmg), attackSpeed(atkSpd), projectileSpeed(prokectileSpeed), hitbox(q), precision(prekision), amountOfProj(amountProj){
    }


    void attack(Entity& player, World& world, bool canHitPlayer) override;
    float getAttackSpeed() override { return attackSpeed; }
    float getAttackDamage() override { return damage; }
    const Quad& getHitbox() override { return hitbox; }
    float getProjectileSpeed() override { return projectileSpeed; }
};