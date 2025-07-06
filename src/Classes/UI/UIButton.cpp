#include "UIButton.h"

UIButton::UIButton(std::string& label)
	: m_label(label)
{

}

void UIButton::render()
{
	bool clicked = ImGui::Button(m_label.c_str());

	if (clicked && getHandler())
	{
		runHandler();
	}
}

