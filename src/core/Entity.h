#pragma once 
#include "Math.h"
#include "../renderer/QuadRenderer.h"
#include<glm/glm.hpp>
class Entity {
protected:
	Vector2 m_velocity;
	float m_health;
	float m_speed;
	Quad m_model;
public:
	void setPosition(Vector2& position) {
		m_model.position.x = position.x;
		m_model.position.y = position.y;
	}
	Entity(float health, float walkspeed,Quad model) : m_health(health), m_speed(walkspeed), m_model(model) {}
	glm::mat4 getModelMatrix();
	Quad getModel() { return m_model; }
	Vector2 getPosition() const { return m_model.position; }
	void setHealth(float health) { m_health = health; }
	float getHealth() const { return m_health; }
};