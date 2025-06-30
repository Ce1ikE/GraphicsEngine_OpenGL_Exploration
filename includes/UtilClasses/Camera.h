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

	void processPan(float, float);
	void processOrbit(float, float);
	void processZoom(float);

	void updateProjection(float, float);
	void updateCamera(float, bool*);

private:
	void updatePosition();
	void updateView();

	// https://learnopengl.com/img/getting-started/camera_axes.png
	// m_cameraPosition describes the position (point) of the camera in worldspace 
	glm::vec3 m_cameraPosition;
	// m_cameraTarget describes the position (point) towards what the camera is aimed at in worldspace 
	glm::vec3 m_cameraTarget;
	// m_cameraFrontDirection describes a unit vector from m_cameraPosition towards where we're looking at derived from m_yaw and m_pitch
	// Target <-------- Pos
	// this gives us a direction from the camera's frame of reference and not worldspace
	// init values for camera rotation
	float m_yaw = -90.0f;
	float m_pitch = 0.0f;
	glm::vec3 m_cameraFrontDirection;
	// a constant vector indicating "up" in the world
	// required for the dot products to get other vectors (right , up) based upon only position and front
	glm::vec3 m_upWorldSpace = glm::vec3(0.0f, 1.0f, 0.0f);
	// a vector perpendicular to m_cameraFrontDirection and m_upWorldSpace
	glm::vec3 m_cameraRightDirection;
	// the camera's local "up" direction which is the cross product between m_cameraRightDirection m_cameraDirection
	glm::vec3 m_cameraUpDirection;
	// m_view & m_projection used by Shader objects to calculate their vertices in the vertex shader
	glm::mat4 m_view;
	glm::mat4 m_projection;
	// init values for how much the camera is affected by directional changes (rotate , translate)
	float m_panSensitivity = 0.05f;
	float m_orbitSensitivity = 0.8f;
	float m_zoomSensitivity = 1.5f;
	float m_distance = 10.0f;
};