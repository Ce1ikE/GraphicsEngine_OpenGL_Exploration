#include "UtilClasses/Game.h"

Game::Game(unsigned int width, unsigned int height, std::string name)
    : m_state(GAME_ACTIVE), m_keys(), m_windowWidth(width), m_windowHeight(height)
{
	Init();
	registerCallbacks();
	UISetup();
	// shaders and textures
	loadResources();

	// enables wireframe view
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// enables VSync
	// https://www.khronos.org/opengl/wiki/Swap_Interval
	glfwSwapInterval(1);
	
	// enables OpenGL to use the Z-buffer
	glEnable(GL_DEPTH_TEST);
	

	Logger::succes(MESSAGE("Finished initialization...starting main rendering loop"));
}

Game::~Game()
{
	Logger::succes(MESSAGE("Window was closed"));
	glfwTerminate();
	Logger::succes(MESSAGE("Gl cleanup complete"));
}

void Game::Init()
{
	glfwInit();

	createWindow();

	// GLAD manages function pointers for OpenGL 
	// so we want to initialize GLAD before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::error(
			MESSAGE("Failed to initialize GLAD")
		);
		glfwTerminate();
		return;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	Logger::info(MESSAGE("Application started\nMaximum nr of vertex attributes supported : " + std::to_string(nrAttributes)));

}

void Game::Update(float dt)
{
	// a callback handles keypresses but based upon what is currently pressed 
	// we want to do some calculations such as moving camera, rotating ,clicking , etc...

	if (UIManager::hasShaderUniformsChanged()) {
		Shader* currentShader = ResourceManager::GetShader("basic"); // Get the active shader

		if (currentShader) {
			// Iterate through the UI elements and update the corresponding shader uniforms
			for (auto const& [name, element_ptr] : UIManager::UIElements) {

				if (auto sliderFloat = dynamic_cast<UISliderFloat*>(element_ptr.get())) {
					currentShader->SetFloat(name.c_str(), sliderFloat->getValue());
				}
				else if (auto sliderVec3 = dynamic_cast<UISliderVec3*>(element_ptr.get())) {
					currentShader->SetVector3f(name.c_str(), sliderVec3->getValue());
				}
				// ... handle other types ...
			}
		}
		// Reset the flag after processing
		UIManager::resetShaderUniformsChangedFlag(); 
	}

	// Option 2: Check specific buttons
	if (UIButton* recompileBtn = UIManager::getButton(std::string("RecompileShaderButton"))) {
		if (recompileBtn->isClicked()) {
			// Trigger shader recompile logic
			Logger::info(
				MESSAGE("Recompiling shader...")
			);
			Shader recompiledShader = ResourceManager::LoadShader("vertexShaders.glsl", "fragmentShaders.glsl", nullptr, "basic");
			// After recompiling, might need to call generateUIFromShader again
			// to update UI for new/changed uniforms.
			UIManager::generateUIFromShader(&recompiledShader);
			UIManager::generateUI();
		}
	}

	if (UISelect* wireframeBtn = UIManager::getSelect(std::string("ViewMode"))) {
		if (wireframeBtn->isChanged()) {
			switch (wireframeBtn->getSelected())
			{
			case 0:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case 1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			default:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			}
			Logger::info(
				MESSAGE("Changed view mode...")
			);
		}
	}

	UpdateObjects(dt);
}

void Game::UpdateObjects(float dt)
{

	float rotationSpeedDegreesPerSecond = 30.0f;
	float rotationAngle = rotationSpeedDegreesPerSecond * dt; // Angle for this frame
	glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 1.0f);

	for (auto& scenePair : UIManager::Scenes)
	{
		Scene& currentScene = scenePair.second;
		if (currentScene.isActive)
		{
			for(auto& gameObjectPair : *currentScene.getGameObjects())
			{
				GameObject* gObj = gameObjectPair.second;
				glm::quat frameRotation = glm::angleAxis(glm::radians(rotationAngle), rotationAxis);
				gObj->setRotation(frameRotation * gObj->getRotation());
			}
		}
	}
};

void Game::Render()
{
	// is a state-setting function
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// is a state-using function
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UIManager::RenderActiveScenes();
	
	UIManager::StartFrame();
	UIManager::RenderGameUI();
	UIManager::EndFrame();
}

void Game::handleFramebufferSize(GLFWwindow* window, int width, int height)
{
	// We have to tell OpenGL the size of the rendering window 
	// so OpenGL knows how we want to display the data and coordinates 
	// with respect to the window.
	glViewport(0, 0, width, height);
};

void Game::handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		Logger::info(
			MESSAGE("ESC pressed")
		);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			m_keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			m_keys[key] = false;
		}
	}

};

void Game::UISetup()
{
	UIManager::Init(m_gameWindow);
}

void Game::registerCallbacks()
{
	// the moment a user resizes the window the viewport should be adjusted as well. 
	// We can register a callback function on the window that gets called each time the window is resized
	glfwSetFramebufferSizeCallback(m_gameWindow, framebuffer_size_callback);
	glfwSetKeyCallback(m_gameWindow, key_callback);
	// There are many callbacks functions we can set to register our own functions. 
	// For example, we can make a callback function to process joystick input changes, process error messages etc. 
	// We register the callback functions after we've created the window and before the render loop is initiated.

	glfwSetWindowUserPointer(m_gameWindow, this);
};

void Game::createWindow()
{
	// a list of all options : https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// This window object holds all the windowing data and is required by most of GLFW's other functions.
	m_gameWindow = glfwCreateWindow(WINDOW_STD_WIDTH, WINDOW_STD_HEIGHT, WINDOW_STD_NAME, NULL, NULL);
	if (m_gameWindow == NULL)
	{
		Logger::error(
			MESSAGE("Failed to create GLFW window")
		);
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_gameWindow);
}

void Game::loadResources()
{
	// Next we want to create a vertex and fragment shader that actually processes this data
	// shaders are written in the shader language "GLSL" (OpenGL Shading Language) which is a language very similar to C.
	// the language has it's own datatypes and input output features
	Shader newshader = ResourceManager::LoadShader("vertexShaders.glsl","fragmentShaders.glsl", nullptr, "basic");
	UIManager::generateUIFromShader(&newshader);
	UIManager::generateUI();
}

void Game::Run()
{
	// main loop a.k.a the "render loop"
	// We don't want the application to draw a single image and then immediately quit and close the window
	// https://community.khronos.org/t/confirmation-that-i-actually-understand-vaos-vbos-and-ebos/73711/6
	
	// BEFORE the drawing loop:
	// ------------------------
	// 1) create window instance and add the correct callbacks to the window
	// 2) load and compile shaders
	// 3) create a GameObject and attach materials and mesh 
	//	3.1) material  = texture  + shader from the ResourceManager
	//	3.2) mesh      = vertices + indices data from a file or hardcoded
	//	  3.2.1) Generate VAO, VBO , EBO 
	//	  3.2.2) bind VAO , 
	//	  3.2.3) bind VBO , fill VBO
	//    3.2.4) bind EBO , fill EBO
	//	  3.2.5) Set up vertex attributes
	//	  3.2.6) Unbind VBO

	// DURING the drawing loop:
	// ------------------------
	// 1) Render calls the UIManger's RenderActiveScenes and then RenderUI
	//  1.1) RenderActiveScenes calls renderScene on only the Scenes in which 'active' attribute is set to 'true'
	//    1.1.1) renderScene = calls draw on every GameObject(such as Cube, Spheres) (glUseProgram + glDrawElements)
	// 	  1.1.2) RenderUI    = calls all ImGui functions to draw and setup the UI + gathering input data
	// 	         (e.g.: ImGui::NewFrame(), ImGui::Begin(), ImGui::End(), ImGui::Render())
	// 2) based upon input data perform some actions such are recompiling shaders or adjusting Uniforms 
	// 3) glfwPollEvents is called to check for events (the callbacks)
	// 4) glfwSwapBuffers is called to swap front and back buffer


	// AFTER the drawing loop:
	// -----------------------
	// 1) delete all vertex arrays
	// 2) terminate glfw
	// 3) shutdown ImGui

	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(m_gameWindow))
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// RENDER ===================================================
		Render();

		// UPDATE   ==========================================
		Update(deltaTime);

		// CHECK EVENTS    ==========================================
		// poll IO events (keys pressed/released, mouse moved etc.)
		glfwPollEvents();

		// UPDATE WINDOW   ==========================================
		// windowing applications apply a double buffer for rendering
		// the Front buffer
		// the Back buffer
		// as soon as all the rendering commands are finished we swap the back buffer to the front buffer
		// so the image can be displayed without still being rendered to avoid any artifacts
		glfwSwapBuffers(m_gameWindow);
	}
};
