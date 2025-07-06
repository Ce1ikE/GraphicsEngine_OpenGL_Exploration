#pragma once

#include <map>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "UIButton.h"
#include "UISliderFloat.h"
#include "UISliderVec3.h"
#include "UISelect.h"
#include "UIPanel.h"

enum class UIElementType {
    BUTTON,
    SLIDER_FLOAT,
    SLIDER_VEC2,
    SLIDER_VEC3,
    SLIDER_VEC4,
    CHECKBOX,
    LABEL,
    // For dropdowns/listboxes
    SELECT, 
    TEXT_INPUT,
    // ...
};

class UIManager {
public:
	// Scene storage
	static std::map<std::string, Scene*> Scenes;
    // UI storage
    static std::map<std::string, UIPanel*> Panels;

    static UIManager& getInstance();

    // a flag to indicate if any shader parameter uniform was changed via UI this frame
    bool m_shaderUniformsChangedThisFrame = false;
    void resetShaderUniformsChangedFlag() { m_shaderUniformsChangedThisFrame = false; };
    
	// activates the UI 
	void Init(GLFWwindow*);
    void Shutdown();
    ~UIManager();

	// renders the current active Scene and UI interface
	void addScene(Scene*,std::string);
	void RenderActiveScenes();
	
    void StartFrame();
	void EndFrame();
	void RenderEngineUI();

    void generateEngineUI();
    void populateShaderListPanel(); 
    void populateGameObjectListPanel(); 
    void populateShaderInfoPanel(Shader* shader, std::string shaderName);
    void populateGameObjectInfoPanel(GameObject* gameObject, std::string gameObjectName);
    void populateFeaturesPanel(); 

    void addUIPanel(UIPanel*,std::string);
    UIPanel* getUIPanel(std::string&);

    void update(float);

private:
    // private constructor, that is we do not want any actual UI manager objects. Its members and functions should be publicly available (static).
    UIManager() { m_shaderUniformsChangedThisFrame = false; };
};