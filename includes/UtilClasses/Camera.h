#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera();

	glm::mat4& getView();
	void setView(glm::mat4&);
	glm::mat4& getProjection();
	void setProjection(glm::mat4&);

private:
	glm::mat4 m_view;
	glm::mat4 m_projection;
};