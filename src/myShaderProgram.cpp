

// https://learnopengl.com/Getting-started/Creating-a-window
// https://learnopengl.com/Getting-started/Hello-Window

// standard c++ headers ------------
#include <iostream>

// 3rd party headers ------------
// GLAD manages OpenGL function pointers
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// custom c++ headers ------------
#include "UtilClasses/Game.h"
#include "UtilClasses/Scene.h"
#include "UtilClasses/Camera.h"
#include "Defaults/availableResources.h"
#include "ResourceClasses/Square.h"
#include "ResourceClasses/Cube.h"
#include "Defaults/config.h"
#include "Logger.h"

using namespace std;

int main(void) 
{
	Game game = Game(WINDOW_STD_WIDTH, WINDOW_STD_HEIGHT, WINDOW_STD_NAME);
	
	Camera camera = Camera();
	
	Scene scene = Scene();
	scene.setCamera(&camera);
	scene.addGameObject("cube", new Cube());

	UIManager::addScene(&scene,"main scene");

	game.Run();

	Logger::info(MESSAGE("Program shutdown..."));
	return 0;
}

