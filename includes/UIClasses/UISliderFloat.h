#pragma once

#include "UIBaseElement.h"
#include <string>

// Derived class for a Float Slider (e.g., for a 'float' uniform)
class UISliderFloat : public BaseUIElement {
public:
    UISliderFloat(std::string&, float, float, float);
    virtual void render() override;

    float getValue();
    void setValue(float);

private:
    std::string m_label;
    float m_value; // The actual value the slider controls
    float m_min;
    float m_max;
};