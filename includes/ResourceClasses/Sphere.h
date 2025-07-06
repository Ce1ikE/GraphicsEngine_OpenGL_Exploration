#pragma once

#include "GameObject.h"
#include "ResourceManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Sphere : public GameObject
{
public:
	Sphere(int, int, int);
	Sphere(int, int, int,glm::vec3&);
private:
};