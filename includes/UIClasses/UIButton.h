#pragma once

#include <string>


#include "UIBaseElement.h"
#include "UIBaseHandler.h"

class UIButton : public BaseUIElement {
public:
    UIButton(std::string&);
    virtual void render() override;
private:
    std::string m_label;
};
