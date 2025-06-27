#pragma once

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "config.h"
#include "callbacks.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "Logger.h"
#include "Scene.h"
#include "Cube.h"

class Game
{
public:
    // game state
    GameState    m_state;
    bool         m_keys[1024];
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    std::string  m_windowName;
    GLFWwindow*  m_gameWindow;

    // constructor/destructor
    Game(unsigned int width, unsigned int height,std::string name);
    ~Game();

    // initialize game state (load all shaders/textures/levels)
    void Init();

    // game loop
    void Run();
    void Update(float dt);
    void Render();

    // member functions to handle the actual event logic
    // these will be called by the static wrappers
    void handleFramebufferSize(GLFWwindow* window, int width, int height);
    void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mode);


private:
    // initialize UI elements
    void UISetup();
    // add all callbacks to the window
    void registerCallbacks();
    // initialize the window context
    void createWindow();
    // initialize the game resources (shader and textures)
    void loadResources();
    // ...
    void UpdateObjects(float dt);
};

