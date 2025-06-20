#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "UtilClasses/Game.h"

// --- Static Callback Wrapper Implementations ---
// these functions will be passed to GLFW 
// 1) they retrieve the Game instance
// 2) from the window's user pointer and then call the appropriate member function of the Game instance
// this allows the Game to handle it's own business 

void framebuffer_size_callback(GLFWwindow*, int, int);

void key_callback(GLFWwindow*, int, int, int, int);