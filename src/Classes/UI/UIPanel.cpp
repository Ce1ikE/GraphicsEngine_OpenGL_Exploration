#include "UIPanel.h"


UIPanel::UIPanel(std::string label)
{
    m_label = label;
};

void UIPanel::render() 
{
    ImGui::Begin(m_label.c_str());
    for (auto const& [name, element] : m_UIElements) 
    {
        element->render();
    }
    ImGui::End();
};


BaseUIElement*  UIPanel::addUIElement(std::string name, std::unique_ptr<BaseUIElement> element)
{
    auto* ptr = element.get();
    m_UIElements[name] = std::move(element);
    Logger::info(
        MESSAGE(m_label + ": Added new element \"" + name +"\""),
        TURQUOISE
    );
    return ptr;
};

void UIPanel::clearUIElements()
{
    Logger::info(
        MESSAGE(m_label + ": Clearing elements"),
        TURQUOISE
    );
    m_UIElements.clear(); // unique_ptr automatically call destructors
};

UIButton* UIPanel::getButton(std::string& name)
{
    auto it = m_UIElements.find(name);
    if (it != m_UIElements.end()) {
        return dynamic_cast<UIButton*>(it->second.get());
    }
    return nullptr;
}
UISliderFloat* UIPanel::getSliderFloat(std::string& name)
{
    auto it = m_UIElements.find(name);
    if (it != m_UIElements.end()) {
        return dynamic_cast<UISliderFloat*>(it->second.get());
    }
    return nullptr;
};
UISliderVec3* UIPanel::getSliderVec3(std::string& name)
{
    auto it = m_UIElements.find(name);
    if (it != m_UIElements.end()) {
        return dynamic_cast<UISliderVec3*>(it->second.get());
    }
    return nullptr;
};
UISelect* UIPanel::getSelect(std::string& name)
{
    auto it = m_UIElements.find(name);
    if (it != m_UIElements.end()) {
        return dynamic_cast<UISelect*>(it->second.get());
    }
    return nullptr;
};
UILabel* UIPanel::getLabel(std::string& name)
{
    auto it = m_UIElements.find(name);
    if (it != m_UIElements.end()) {
        return dynamic_cast<UILabel*>(it->second.get());
    }
    return nullptr;
};
