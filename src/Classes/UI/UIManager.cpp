#include "UIManager.h"


std::map<std::string, Scene*> UIManager::Scenes;
std::map<std::string, UIPanel*> UIManager::Panels;

void UIManager::Init(GLFWwindow* window)
{
	// https://github.com/ocornut/imgui
	// https://github.com/ocornut/imgui/wiki/Getting-Started
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
    // ImGuiIO& io = ImGui::GetIO();
	// (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// sets a theme for the UIElements
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");
};

void UIManager::Shutdown()
{
    for (auto it : Panels)
    {
        it.second->clearUIElements();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    Logger::succes(MESSAGE("ImGui cleanup complete"));
};

UIManager::~UIManager()
{
    Shutdown();
};

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

// renders the current active Scene and UI interface
void UIManager::RenderActiveScenes()
{
	for (auto scene : Scenes)
	{
		if (scene.second->isActive)
		{
			scene.second->renderScene();
		}
		else
		{
			Logger::info(
				MESSAGE("SCENE: " + scene.first + " not rendered")
			);
		}
	}
};

void UIManager::RenderEngineUI()
{
    StartFrame();
    
    // Set up a docking space if you want a flexible layout (optional)
    // ImGui::DockSpaceOverViewport();
    // ImGui::GetMainViewport();

    // Render all panels
    for (auto itr : Panels) 
    {
        itr.second->render();
    }
    EndFrame();
};

void UIManager::StartFrame()
{
	// Per-frame setup for ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
};

void UIManager::EndFrame()
{
	// Render ImGui frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

void UIManager::addScene(Scene* newScene, std::string name)
{
	Scenes[name] = newScene;
};

void UIManager::addUIPanel(UIPanel* newPanel, std::string name)
{
    Panels[name] = newPanel;
    Logger::info(
        MESSAGE("Added new panel: " + name),
        TURQUOISE
    );
};

UIPanel* UIManager::getUIPanel(std::string& name) {
    auto it = Panels.find(name);
    if (it != Panels.end()) 
    {
        return it->second;
    }
    return nullptr;
}

void UIManager::generateEngineUI()
{
    addUIPanel(new UIPanel((std::string)"Shaders"), "Shaders");
    addUIPanel(new UIPanel((std::string)"Game Objects"), "GameObjects");
    addUIPanel(new UIPanel((std::string)"Shader Info"), "ShaderInfo");
    addUIPanel(new UIPanel((std::string)"GameObject Info"), "GameObjectInfo");
    addUIPanel(new UIPanel((std::string)"Global Features"), "Features");

    populateShaderListPanel();
    populateGameObjectListPanel();
    populateFeaturesPanel();
}

void UIManager::populateShaderListPanel()
{
    UIPanel* shaderPanel = getUIPanel((std::string)"Shaders");

    if (!shaderPanel)
        return;
    
    std::vector<std::string> shaders;
    for (auto itr : ResourceManager::Shaders)
    {
        shaders.push_back(itr.first);
    }

    BaseUIElement* select = shaderPanel->addUIElement("shaders", std::make_unique<UISelect>(std::string("current Shaders"), shaders));
    
    select->setHandler([this,select]() {
        std::string selectedShader = dynamic_cast<UISelect*>(select)->getSelectedOption();
        auto it = ResourceManager::Shaders.find(selectedShader);
        if (it != ResourceManager::Shaders.end()) {
            Shader* shader = &it->second;
            this->populateShaderInfoPanel(shader, selectedShader);
        }
    });
};

void UIManager::populateGameObjectListPanel()
{
    UIPanel* gameObjectsPanel = getUIPanel((std::string)"GameObjects");

    if (!gameObjectsPanel)
        return;
    
    std::vector<std::string> gameObjects;
    for (auto itr : *Scenes[STD_SCENE]->getGameObjects())
    {
        gameObjects.push_back(itr.first);
    }
    BaseUIElement* select = gameObjectsPanel->addUIElement("gameObjects", std::make_unique<UISelect>(std::string("current gameObjects"), gameObjects));
    select->setHandler([this,select]() {
        std::string selectedName = dynamic_cast<UISelect*>(select)->getSelectedOption();

        auto& gameObjects = *Scenes[STD_SCENE]->getGameObjects();

        auto it = gameObjects.find(selectedName);
        if (it != gameObjects.end()) {
            GameObject* selectedObject = it->second;
            populateGameObjectInfoPanel(selectedObject,selectedName);
        }
    });
};

void UIManager::populateFeaturesPanel()
{
    UIPanel* featuresPanel = getUIPanel((std::string)"Features");

    if (!featuresPanel)
        return;
    BaseUIElement* select = featuresPanel->addUIElement("ViewMode", std::make_unique<UISelect>(std::string("View mode"), std::vector<std::string>({ "line","fill" })));
    select->setHandler([this,select]() {
        switch (dynamic_cast<UISelect*>(select)->getSelectedIndex())
        {
        case 0:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        }
    });
};

void UIManager::populateGameObjectInfoPanel(GameObject* gameObject,std::string gameObjectName)
{
    UIPanel* panel = getUIPanel((std::string)"GameObjectInfo");
    if (!panel) 
        return;

    panel->clearUIElements(); // Clear previous object's UI

    if (!gameObject) {
        panel->addUIElement("NoObjectSelectedLabel", std::make_unique<UILabel>((std::string)"No Game Object Selected."));
        return;
    }

    panel->addUIElement("ObjectNameLabel", std::make_unique<UILabel>((std::string)""));

    // Object Position Sliders
    BaseUIElement * slider = panel->addUIElement("PositionX", std::make_unique<UISliderFloat>((std::string)"Position X", gameObject->getPosition().x, -10.0f, 10.0f));
    slider->setHandler([gameObject,slider]() {
        float x = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentPosition = gameObject->getPosition();
        currentPosition.x = x;
        gameObject->setPosition(currentPosition);
    });

    slider = panel->addUIElement("PositionY", std::make_unique<UISliderFloat>((std::string)"Position Y", gameObject->getPosition().y, -10.0f, 10.0f));
    slider->setHandler([gameObject, slider]() {
        float y = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentPosition = gameObject->getPosition();
        currentPosition.y = y;
        gameObject->setPosition(currentPosition);
    });
    slider = panel->addUIElement("PositionZ", std::make_unique<UISliderFloat>((std::string)"Position Z", gameObject->getPosition().z, -10.0f, 10.0f));
    slider->setHandler([gameObject, slider]() {
        float z = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentPosition = gameObject->getPosition();
        currentPosition.z = z;
        gameObject->setPosition(currentPosition);
    });
    // Object Scale Sliders
    slider = panel->addUIElement("ScaleX", std::make_unique<UISliderFloat>((std::string)"Scale X", gameObject->getScale().x, 0.1f, 10.0f));
    slider->setHandler([gameObject, slider]() {
        float x = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentScale = gameObject->getScale();
        currentScale.x = x;
        gameObject->setScale(currentScale);
    });
    slider = panel->addUIElement("ScaleY", std::make_unique<UISliderFloat>((std::string)"Scale Y", gameObject->getScale().y, 0.1f, 10.0f));
    slider->setHandler([gameObject, slider]() {
        float y = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentScale = gameObject->getScale();
        currentScale.y = y;
        gameObject->setScale(currentScale);
    });
    slider = panel->addUIElement("ScaleZ", std::make_unique<UISliderFloat>((std::string)"Scale Z", gameObject->getScale().z, 0.1f, 10.0f));
    slider->setHandler([gameObject, slider]() {
        float z = dynamic_cast<UISliderFloat*>(slider)->getValue();
        glm::vec3 currentScale = gameObject->getScale();
        currentScale.z = z;
        gameObject->setScale(currentScale);
    });
};

void UIManager::populateShaderInfoPanel(Shader* shader,std::string shaderName)
{
    UIPanel* shaderInfoPanel = getUIPanel((std::string)"ShaderInfo");
    if (!shaderInfoPanel)
        return;
    // clear existing UI elements for the old shader
    // before adding new buttons
    shaderInfoPanel->clearUIElements();
    if (!shader || shader->ID == 0) {
        return;
    }

    // default button to recompile shaders
    BaseUIElement * button = shaderInfoPanel->addUIElement(std::string("RecompileShaderButton"), std::make_unique<UIButton>(std::string("Recompile Current Shader")));
    button->setHandler([=]() {
        ResourceManager::LoadShader(
            shader->getVertexSource(),
            shader->getFragmentSource(),
            shader->getGeometrySource(),
            shaderName
        );
    });

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
        // can't think of way we want to change it
        if (uniformName.rfind("gl_", 0) == 0 ||uniformName == "model" || uniformName == "view" || uniformName == "projection" || uniformName.find("[") != std::string::npos) 
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
            shaderInfoPanel->addUIElement(uniformName, std::make_unique<UISliderFloat>(uniformName, 0.0f, -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC2:
            // TODO
            //addUIElement(uniformName, std::make_unique<UISliderVec2>(uniformName, glm::vec2(0.0f), -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC3:
            shaderInfoPanel->addUIElement(uniformName, std::make_unique<UISliderVec3>(uniformName, glm::vec3(0.0f), -10.0f, 10.0f));
            break;
        case GL_FLOAT_VEC4:
            // TODO
            //addUIElement(uniformName, std::make_unique<UISliderVec4>(uniformName, glm::vec4(0.0f), -10.0f, 10.0f));
            break;
            // TODO
        case GL_INT:
        case GL_SAMPLER_2D: // Samplers usually map to texture slots or file paths
            // Add specific UI for integers or textures if needed
            //addUIElement(uniformName, std::make_unique<UILabel>(uniformName + " (Sampler/Int)"));
            break;
        case GL_BOOL:
            // TODO
            //addUIElement(uniformName, std::make_unique<UICheckbox>(uniformName, false)); // Assuming a UICheckbox class
            break;
        default:
            // TODO
            //addUIElement(uniformName, std::make_unique<UILabel>(uniformName + " (unsupported type)"));
            break;
        }
    }
}

void UIManager::update(float dt)
{

  
};
