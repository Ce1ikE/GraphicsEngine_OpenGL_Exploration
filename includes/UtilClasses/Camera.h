#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	glm::mat4& getView();
	glm::mat4& getProjection();

private:
	glm::mat4 m_view;
	glm::mat4 m_projection;
};