#include "UtilClasses/GameObject.h"

GameObject::GameObject()
{
	m_material = nullptr;
	m_mesh = nullptr;

	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(1.0f);
	m_scale = glm::vec3(1.0f);
	m_modelMatrix = glm::mat4(1.0f);
};

GameObject::GameObject(Mesh* mesh, Material* material) 
{
	m_mesh = mesh;
	m_material = material;

	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(1.0f);
	m_scale = glm::vec3(1.0f);
	m_modelMatrix = glm::mat4(1.0f);
};

void GameObject::setPosition(const glm::vec3& pos)
{
	m_position = pos;
};

void GameObject::setRotation(const glm::quat& rot)
{
	m_rotation = rot;
}; 

void GameObject::setScale(const glm::vec3& scale)
{
	m_scale = scale;
};

void GameObject::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
};

void GameObject::setMaterial(Material* material)
{
	m_material = material;
};

void GameObject::updateModelMatrix()
{
	m_mesh->unbind();
	// ...
	m_mesh->bind();
};

void GameObject::draw(const glm::mat4& view, const glm::mat4& projection) const
{
	Shader& currentShader = m_material->use();
	currentShader.SetMatrix4("model", m_modelMatrix);
	currentShader.SetMatrix4("view", view);
	currentShader.SetMatrix4("projection", projection);

	m_mesh->draw();
};
