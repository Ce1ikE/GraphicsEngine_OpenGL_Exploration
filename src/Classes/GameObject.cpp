#include "UtilClasses/GameObject.h"

GameObject::GameObject()
{
	m_material = nullptr;
	m_mesh = nullptr;

	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);

	updateModelMatrix();
};

GameObject::GameObject(Mesh* mesh, Material* material) 
{
	m_mesh = mesh;
	m_material = material;

	m_position = glm::vec3(0.0f);
	m_rotation = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_model = glm::mat4(1.0f);
};

void GameObject::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	updateModelMatrix();
};

void GameObject::setRotation(const glm::quat& rot)
{
	m_rotation = rot;
	updateModelMatrix();
};

void GameObject::setScale(const glm::vec3& scale)
{
	m_scale = scale;
	updateModelMatrix();
};

void GameObject::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
};

void GameObject::setMaterial(Material* material)
{
	m_material = material;
};

glm::mat4 GameObject::getModel() { return m_model; };
glm::vec3 GameObject::getPosition() { return m_position; };
glm::quat GameObject::getRotation() { return m_rotation; };
glm::vec3 GameObject::getScale() { return m_scale; };
Mesh* GameObject::getMesh() { return m_mesh; };
Material* GameObject::getMaterial() { return m_material; };

void GameObject::updateModelMatrix()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_position);
	m_model = m_model * glm::mat4_cast(m_rotation);
	m_model = glm::scale(m_model, m_scale);
	// The order (Translate * Rotate * Scale) is for the local coordinate system
	// In matrix multiplication, this means: M = T * R * S
	// glm operations apply transformations from right to left if you multiply
	// the matrix by the new transformation: m_model = transform * m_model
	// but if you do m_model = m_model * transform, it applies in world space
	// The way done above is more intuitive and standard: build T, then multiply by R, then by S.
	// Or, more explicitly:
	/*
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 rotMatrix = glm::mat4_cast(m_rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		m_model = transMatrix * rotMatrix * scaleMatrix;
	*/
};

void GameObject::draw(const glm::mat4& view, const glm::mat4& projection)
{
	Shader& currentShader = m_material->use();
	currentShader.SetMatrix4("model", m_model);
	currentShader.SetMatrix4("view", view);
	currentShader.SetMatrix4("projection", projection);

	m_mesh->draw();
};
