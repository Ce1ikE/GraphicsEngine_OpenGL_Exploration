#include "Camera.h"

Camera::Camera() 
{
	// We can use orthographic or perspective projection for our scene
	m_projection = glm::mat4(1.0f);
	updateProjection((float)WINDOW_STD_WIDTH,(float)WINDOW_STD_HEIGHT);
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
	m_cameraPosition = glm::vec3(0.0f, 0.0f, m_distance);
	//updatePosition();
	updateView();
};

void Camera::updatePosition()
{
	// to prevent glimbal lock/flipping
	// https://computergraphics.stackexchange.com/questions/12273/gimbal-lock-confusion
	// https://fliponline.blogspot.com/2007/04/quick-trick-gimbal-lock-just-ignore-it.html
	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
	
	float yaw_rad = glm::radians(m_yaw);
	float pitch_rad = glm::radians(m_pitch);
	// we calculate the position relative to target using spherical coordinates
	// so are target is in the center of the sphere while our position lies upon the sphere
	float x = m_distance * cos(pitch_rad) * cos(yaw_rad);
	float y = m_distance * sin(pitch_rad);
	float z = m_distance * cos(pitch_rad) * sin(yaw_rad);
	// https://docs.blender.org/manual/en/2.93/editors/3dview/navigate/walk_fly.html#id1
	m_cameraPosition = m_cameraTarget + glm::vec3(x,y,z);
};

	// usage : MMB
void Camera::processOrbit(float xoffset, float yoffset)
{
	// move mouse left/right for yaw (around Y-axis)
	m_yaw += xoffset * m_orbitSensitivity;
	// move mouse up/down for pitch (around X-axis)
	m_pitch -= yoffset * m_orbitSensitivity;
	updatePosition();
	updateView();
};

	// usage : Shift + MMB
void Camera::processPan(float xoffset, float yoffset)
{
	// to pan, we need the camera's current right and up vectors.
	// these are derived from the front direction relative to the world up
	m_cameraFrontDirection = glm::normalize(m_cameraTarget - m_cameraPosition);
	m_cameraRightDirection = glm::normalize(glm::cross(m_cameraFrontDirection, m_upWorldSpace));
	m_cameraUpDirection = glm::normalize(glm::cross(m_cameraRightDirection, m_cameraFrontDirection));

	glm::vec3 translation{ 0.0f,0.0f,0.0f };
	translation += m_cameraRightDirection * (-xoffset * m_panSensitivity);
	translation += m_cameraUpDirection * (-yoffset * m_panSensitivity);

	m_cameraTarget += translation;

	updatePosition();
	updateView(); 
};

void Camera::processZoom(float yoffset)
{
	m_distance -= yoffset * m_zoomSensitivity;
	// prevent zooming too close or too far
	glm::clamp(m_distance, 0.1f, 200.0f);
	
	updatePosition();
	updateView();
};

void Camera::updateProjection(float width , float height)
{
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
	m_projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	// 3 Major matrices should be noted (the MVP's):
	// 1) Model Matrix		-> This matrix transforms vertices from a model/mesh's local space to world space
	// 2) View Matrix		-> This matrix represents the camera's position and orientation transforming vertices from world space into the camera's view space
	// 3) Projection Matrix -> This matrix defines how the 3D scene is projected onto a 2D screen (e.g.: perspective or orthographic) transforming view space into clip space (because well... it clips uncessary objects that don't need to be rendered)

	// then we just have to modify these matrices once per object and once per frame 
	// and in our vertex shader we multiply these matrices => projection * view * model * vertexPosition
	// https://learnopengl.com/img/getting-started/coordinate_systems.png
	// http://www.songho.ca/opengl/gl_projectionmatrix.html
};

void Camera::updateView()
{
	Logger::info(
		MESSAGE("X: " + std::to_string(m_cameraPosition.x) + " Y: " + std::to_string(m_cameraPosition.y) + " Z: " + std::to_string(m_cameraPosition.z))
	);
	// takes the position from which you want to look at
	// target position you want to look at
	// and the vector which tells us what is up in worldspace
	m_view = glm::lookAt(
		m_cameraPosition,
		m_cameraTarget,
		m_upWorldSpace
	);
};


void Camera::updateCamera(float dt,bool* keys)
{
	//bool cameraMoved = false;
	//if (keys[GLFW_KEY_LEFT_SHIFT] == true)
	//{
	//	m_cameraSpeed = 20.0f;
	//}
	//if (keys[GLFW_KEY_LEFT_SHIFT] == false)
	//{
	//	m_cameraSpeed = 10.0f;
	//}

	//if (keys[GLFW_KEY_W] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition += m_cameraSpeed * m_cameraFrontDirection * dt;
	//}
	//if (keys[GLFW_KEY_S] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition -= m_cameraSpeed * m_cameraFrontDirection * dt;
	//}
	//
	//if (keys[GLFW_KEY_A] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition -= m_cameraSpeed  * m_cameraRightDirection * dt;
	//}
	//if (keys[GLFW_KEY_D] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition += m_cameraSpeed  * m_cameraRightDirection * dt;
	//}

	//if (keys[GLFW_KEY_SPACE] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition += m_cameraSpeed * m_upWorldSpace * dt;
	//}
	//if (keys[GLFW_KEY_LEFT_CONTROL] == true)
	//{
	//	cameraMoved = true;
	//	m_cameraPosition -= m_cameraSpeed * m_upWorldSpace * dt;
	//}

	//UpdateView();
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
};

glm::vec3 Camera::getCameraPosition()
{
	return m_cameraPosition;
};

void Camera::setCameraTarget(glm::vec3& newTarget)
{
	m_cameraTarget = newTarget;
};

glm::vec3 Camera::getCameraTarget()
{
	return m_cameraTarget;
};
