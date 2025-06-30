#pragma once

#include "Shader.h"
#include "Texture2D.h"

#include <string>
#include <map>

class Material {
public:
    // Takes a pointer to a Shader from ResourceManager
    Material(Shader* shader); 
    // For different texture types (diffuse, specular)
    void addTexture(const std::string& name, Texture2D* texture); 
    // For uniform colors/vectors
    void setVec3(const std::string& name, const glm::vec3& value); 
    // ... more uniform setters (glUniform)

    // Activates the shader and binds textures/sets uniforms
    Shader& use() const;

private:
    Shader* m_shader;
    // e.g., "texture_diffuse", "texture_specular"
    std::map<std::string, Texture2D*> m_textures; 
    // Other material properties (colors, floats, etc.)
};
