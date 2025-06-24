#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Logger.h"
#include <map>

class Scene {
public:
    Scene();
    Scene(Camera*);
    void addGameObject(std::string,GameObject* obj);
    std::map<std::string, GameObject*>* getGameObjects();

    void setCamera(Camera* cam);
    Camera * getCamera();
    // The main rendering pass
    void renderScene(); 

    bool isActive;

    // For handling FBOs (Framebuffer Objects)
    // These are typically used for off-screen rendering (e.g., shadow maps, post-processing)
    // or rendering UI to a texture for ImGui
    // FBOs themselves would be encapsulated in a Framebuffer class
    // Framebuffer* createFramebuffer(int width, int height);


private:
    std::map<std::string, GameObject*> m_gameObjects;
    Camera* m_camera;
};