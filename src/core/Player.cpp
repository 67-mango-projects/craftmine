#include "Player.h"
#include "Controller.h"
#include "../game.h"

Player::Player() : Entity(100, 2, m_model) {
	m_model.texture = m_frontTexture;
	m_currentPlayer = this;

	Controller::bindPress(handlePressInput);
	Controller::bindPress(handleReleaseInput);
}
void Player::renderPlayer() {
	
}
void Player::updatePlayer() {
	double dt = Game::getDeltaTime();
	m_model.position += m_velocity;
}

void Player::handlePressInput(int key) {
	if (key == GLFW_KEY_W) {
		m_currentPlayer->m_velocity.y = -2;
	}
	if (key == GLFW_KEY_A) {
		m_currentPlayer->m_velocity.x = -2;
	}
	if (key == GLFW_KEY_S) {
		m_currentPlayer->m_velocity.y = 2;
	}
	if (key == GLFW_KEY_D) {
		m_currentPlayer->m_velocity.x = 2;
	}
}
void Player::handleReleaseInput(int key) {
	if (key == GLFW_KEY_W) {
		m_currentPlayer->m_velocity.y = 0;
	}
	if (key == GLFW_KEY_A) {
		m_currentPlayer->m_velocity.x = 0;
	}
	if (key == GLFW_KEY_S) {
		m_currentPlayer->m_velocity.y = 0;
	}
	if (key == GLFW_KEY_D) {
		m_currentPlayer->m_velocity.x = 0;
	}
}