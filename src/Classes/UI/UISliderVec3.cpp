#include "UISliderVec3.h"


UISliderVec3::UISliderVec3(std::string& label, glm::vec3& initialValue, float min, float max)
    : m_label(label), m_value(initialValue), m_min(min), m_max(max)
{

}

void UISliderVec3::render()
{
    bool valueChanged = ImGui::SliderFloat3(m_label.c_str(), glm::value_ptr(m_value), m_min, m_max);
    if (valueChanged && getHandler())
    {
        runHandler();
    }
}

glm::vec3 UISliderVec3::getValue()
{
    return m_value;
}

void UISliderVec3::setValue(glm::vec3& val)
{
    m_value = val;
}
