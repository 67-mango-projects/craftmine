#include "Controller.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
void Controller::glCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		onKeyPressed(key);
	}
	if (action == GLFW_RELEASE) {
		onKeyReleased(key);
	}
}

void Controller::glMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		onKeyPressed(button);
	if (action == GLFW_RELEASE)
		onKeyReleased(button);
}

void Controller::onKeyPressed(int key) {
	for (auto& callback : m_pressCallbacks) {
		callback(key);
	}
}

void Controller::updateController(GLFWwindow* window,Camera *c, const Vector2& windowDimensions) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mousePosition = Vector2(x, windowDimensions.y - y);

	float ndcX = (2.0f * static_cast<float>(mousePosition.x)) / windowDimensions.x - 1.0f;
	float ndcY = (2.0f * static_cast<float>(mousePosition.y)) / windowDimensions.y - 1.0f;
	glm::mat4 proj = glm::ortho(-windowDimensions.x * 0.5f, windowDimensions.x * 0.5f,
		-windowDimensions.y * 0.5f, windowDimensions.y * 0.5f,
		-1.0f, 1.0f);
	glm::mat4 view = c->getViewMatrix();

	glm::mat4 invPV = glm::inverse(proj * view);

	glm::vec4 world = invPV * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
	mousePositionWorld = Vector2(world.x, world.y);

}
const Vector2& Controller::getMousePositionWorld() {
	return mousePositionWorld;
}
const Vector2& Controller::getMousePositionScreen() {
	return mousePosition;
}

void Controller::onKeyReleased(int key) {
	for (auto& callback : m_releaseCallbacks) {
		callback(key);
	}
}

void Controller::initialize(GLFWwindow* window) {
	glfwSetKeyCallback(window, glCallback);
	glfwSetMouseButtonCallback(window, glMouseCallback);
}

void Controller::bindPress(void (*func)(int key)) {
	m_pressCallbacks.push_back(func);
}
void Controller::bindRelease(void (*func)(int key)) {
	m_releaseCallbacks.push_back(func);
}