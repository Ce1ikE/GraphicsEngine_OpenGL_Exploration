#include "Axis.h"

Axis::Axis(float length)
{
	std::vector<Vertex> vertices = {
		// positions                                 // colors
		Vertex(glm::vec3(0.0f, 0.0f, 0.0f),          glm::vec3(1.0f, 0.0f, 0.0f)), // X-axis
		Vertex(glm::vec3(length * 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)), // X-axis

		Vertex(glm::vec3(0.0f, 0.0f, 0.0f),          glm::vec3(0.0f, 1.0f, 0.0f)), // Y-axis
		Vertex(glm::vec3(0.0f, length * 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), // Y-axis

		Vertex(glm::vec3(0.0f, 0.0f, 0.0f),          glm::vec3(0.0f, 0.0f, 1.0f)), // Z-axis
		Vertex(glm::vec3(0.0f, 0.0f, length * 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // Z-axis
	};

	GameObject::setMesh(new Mesh(vertices));
	GameObject::setMaterial(new Material(ResourceManager::GetShader(STD_SHADER)));
};

Axis::Axis(float length, glm::vec3& axisPosition)
	:Axis(length)
{
	setPosition(axisPosition);
};


void Axis::draw(const glm::mat4& view, const glm::mat4& projection)
{
	Shader& currentShader = GameObject::getMaterial()->use();
	currentShader.SetMatrix4("model", GameObject::getModel());
	currentShader.SetMatrix4("view", view);
	currentShader.SetMatrix4("projection", projection);

	GameObject::getMesh()->draw(false);
};