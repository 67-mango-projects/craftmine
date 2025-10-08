#pragma once
#include <glfw3.h>
#include <vector>
#include "Math.h"
class Camera;
class Controller {
private:
	inline static std::vector<void(*)(int)> m_releaseCallbacks;
	inline static std::vector<void(*)(int)> m_pressCallbacks;
	inline static Vector2 mousePosition;
	inline static Vector2 mousePositionWorld;
public:
	static void initialize(GLFWwindow* window);
	static void bindPress(void (*func)(int key));
	static void bindRelease(void (*func)(int key));
	static void updateController(GLFWwindow* window,Camera *c, const Vector2& windowDimensions);
	static const Vector2& getMousePositionWorld();
	static const Vector2& getMousePositionScreen();
private:
	static void onKeyPressed(int key);
	static void onKeyReleased(int key);
	static void glMouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void glCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};