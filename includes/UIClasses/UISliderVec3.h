#pragma once

#include "UIBaseElement.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Derived class for a Vec3 Slider (e.g., for a 'vec3' uniform)
class UISliderVec3 : public BaseUIElement {
public:
    UISliderVec3(std::string&, glm::vec3&, float, float);
    virtual void render() override;

    glm::vec3 getValue();
    void setValue(glm::vec3&);

private:
    std::string m_label;
    glm::vec3 m_value;
    float m_min;
    float m_max;
};