#pragma once

#include "GameObject.h"
#include "ResourceManager.h"

class Axis : public GameObject
{
public:
	Axis(float);
	Axis(float,glm::vec3&);
	void virtual draw(const glm::mat4&, const glm::mat4&) override;
private:
};