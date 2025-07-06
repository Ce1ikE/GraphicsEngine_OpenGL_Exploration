#include "UILabel.h"

UILabel::UILabel(std::string& label)
{
	m_label = label;
};

void UILabel::render()
{
	ImGui::LabelText(m_label.c_str(), m_label.c_str());
};
