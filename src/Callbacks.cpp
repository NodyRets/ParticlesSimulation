#include "Callbacks.h"
#include <iostream>

namespace Callbacks
{

	void error_callback(int error, const char* description) 
	{
		std::cerr << "Error: " << description << std::endl;
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
	{
		

	}
}