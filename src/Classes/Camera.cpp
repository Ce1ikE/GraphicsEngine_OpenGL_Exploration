#include "Camera.h"

Camera::Camera() 
{
	// We can use orthographic or perspective projection for our scene
	m_projection = glm::mat4(1.0f);
	
	// orthographic:
	// -------------
	// https://glm.g-truc.net/0.9.9/api/a00665.html#ga6615d8a9d39432e279c4575313ecb456
	// param 1 & 2 => left and right coordinate of the frustum 
	// param 3 & 4 => bottom and top coordinate of the frustum 
	// param 5 & 6 => distance between the near and far plane 
	// m_projection = glm::ortho(0.0f, (float)WINDOW_STD_HEIGHT , 0.0f, (float)WINDOW_STD_WIDTH, 0.1f, 100.0f);
	
	// perspective:
	// ------------
	// https://glm.g-truc.net/0.9.9/api/a00665.html#ga747c8cf99458663dd7ad1bb3a2f07787
	// param 1 => fov (field of view) value
	// param 2 => aspect ratio 
	// param 3 & 4 => distance between the near and far plane 
	m_projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_STD_WIDTH / (float)WINDOW_STD_HEIGHT, 0.1f, 100.0f);
	
	// 3 Major matrices should be noted (the MVP's):
	// 1) Model Matrix		-> This matrix transforms vertices from a model/mesh's local space to world space
	// 2) View Matrix		-> This matrix represents the camera's position and orientation transforming vertices from world space into the camera's view space
	// 3) Projection Matrix -> This matrix defines how the 3D scene is projected onto a 2D screen (e.g.: perspective or orthographic) transforming view space into clip space (because well... it clips uncessary objects that don't need to be rendered)

	// then we just have to modify these matrices once per object and once per frame 
	// and in our vertex shader we multiply these matrices => projection * view * model * vertexPosition
	// https://learnopengl.com/img/getting-started/coordinate_systems.png
	// http://www.songho.ca/opengl/gl_projectionmatrix.html

	// https://learnopengl.com/Getting-started/Camera
	// By convention, OpenGL is a right-handed system.
	// What this basically says is that :
	// 1) the positive x-axis is to your right, 
	// 2) the positive y-axis is up 
	// 3) the positive z-axis is backwards. 
	// Think of your screen being the center of the 3 axes and 
	// the positive z-axis going through your screen towards you. 
	m_view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move the camera
	// "To move a camera backwards, is the same as moving the entire scene forward"

	m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

	UpdateDirection();
};

glm::mat4& Camera::getView()
{
	return m_view;
};

glm::mat4& Camera::getProjection()
{
	return m_projection;
};

void Camera::setView(glm::mat4& newView)
{
	m_view = newView;
};

void Camera::setProjection(glm::mat4& newProjection)
{
	m_projection = newProjection;
};

void Camera::setCameraPosition(glm::vec3& newPosition)
{
	m_cameraPosition = newPosition;
	UpdateDirection();
};

glm::vec3 Camera::getCameraPosition()
{
	return m_cameraPosition;
};

void Camera::setCameraTarget(glm::vec3& newTarget)
{
	m_cameraTarget = newTarget;
	UpdateDirection();
};

glm::vec3 Camera::getCameraTarget()
{
	return m_cameraTarget;
};

void Camera::UpdateView()
{
	glm::vec3 upWorldSpace = glm::vec3(0.0f, 1.0f, 0.0f);

	m_view = glm::lookAt(
		m_cameraPosition,
		m_cameraDirection,
		upWorldSpace
	);
};

void Camera::UpdateDirection()
{
	m_cameraDirection = glm::normalize(m_cameraPosition - m_cameraTarget);
	UpdateView();
};

void Camera::UpdateCamera(float dt,bool* keys)
{
	glm::vec3 upWorldSpace = glm::vec3(0.0f, 1.0f, 0.0f);
	bool cameraMoved = false;
	if (keys[GLFW_KEY_W] == true)
	{
		cameraMoved = true;
		Logger::info(
			MESSAGE("W key pressed")
		);
		m_cameraPosition += m_cameraSpeed * m_cameraDirection * dt;
	}
	if (keys[GLFW_KEY_S] == true)
	{
		cameraMoved = true;
		Logger::info(
			MESSAGE("S key pressed")
		);		
		m_cameraPosition -= m_cameraSpeed * m_cameraDirection * dt;
	}
	if (keys[GLFW_KEY_A] == true)
	{
		cameraMoved = true;
		Logger::info(
			MESSAGE("A key pressed")
		);
		m_cameraPosition -= glm::normalize(glm::cross(m_cameraDirection, upWorldSpace)) * m_cameraSpeed * dt;
	}
	if (keys[GLFW_KEY_D] == true)
	{
		cameraMoved = true;
		Logger::info(
			MESSAGE("D key pressed")
		);
		m_cameraPosition += glm::normalize(glm::cross(m_cameraDirection, upWorldSpace)) * m_cameraSpeed * dt;
	}

	if(cameraMoved)
	{
		UpdateDirection();
		Logger::info(
			MESSAGE(std::string("X: " + std::to_string(m_cameraPosition.x) + "Z: " + std::to_string(m_cameraPosition.z)))
		);
	}
};