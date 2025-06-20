#include "Defaults/callbacks.h"
#include "Logger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Retrieve the Game instance associated with this specific window
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		gameInstance->handleFramebufferSize(window, width, height);
	}
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Retrieve the Game instance associated with this specific window
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		gameInstance->handleKeyInput(window, key, scancode, action, mode);
	}
};