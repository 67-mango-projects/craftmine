#include "Controller.h"

void Controller::glCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		onKeyPressed(key);
	}
	if (action == GLFW_RELEASE) {
		onKeyReleased(key);
	}
}
void Controller::onKeyPressed(int key) {
	for (auto& callback : m_pressCallbacks) {
		callback(key);
	}
}
void Controller::onKeyReleased(int key) {
	for (auto& callback : m_releaseCallbacks) {
		callback(key);
	}
}

void Controller::initialize(GLFWwindow* window) {
	glfwSetKeyCallback(window, glCallback);
}

void Controller::bindPress(void (*func)(int key)) {
	m_pressCallbacks.push_back(func);
}
void Controller::bindRelease(void (*func)(int key)) {
	m_pressCallbacks.push_back(func);
}