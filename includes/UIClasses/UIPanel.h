#pragma once

#include <string>
#include <map>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Logger.h"
#include "UIBaseElement.h"
#include "UIButton.h"
#include "UISliderFloat.h"
#include "UISliderVec3.h"
#include "UISelect.h"
#include "UILabel.h"

class UIPanel : public BaseUIElement {
public:
    UIPanel(std::string);

    virtual void render() override;

    BaseUIElement* addUIElement(std::string, std::unique_ptr<BaseUIElement>);
    void clearUIElements();

    UIButton* getButton(std::string&);
    UISliderFloat* getSliderFloat(std::string&);
    UISliderVec3* getSliderVec3(std::string&);
    UISelect* getSelect(std::string&);
    UILabel* getLabel(std::string&);

private:
    std::map<std::string, std::unique_ptr<BaseUIElement>> m_UIElements;
    std::string m_label;
    bool m_anyChildChanged = false;
};

