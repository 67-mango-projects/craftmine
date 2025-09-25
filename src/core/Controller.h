#pragma once
#include <glfw3.h>
#include <vector>

class Controller {
private:
	inline static std::vector<void(*)(int)> m_releaseCallbacks;
	inline static std::vector<void(*)(int)> m_pressCallbacks;
public:
	static void initialize(GLFWwindow* window);
	static void bindPress(void (*func)(int key));
	static void bindRelease(void (*func)(int key));
private:
	static void onKeyPressed(int key);
	static void onKeyReleased(int key);
	static void glCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};