#include "ResourceClasses/Square.h"

Square::Square()
{
	std::vector<Vertex> vertices = {
		// positions						 // colors
		Vertex(glm::vec3( 0.5f,  0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f)), // top right 
		Vertex(glm::vec3( 0.5f, -0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f)), // top right
		Vertex(glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f)), // bottom left 
		Vertex(glm::vec3(-0.5f,  0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f)), // bottom left
	};

	std::vector<unsigned int> indices = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GameObject::setMesh(new Mesh(vertices, indices));
	GameObject::setMaterial(new Material(ResourceManager::GetShader("basic")));
};