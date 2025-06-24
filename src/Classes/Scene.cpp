#include "UtilClasses/Scene.h"

Scene::Scene() 
{
    isActive = true;
};

Scene::Scene(Camera * newCamera)
{
    m_camera = newCamera;
    isActive = true;
};

void Scene::addGameObject(std::string gObjName,GameObject * gObj)
{
    m_gameObjects[gObjName] = gObj;
    Logger::succes(
        MESSAGE("Added new GameObject to scene")
    );
};

std::map<std::string, GameObject*> * Scene::getGameObjects()
{
    return &m_gameObjects;
};

void Scene::setCamera(Camera* cam)
{
	m_camera = cam;
};

Camera * Scene::getCamera()
{
    return m_camera;
};

void Scene::renderScene()
{
    for (auto iter : m_gameObjects)
    {
        iter.second->draw(
            m_camera->getView(),
            m_camera->getProjection()
        );
    }
};
