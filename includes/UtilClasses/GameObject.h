#pragma once

#include "ResourceClasses/Mesh.h"
#include "ResourceClasses/Material.h"

class GameObject {
public:
    GameObject();
    GameObject(Mesh* mesh, Material* material);

    void setPosition(const glm::vec3&);
    void setRotation(const glm::quat&); // Use quaternions for rotation!
    void setScale(const glm::vec3&);
    void setMesh(Mesh*);
    void setMaterial(Material*);

    // Updates the internal model matrix
    void updateModelMatrix();

    // Draws the object
    void draw(const glm::mat4&, const glm::mat4&) const;

    // Optionally, children for hierarchical transforms (e.g., robot arm)
    // std::vector<GameObject*> children;
    // GameObject* parent;

private:
    Mesh* m_mesh;
    Material* m_material;

    glm::vec3 m_position;
    glm::quat m_rotation; // Better for rotations than Euler angles
    glm::vec3 m_scale;
    glm::mat4 m_modelMatrix; // The calculated model matrix
};