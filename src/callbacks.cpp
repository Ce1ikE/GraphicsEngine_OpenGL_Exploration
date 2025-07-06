#include "Defaults/callbacks.h"
#include "Logger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Retrieve the Game instance associated with this specific window
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		// We have to tell OpenGL the size of the rendering window 
		// so OpenGL knows how we want to display the data and coordinates 
		// with respect to the window.
		glViewport(0, 0, width, height);
		Scene* mainScene = UIManager::Scenes[STD_SCENE];
		if (mainScene)
		{
			mainScene->getCamera()->updateProjection(width, height);
		}
	}
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
			Logger::info(
				MESSAGE("ESC pressed")
			);
		}

		if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		{
			gameInstance->m_enabledDepthTest = !gameInstance->m_enabledDepthTest;
			gameInstance->m_enabledDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			Logger::info(
				MESSAGE("Changed depth test")
			);
		}

		if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
		{
			gameInstance->m_enabledCaptureCursor = !gameInstance->m_enabledCaptureCursor;
			gameInstance->m_enabledCaptureCursor ? glfwSetInputMode(gameInstance->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_CAPTURED) : glfwSetInputMode(gameInstance->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Logger::info(
				MESSAGE("Changed cursor mode")
			);
		}

		if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
		{
			gameInstance->m_enabledVSync = !gameInstance->m_enabledVSync;
			gameInstance->m_enabledVSync ? glfwSwapInterval(1) : glfwSwapInterval(0);
			Logger::info(
				MESSAGE("Changed v-sync")
			);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				gameInstance->m_keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				gameInstance->m_keys[key] = false;
			}
		}
	}
};

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game * gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		
		// only process movement if we're actually dragging (middle button down)
		if (!gameInstance->m_mouseKeys[GLFW_MOUSE_BUTTON_MIDDLE] || gameInstance->m_lastX == -1.0) {
			// If not dragging, just update last positions
			gameInstance->m_lastX = xpos;
			gameInstance->m_lastY = ypos;
			return;
		}
		
		float xoffset = xpos - gameInstance->m_lastX;
		float yoffset = gameInstance->m_lastY - ypos;
		gameInstance->m_lastX = xpos;
		gameInstance->m_lastY = ypos;

		// process is different based upon (MMB + shift or MMB)
		Scene* mainScene = UIManager::Scenes[STD_SCENE];
		if (mainScene)
		{
			if (gameInstance->m_keys[GLFW_KEY_LEFT_SHIFT])
			{
				mainScene->getCamera()->processPan(xoffset, yoffset);
			}
			else
			{
				mainScene->getCamera()->processOrbit(xoffset, yoffset);
			}
		}
	}
};

void mouse_scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
	// a "normal" mouse wheel, being vertical, provides offsets along the Y-axis
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		Scene* mainScene = UIManager::Scenes[STD_SCENE];
		if (mainScene)
		{
			mainScene->getCamera()->processZoom(yoff);
		}
	}
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (gameInstance) {
		if (button == GLFW_MOUSE_BUTTON_MIDDLE || button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				gameInstance->m_mouseKeys[GLFW_MOUSE_BUTTON_MIDDLE] = true;
				// Capture initial position when button is pressed
				glfwGetCursorPos(window, &gameInstance->m_lastX, &gameInstance->m_lastY);
			}
			
			if (action == GLFW_RELEASE) {
				gameInstance->m_mouseKeys[GLFW_MOUSE_BUTTON_MIDDLE] = false;
				// reset last position to signal start of a new drag next time
				gameInstance->m_lastX = -1.0;
				gameInstance->m_lastY = -1.0;
			}
		}
	}
};