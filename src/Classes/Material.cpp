#include "ResourceClasses/Material.h"

Material::Material(Shader* shader)
{
	m_shader = shader;
};

void Material::addTexture(const std::string& name, Texture2D* texture)
{
	m_textures[name] = texture;
};

void Material::setVec3(const std::string& name, const glm::vec3& value)
{
	int location = glGetUniformLocation(m_shader->ID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
};

Shader& Material::use() const
{
	return m_shader->Use();
};