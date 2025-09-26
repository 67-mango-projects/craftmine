#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Player.h"

void Camera::bindPlayer(Player *p) {
	m_player = p;
}

Camera::Camera(Player *p)  {
	bindPlayer(p);
	m_currentCamera = this;
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 view(1.0f);
	view = glm::translate(view, -glm::vec3(m_position));
	view = glm::rotate(view, glm::radians(m_rotation), glm::vec3(0,0,1)); // rotating in the z axis
	return view;
}

void Camera::updateCamera() {
	m_position = m_player->getPosition();
	m_rotation = m_player->getModel().rotation;
}