

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
#include "availableResources.h"
#include "Square.h"
#include "Cube.h"
#include "Sphere.h"
#include "config.h"
#include "Logger.h"

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

	UIManager::addScene(&mainScene, STD_SCENE);


	game.Run();

	Logger::info(MESSAGE("Program shutdown..."));
	return 0;
}

/* Function definitions */

void bootUp(Scene& mainScene, Camera& camera)
{
	srand(time(0));
	mainScene.setCamera(&camera);

	int neg = -1;
	for (int i = 0; i <= 4; i++)
	{
		if (rand() % 2)
		{
			neg = -neg;
		}

		int x = (rand() % 10) * neg;
		int y = (rand() % 10) * neg;
		
		std::string name = "Cube" + std::to_string(i);
		
		std::cout << name << " added";

		mainScene.addGameObject(
			name,
			new Sphere(1,20,20,glm::vec3(x, y, -10))
		);
	}

	mainScene.addGameObject(
		"MainCube",
		new Sphere(1, 20, 20)
	);

};