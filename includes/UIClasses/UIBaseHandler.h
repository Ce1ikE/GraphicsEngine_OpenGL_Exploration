#pragma once

#include <functional>
#include <any>
#include <utility>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "UIEvent.h"
#include "UIEventQueue.h"

// while the BaseHandler might redundant someone might come along and expand a new UIHandler for other purposes
class BaseHandler {
public:
    virtual void execute() = 0;
    virtual ~BaseHandler() = default;
private:
    // ...
};

// https://www.geeksforgeeks.org/cpp/variadic-function-templates-c/
// other ways : https://dev.to/pauljlucas/variadic-functions-in-c-2alg
// the C way : https://en.cppreference.com/w/cpp/utility/variadic.html
template<typename Func, typename... Args>
class UIHandler : public BaseHandler {
public:
    UIHandler(Func&& func, Args&&... args)
        : m_func(std::forward<Func>(func)), m_args(std::forward<Args>(args)...)
    {
    };

    virtual void execute() override
    {
        std::cout << "\nexecute Handler\n";
        std::apply(m_func, m_args);
    };

private:
    Func m_func;
    std::tuple<Args...> m_args;
};