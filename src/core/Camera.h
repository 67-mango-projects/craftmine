#pragma once
#include "Math.h"
#include <glm/glm.hpp>

class Player;
class Renderer;

class Camera {
private:
	inline static Camera *m_currentCamera;
	Player* m_player;
	Vector2 m_position;
	float m_rotation;
	float zoom = 1;
public:
	Camera(Player *p);
	void setZoom(float z) { zoom = z; }
	void updateCamera();
	static Camera* getCurrentCamera() { return m_currentCamera; }
	Vector2 getPosition() const { return m_position; }
	glm::mat4 getViewMatrix();

private:
	void bindPlayer(Player *p);
};	