#pragma once

#include "UIBaseElement.h"
#include <string>

class UILabel : public BaseUIElement {
public:
    UILabel(std::string&);
    virtual void render() override;
private:
    std::string m_label;
};
