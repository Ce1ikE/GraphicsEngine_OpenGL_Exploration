#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Logger.h"

class Scene {
public:
    Scene();
    void addGameObject(GameObject* obj);
    void setCamera(Camera* cam);

    // The main rendering pass
    void renderScene(); 

    bool isActive;

    // For handling FBOs (Framebuffer Objects)
    // These are typically used for off-screen rendering (e.g., shadow maps, post-processing)
    // or rendering UI to a texture for ImGui
    // FBOs themselves would be encapsulated in a Framebuffer class
    // Framebuffer* createFramebuffer(int width, int height);


private:
    std::vector<GameObject*> m_gameObjects;
    Camera* m_camera;
};