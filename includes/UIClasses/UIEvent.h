#pragma once

// !!! depricated !!!
#include <string>
#include <queue>
#include <mutex> // For thread safety if multi-threading
#include <functional>

#include <glm/glm.hpp>
#include "UIBaseElement.h"

//enum UIEventType {
//    None = 0, // Indicates no event occurred
//    // UI-generated events
//    UIButtonClicked,
//    UISliderFloatChanged,
//    UISliderVec2Changed,
//    UISliderVec3Changed,
//    UISliderVec4Changed,
//    UICheckboxChanged,
//    UISelectChanged,
//    // Engine-level events triggered by UI interactions (more specific)
//    ShaderRecompileRequested,
//    ShaderSelected,
//    ShaderUniformFloatChanged, // Specific for float uniform updates
//    ShaderUniformVec3Changed,  // Specific for vec3 uniform updates
//    GameObjectSelected,
//    GameObjectPositionChanged,
//    GameObjectScaleChanged,
//    GameObjectRotationChanged,
//    ViewModeChanged,
//    // ... more game-specific events (e.g., PlayerMoved, EnemyDied)
//
//};
//
//struct UIEvent {
//    UIEventType type = UIEventType::None;
//    BaseUIElement* source;
//    // data;
//};
