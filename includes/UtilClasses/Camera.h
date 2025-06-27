#pragma once
#include "config.h"
#include "Logger.h"
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

	void setCameraPosition(glm::vec3&);
	glm::vec3 getCameraPosition();
	void setCameraTarget(glm::vec3&);
	glm::vec3 getCameraTarget();

	void UpdateCamera(float, bool*);

private:
	void UpdateView();
	void UpdateDirection();

	// https://learnopengl.com/img/getting-started/camera_axes.png
	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraDirection;
	float m_cameraSpeed = 10.0f;

	glm::mat4 m_view;
	glm::mat4 m_projection;
};