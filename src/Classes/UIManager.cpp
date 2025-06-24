#include "UtilClasses/UIManager.h"


std::map<std::string, Scene>		UIManager::Scenes;
std::map<std::string, std::unique_ptr<BaseUIElement>> UIManager::UIElements;
bool UIManager::m_shaderUniformsChangedThisFrame = false;

// renders the current active Scene and UI interface
void UIManager::RenderActiveScenes()
{
	for (auto scene : Scenes)
	{
		if (scene.second.isActive)
		{
			scene.second.renderScene();
		}
		else
		{
			Logger::info(
				MESSAGE("SCENE: " + scene.first + " not rendered")
			);
		}
	}
};

void UIManager::StartFrame()
{
	// Per-frame setup for ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// this creates an ImGui window for the controls
	ImGui::Begin("Controls");
};

void UIManager::EndFrame()
{
	ImGui::End();
	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

void UIManager::RenderGameUI()
{
    // Reset flag at start of frame's UI rendering
    m_shaderUniformsChangedThisFrame = false; 

    // Iterates through and renders all dynamically added UI elements
    for (auto const& [name, element_ptr] : UIElements) {
        // we call render on the base pointer, polymorphism handles the rest
        if (element_ptr->render()) { 
            // Set flag if any element was interacted with
            m_shaderUniformsChangedThisFrame = true; 
            Logger::info(
                MESSAGE("Uniform value was changed")
            );
        }
    }

};

void UIManager::Init(GLFWwindow* window)
{
	// https://github.com/ocornut/imgui
	// https://github.com/ocornut/imgui/wiki/Getting-Started
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// theme
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");
};

void UIManager::addScene(Scene* newScene, std::string name)
{
	Scenes[name] = *newScene;
};

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Logger::succes(MESSAGE("ImGui cleanup complete"));
};

void UIManager::addUIElement(std::string& name, std::unique_ptr<BaseUIElement> element) {
    UIElements[name] = std::move(element);
    Logger::info(
        MESSAGE("Added new element " + name),
        TURQUOISE
    );
}

bool UIManager::hasShaderUniformsChanged()
{ 
    return m_shaderUniformsChangedThisFrame; 
}

void UIManager::clearDynamicUIElements() {
    UIElements.clear(); // unique_ptr automatically call destructors
}

void UIManager::generateUIFromShader(Shader* shader) {
    // clear existing UI elements for the old shader
    // before adding new buttons
    clearDynamicUIElements(); 

    if (!shader || shader->ID == 0) {
        return;
    }

    // default button to recompile shaders
    addUIElement(std::string("RecompileShaderButton"), std::make_unique<UIButton>(std::string("Recompile Current Shader")));

    GLint numUniforms = 0;
    GLint numAttributes = 0;
    glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(shader->ID, GL_ACTIVE_ATTRIBUTES, &numAttributes);


    char nameBuffer[256]; // Buffer for uniform name
    GLsizei length;       // Length of uniform name
    GLint size;           // Size of uniform (e.g., array size)
    GLenum type;          // Type of uniform (GL_FLOAT, GL_FLOAT_VEC3, etc.)

    Logger::info(
        MESSAGE("Number of uniforms active in shader program " + std::to_string(numUniforms))
    );
    Logger::info(
        MESSAGE("Number of attributes active in shader program " + std::to_string(numAttributes))
    );

    for (int i = 0; i < numUniforms; ++i) {
        glGetActiveUniform(shader->ID, i, sizeof(nameBuffer), &length, &size, &type, nameBuffer);
        std::string uniformName(nameBuffer, length);

        // Filter out built-in uniforms or matrices you handle separately (model, view, projection)
        if (uniformName.rfind("gl_", 0) == 0 ||
            uniformName == "model" || uniformName == "view" || uniformName == "projection" ||
            uniformName.find("[") != std::string::npos) // Skip array uniforms for simplicity for now
        {
            Logger::info(
                MESSAGE("Skipped uniform:  " + uniformName),
                TURQUOISE
            );
            continue;
        }

        // Dynamically create UI elements based on uniform type
        switch (type) {
        case GL_FLOAT:
            addUIElement(uniformName, std::make_unique<UISliderFloat>(uniformName, 0.0f, -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC2:
            //addUIElement(uniformName, std::make_unique<UISliderVec2>(uniformName, glm::vec2(0.0f), -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC3:
            addUIElement(uniformName, std::make_unique<UISliderVec3>(uniformName, glm::vec3(0.0f), -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC4:
            //addUIElement(uniformName, std::make_unique<UISliderVec4>(uniformName, glm::vec4(0.0f), -10.0f, 10.0f));
            break;
        case GL_INT:
        case GL_SAMPLER_2D: // Samplers usually map to texture slots or file paths
            // Add specific UI for integers or textures if needed
            //addUIElement(uniformName, std::make_unique<UILabel>(uniformName + " (Sampler/Int)"));
            break;
        case GL_BOOL:
            //addUIElement(uniformName, std::make_unique<UICheckbox>(uniformName, false)); // Assuming a UICheckbox class
            break;
        default:
            //addUIElement(uniformName, std::make_unique<UILabel>(uniformName + " (unsupported type)"));
            break;
        }
    }
}

void UIManager::generateUI()
{
    addUIElement(std::string("ViewMode"), std::make_unique<UISelect>(std::string("Enable Wireframe View"),std::vector<std::string>({"line","fill"})));
};

UIButton* UIManager::getButton(std::string& name) {
    auto it = UIElements.find(name);
    if (it != UIElements.end()) {
        return dynamic_cast<UIButton*>(it->second.get());
    }
    return nullptr;
}
UISliderFloat* UIManager::getSliderFloat(std::string& name)
{
    auto it = UIElements.find(name);
    if (it != UIElements.end()) {
        return dynamic_cast<UISliderFloat*>(it->second.get());
    }
    return nullptr;
};
UISliderVec3* UIManager::getSliderVec3(std::string& name)
{
    auto it = UIElements.find(name);
    if (it != UIElements.end()) {
        return dynamic_cast<UISliderVec3*>(it->second.get());
    }
    return nullptr;
};
UISelect* UIManager::getSelect(std::string& name)
{
    auto it = UIElements.find(name);
    if (it != UIElements.end()) {
        return dynamic_cast<UISelect*>(it->second.get());
    }
    return nullptr;
};


UIButton::UIButton(std::string& label) 
	: m_label(label) 
{

}
bool UIButton::render() 
{
	m_clickedThisFrame = ImGui::Button(m_label.c_str());
	return m_clickedThisFrame;
}
bool UIButton::isClicked()
{ 
	return m_clickedThisFrame; 
}

UISliderFloat::UISliderFloat(std::string& label, float initialValue, float min, float max)
    : m_label(label), m_value(initialValue), m_min(min), m_max(max) 
{

}
bool UISliderFloat::render() 
{
    return ImGui::SliderFloat(m_label.c_str(), &m_value, m_min, m_max);
}
float UISliderFloat::getValue() 
{ 
    return m_value; 
}
void UISliderFloat::setValue(float val) 
{ 
    m_value = val; 
} 

UISliderVec3::UISliderVec3(std::string& label,glm::vec3& initialValue, float min, float max)
    : m_label(label), m_value(initialValue), m_min(min), m_max(max) 
{

}
bool UISliderVec3::render() 
{
    return ImGui::SliderFloat3(m_label.c_str(), glm::value_ptr(m_value), m_min, m_max);
}
glm::vec3 UISliderVec3::getValue() 
{ 
    return m_value; 
}
void UISliderVec3::setValue(glm::vec3& val) 
{ 
    m_value = val; 
}

UISelect::UISelect(std::string& label, std::vector<std::string>& selectables)
{
    m_label = label;
    m_selectables = selectables;
    m_selected = -1;
    m_previous = -1;
};
bool UISelect::isChanged()
{
    bool changed = m_previous != m_selected;
    m_previous = m_selected;
    return changed;
};
int UISelect::getSelected()
{
    return m_selected;
};
bool UISelect::render()
{
    if (ImGui::TreeNode(m_label.c_str()))
    {
        for (int n = 0; n < m_selectables.size(); n++)
        {
            if (ImGui::Selectable(m_selectables[n].c_str(), m_selected == n))
            {
                m_selected = n;
            }
        }
        ImGui::TreePop();
    }

    return false;
}