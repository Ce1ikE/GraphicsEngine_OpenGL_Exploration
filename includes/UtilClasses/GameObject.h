#pragma once

#include "Mesh.h"
#include "Material.h"
#include <glm/gtc/matrix_transform.hpp>

class GameObject {
public:
    GameObject();
    GameObject(Mesh* mesh, Material* material);

    void setPosition(const glm::vec3&);
    void setRotation(const glm::quat&); // Use quaternions for rotation!
    void setScale(const glm::vec3&);
    void setMesh(Mesh*);
    void setMaterial(Material*);
    glm::mat4 getModel();
    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();
    Mesh* getMesh();
    Material* getMaterial();

    // Updates the internal model matrix
    void updateModelMatrix();
    // Update modelMatrix


    // Draws the object
    virtual void draw(const glm::mat4&, const glm::mat4&);


    // Optionally, children for hierarchical transforms (e.g., robot arm)
    // std::vector<GameObject*> children;
    // GameObject* parent;

private:
    Mesh* m_mesh;
    Material* m_material;

    glm::vec3 m_position;
    glm::quat m_rotation; // Better for rotations than Euler angles
    glm::vec3 m_scale;
    glm::mat4 m_model; // The calculated model matrix
};