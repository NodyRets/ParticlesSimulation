#pragma once
#include <GLFW/glfw3.h>

namespace Callbacks
{
	void error_callback(int error, const char* description);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

}