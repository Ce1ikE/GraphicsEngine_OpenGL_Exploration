#include "UISelect.h"


UISelect::UISelect(std::string& label, std::vector<std::string>& options)
{
    m_label = label;
    m_options = options;
    m_selectedIndex = 0;
};

void UISelect::setSelectedIndex(int index)
{
    if (index >= 0 && index < m_options.size())
    {
        m_selectedIndex = index;
    }
};

int UISelect::getSelectedIndex()
{
    return m_selectedIndex;
};

std::string UISelect::getSelectedOption()
{
    if (m_selectedIndex >= 0 && m_selectedIndex < m_options.size()) {
        return m_options[m_selectedIndex];
    }
    return "";
};

void UISelect::render()
{
    bool valueChanged = false;

    if (ImGui::BeginCombo(m_label.c_str(), m_options.empty() ? "None" : m_options[m_selectedIndex].c_str())) 
    {
    
        for (int i = 0; i < m_options.size(); ++i) 
        {
        
            bool is_selected = (m_selectedIndex == i);
            if (ImGui::Selectable(m_options[i].c_str(), is_selected)) 
            {
                m_selectedIndex = i;
                valueChanged = true;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (valueChanged && getHandler())
    {
        runHandler();
    }

}