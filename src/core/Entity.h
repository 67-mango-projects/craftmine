#pragma once 
#include "Math.h"
#include "../renderer/QuadRenderer.h"
#include "Weapon.h"
#include<glm/glm.hpp>


enum EntityType {
	PLAYER,
	ZOMBIE,
	SKELETON,
	WIZARD,
	BLAZE,
	AARUSH,
	UNDEADBOSS,
	WITHER,
	DRAGON
};

class World;

class Entity {
private:
	inline static std::vector<Entity*> g_entityList;
	EntityType type;
	inline static std::unordered_map<EntityType, Entity*> g_entitys;
	float m_attackRange = 20;
	Weapon* m_reservedEntWeapon; // reserved for npcs
	Vector2 lookVector;
	float m_attackTimer = 0;
protected:
	Vector2 m_velocity;
	float m_health;
	float m_speed;
	Quad* m_model;
	
public:
	float m_dmgMultiplier = 1;
	virtual const Vector2& getLookVector() {
		return lookVector;
	}
	void setPosition(Vector2 position) {
		m_model->position.x = position.x;
		m_model->position.y = position.y;
	}
	const float getSpeed() const { return m_speed; }
	static void initializeEntitys();
	~Entity() {}
	Entity(EntityType type,float health, float walkspeed, Quad model,Weapon* weapon = nullptr) : type(type), m_health(health), m_speed(walkspeed), m_reservedEntWeapon(weapon) { m_model = new Quad(model); }
	static Entity* spawn(EntityType type, Vector2 position);
	static void renderEntitys(QuadRenderer &r, Shader &s, Camera *c);
	static void updateEntitys(float dt,World& world,Player*p);
	static std::vector<Entity*>& getEntityList() { return g_entityList; }

	virtual void attack(class World& world);
	Entity* clone() const;


	const Quad* getModel() { return m_model; }
	void changeMesh(const Mesh* mesh) { m_model->mesh = mesh; }
	Vector2 getPosition() const { return m_model->position; }
	void setHealth(float health) { m_health = health; }
	float getHealth() const { return m_health; }
};