#include "UtilClasses/Camera.h"

Camera::Camera() 
{
	m_view = glm::mat4(1.0f);
	m_projection = glm::mat4(1.0f);
};

glm::mat4& Camera::getView()
{
	return m_view;
};

glm::mat4& Camera::getProjection()
{
	return m_projection;
};