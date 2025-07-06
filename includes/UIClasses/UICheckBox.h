#pragma once

#include "UIBaseElement.h"
#include <string>

class UICheckbox : public BaseUIElement {
public:
    UICheckbox(std::string&);
    virtual void render() override;

private:
    std::string m_label;
    bool m_checked;
};
