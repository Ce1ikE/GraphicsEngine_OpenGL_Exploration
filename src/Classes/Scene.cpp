#include "UtilClasses/Scene.h"

Scene::Scene() 
{
    isActive = true;
};

void Scene::addGameObject(GameObject* obj)
{
    m_gameObjects.push_back(obj);
    Logger::succes(
        MESSAGE("Added new GameObject to scene")
    );
};

void Scene::setCamera(Camera* cam)
{
	m_camera = cam;
};

void Scene::renderScene()
{
    for (auto iter : m_gameObjects)
    {
        iter->draw(
            m_camera->getView(),
            m_camera->getProjection()
        );
    }
};
