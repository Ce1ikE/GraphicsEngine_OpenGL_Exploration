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
    // game state (helps to order you're engine/game : "what do you want to update/draw" ?)
    GameState    m_state;
    // 2 numbers keeping track of the mouse last recordered coordinates
    double       m_lastX{ WINDOW_STD_WIDTH / 2 }, m_lastY{ WINDOW_STD_HEIGHT / 2 };
    // a array that keeps track wether a keyboard button was pressed or released
    bool         m_keys[1024];
    bool         m_mouseKeys[8];
    // window related variables
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    std::string  m_windowName;
    GLFWwindow*  m_gameWindow;
    // keeps track of certain features such as V-sync or depth test
    bool         m_enabledVSync = true;
    bool         m_enabledDepthTest = true;
    bool         m_enabledCaptureCursor = true;

    // constructor/destructor
    Game(unsigned int width, unsigned int height,std::string name);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // main game loop functions
    void Run();
    void Update(float dt);
    void Render();

private:
    // add all callbacks to the window
    void registerCallbacks();
    // initialize the window context
    void createWindow();
    // initialize the game resources (shader and textures)
    void loadResources();
};

