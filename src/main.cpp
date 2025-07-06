

// https://learnopengl.com/Getting-started/Creating-a-window
// https://learnopengl.com/Getting-started/Hello-Window

// standard c++ headers ------------
#include <iostream>

// 3rd party headers ------------
// GLAD manages OpenGL function pointers
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// custom c++ headers ------------
#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "Square.h"
#include "Cube.h"
#include "Sphere.h"
#include "Axis.h"
#include "Logger.h"

#include "config.h"

using namespace std;

/* Function declarations */
void bootUp(Scene&,Camera&);

int main(void) 
{
	// 1) create a Game instance first before calling any other class because 
	// Game initializes "glfw" 
	Game game = Game(WINDOW_STD_WIDTH, WINDOW_STD_HEIGHT, WINDOW_STD_NAME);
	Camera camera = Camera();
	Scene mainScene = Scene();

	bootUp(mainScene,camera);
	
	UIManager::getInstance().addScene(&mainScene, STD_SCENE);
	
	game.Run();
	
	Logger::info(MESSAGE("Program shutdown..."));
	return 0;
}

/* Function definitions */

void bootUp(Scene& mainScene, Camera& camera)
{
	mainScene.setCamera(&camera);
	mainScene.addGameObject(
		"MainCube",
		new Sphere(5, 20, 20)
	);
	mainScene.addGameObject(
		"MainAxis",
		new Axis(15.0f)
	);
};