#pragma once

#include <map>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Scene.h"

class BaseUIElement {
public:
	virtual ~BaseUIElement() = default;

	// This method will be overridden by derived classes to draw the specific ImGui widget.
	// It should return true if the widget's value changed or it was interacted with
	// (e.g., button clicked, slider moved).
	virtual bool render() = 0;
private:
    //...
};

class UIButton : public BaseUIElement {
public:
    UIButton(std::string&);
    
    bool render() override;
    bool isClicked();

private:
    std::string m_label;
    bool m_clickedThisFrame = false; // Internal state
};

// Derived class for a Float Slider (e.g., for a 'float' uniform)
class UISliderFloat : public BaseUIElement {
public:
    UISliderFloat(std::string&, float, float, float);
    
    bool render() override;
    float getValue();
    void setValue(float);

private:
    std::string m_label;
    float m_value; // The actual value the slider controls
    float m_min;
    float m_max;
};

// Derived class for a Vec3 Slider (e.g., for a 'vec3' uniform)
class UISliderVec3 : public BaseUIElement {
public:
    UISliderVec3(std::string&, glm::vec3&, float, float);
    
    bool render() override;
    glm::vec3 getValue();
    void setValue(glm::vec3&);

private:
    std::string m_label;
    glm::vec3 m_value;
    float m_min;
    float m_max;
};

class UISelect : public BaseUIElement {
public:
    UISelect(std::string&, std::vector<std::string>&);
    bool isChanged();
    int getSelected();
    bool render() override;

private:
    std::string m_label;
    int m_selected;
    int m_previous;
    std::vector<std::string> m_selectables;
};


class UIManager {
public:
	// Scene storage
	static std::map<std::string, Scene*>		Scenes;
    // UI storage
    static std::map<std::string, std::unique_ptr<BaseUIElement>> UIElements;
    // a flag to indicate if any shader parameter uniform was changed via UI this frame
    static bool m_shaderUniformsChangedThisFrame;

	// renders the current active Scene and UI interface
	static void RenderActiveScenes();
	
	static void StartFrame();
	static void RenderGameUI();
	static void EndFrame();

    // Method to add elements to the map (typically called by generateUIFromShader)
    static void addUIElement(std::string&, std::unique_ptr<BaseUIElement>);
    // Method to clear all dynamically generated UI elements
    static void clearDynamicUIElements();

    // Method to generate UI elements based on a Shader's uniforms
    static void generateUIFromShader(Shader*); // Forward declare Shader if needed
    static bool hasShaderUniformsChanged();
    static void resetShaderUniformsChangedFlag() { m_shaderUniformsChangedThisFrame = false; }

    static void generateUI();

    // Getters to retrieve specific element types for their values (requires dynamic_cast)
    static UIButton* getButton(std::string&);
    static UISliderFloat* getSliderFloat(std::string&);
    static UISliderVec3* getSliderVec3(std::string&);
    static UISelect* UIManager::getSelect(std::string&);


	// activates the UI 
	static void Init(GLFWwindow*);

	static void addScene(Scene*,std::string);
	~UIManager();
private:
	// private constructor, that is we do not want any actual UI manager objects. Its members and functions should be publicly available (static).
    UIManager() { m_shaderUniformsChangedThisFrame = false; };
};