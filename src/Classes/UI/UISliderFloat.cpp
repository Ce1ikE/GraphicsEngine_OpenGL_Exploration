#include "UISliderFloat.h"

UISliderFloat::UISliderFloat(std::string& label, float initialValue, float min, float max)
    : m_label(label), m_value(initialValue), m_min(min), m_max(max)
{

}

void UISliderFloat::render()
{
    bool valueChanged = ImGui::SliderFloat(m_label.c_str(), &m_value, m_min, m_max);
    if (valueChanged && getHandler())
    {
        runHandler();
    }
}

float UISliderFloat::getValue()
{
    return m_value;
}

void UISliderFloat::setValue(float val)
{
    m_value = val;
}