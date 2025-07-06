#include "UICheckBox.h"

UICheckbox::UICheckbox(std::string& label) 
    : m_label(label)
{

};

void UICheckbox::render()
{
    bool checked = ImGui::Checkbox(m_label.c_str(), &m_checked);
    if (checked && getHandler())
    {
        getHandler()->execute();
    }
};
