#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>

#include "Logger.h"

struct Vertex {
    Vertex(){};
    Vertex(glm::vec3 pos, glm::vec3 clr)
    {
        m_position = pos;
        m_normal = clr;
    };

    glm::vec3 m_position;
    glm::vec3 m_normal;
    // glm::vec2 m_texCoords;
    // ...
    // can have more attributes as needed (e.g., Tangent, Bitangent, BoneIDs, Weights)
};

class Mesh {
public:
    Mesh();
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices = {});
    ~Mesh();

    // Unbinds/Binds the VAO
    void bind() const; 
    void unbind() const;

    // clears the VAO , VBO and EBO (if any) from GPU memory
    void freeResources();

    // Calls glDrawElements or glDrawArrays
    void draw(bool drawTriangles = true) const; 

    // For dynamic data (e.g., particles, or deforming meshes)
    void updateVertices(const std::vector<Vertex>& newVertices);

private:
    unsigned int m_VAO_ID, m_VBO_ID, m_EBO_ID; // OpenGL IDs
    size_t m_indexCount;
    bool m_isIndexed;
    size_t m_vertexCount;

    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};
