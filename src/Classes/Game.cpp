#include "UtilClasses/Game.h"

Game::Game(unsigned int width, unsigned int height, std::string name)
    : m_state(GAME_ACTIVE), m_keys(), m_windowWidth(width), m_windowHeight(height)
{
	Init();
	
	for (int i = 0;i < 8;i++)
	{
		m_mouseKeys[i] = GLFW_RELEASE;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	Logger::info(MESSAGE("Application started\nMaximum nr of vertex attributes supported : " + std::to_string(nrAttributes)));
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
	// sets the callbacks for all devices (keyboard , display , window , mouse, etc...)
	registerCallbacks();
	// sets up ImGui
	UIManager::getInstance().Init(m_gameWindow);
	// enables VSync
	// https://www.khronos.org/opengl/wiki/Swap_Interval
	glfwSwapInterval(1);
	// enables OpenGL to use the Z-buffer
	glEnable(GL_DEPTH_TEST);
	// set the input mode of the cursor
	glfwSetInputMode(m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Game::registerCallbacks()
{
	// the moment a user resizes the window the viewport should be adjusted as well. 
	// We can register a callback function on the window that gets called each time the window is resized
	glfwSetFramebufferSizeCallback(m_gameWindow, framebuffer_size_callback);
	// There are many callbacks functions we can set to register our own functions. 
	// For example, we can make a callback function to process joystick input changes, process error messages etc. 
	// We register the callback functions after we've created the window and before the render loop is initiated
	// https://www.glfw.org/docs/3.3/input_guide.html
	// reports when keyboard button is pressed/released
	glfwSetKeyCallback(m_gameWindow, keyboard_callback);
	// cursor position, measured in screen coordinates
	glfwSetCursorPosCallback(m_gameWindow, cursor_position_callback);
	// reports when mouse button is pressed/released
	glfwSetMouseButtonCallback(m_gameWindow, mouse_button_callback);
	// reports when mouse scroll is moved
	glfwSetScrollCallback(m_gameWindow, mouse_scroll_callback);
	// thies a pointer to the window object (here we pass "this" such that we can retrieve the Game object)
	// this allows us to retrieve the Game object in the callback functions
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
	// Next we want to create a vertex and fragment shader that actually processes data
	// shaders are written in the shader language "GLSL" (OpenGL Shading Language) which is a language very similar to C.
	// the language has it's own datatypes and input output features
	ResourceManager::LoadShader("vertexShaders.glsl", "fragmentShaders.glsl", nullptr, STD_SHADER);
	UIManager::getInstance().generateEngineUI();
}

void Game::Update(float dt)
{
	UIManager::getInstance().update(dt);
}

void Game::Render()
{
	// is a state-setting function
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// is a state-using function
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	UIManager::getInstance().RenderActiveScenes();
	UIManager::getInstance().RenderEngineUI();
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

	// shaders and textures
	loadResources();
	
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
