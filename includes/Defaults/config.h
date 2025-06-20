#pragma once
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

/* STANDARD */
#define WINDOW_STD_WIDTH			800
#define WINDOW_STD_HEIGHT			600
#define WINDOW_STD_NAME				"LearnOpenGL"

/* LOGGING COLORS */
#define RED							"\033[38;5;196m"
#define GREEN						"\033[38;5;119m"
#define BLUE						"\033[38;5;21m"
#define VIOLET						"\033[38;5;129m"
#define PURPLE						"\033[38;5;90m"
#define PINK						"\033[38;5;198m"
#define CYAN						"\033[38;5;87m"
#define ORANGE						"\033[38;5;202m"
#define YELLOW						"\033[38;5;226m"
#define GOLD						"\033[38;5;172m"
#define TURQUOISE					"\033[38;5;37m"
#define WHITE						"\033[38;5;15m"
#define BLACK						"\033[38;5;0m"
#define GRAY						"\033[38;5;245m"
#define DARK_GRAY					"\033[38;5;236m"
#define LIGHT_RED					"\033[38;5;196m"
#define LIGHT_GREEN					"\033[38;5;119m"
#define LIGHT_BLUE					"\033[38;5;21m"
#define LIGHT_VIOLET				"\033[38;5;129m"
#define LIGHT_PURPLE				"\033[38;5;90m"
#define LIGHT_PINK					"\033[38;5;198m"
#define LIGHT_CYAN					"\033[38;5;87m"
#define LIGHT_ORANGE				"\033[38;5;202m"
#define LIGHT_YELLOW				"\033[38;5;226m"
#define LIGHT_GOLD					"\033[38;5;172m"
#define LIGHT_TURQUOISE				"\033[38;5;37m"
#define RESET_COLOR					"\033[0m"

/* PATHS */
#define ASSETS                      "assets/"
#define IMAGE_ASSETS                ASSETS + "images/"
#define MESH_ASSETS                 ASSETS + "meshes/"

#define JPG_ASSET(name)             IMAGE_ASSETS + name 
#define PNG_ASSET(name)             IMAGE_ASSETS + name 
#define OBJ_ASSET(name)             MESH_ASSETS + name 