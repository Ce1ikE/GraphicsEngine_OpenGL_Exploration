#pragma once

#include <functional>
#include <any>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "UIEvent.h"
#include "UIEventQueue.h"
#include "UIBaseHandler.h"
#include "Logger.h"

class BaseUIElement {
public:
	virtual ~BaseUIElement() = default;
	// This method will be overridden by derived classes to draw the specific ImGui widget.
	// It should return true if the widget's value changed or it was interacted with
	// (e.g., button clicked, slider moved).
	virtual void render() = 0;

	// basically a list of any number of types, like:
	// setHandler([](int x, float y) {}, 1, 3.14f);  // Args = int, float
	// setHandler([]() {});                          // Args = (none)
	template<typename Func, typename... Args>
	void setHandler(Func&& f, Args&&... args)
	{
		// && == perfect forwarding which allows us to preserve the exact type and value category of the argument 
		// as it was passed to the function template orginally
		// it's important that we don't lose it as we don't know what the function we pass on looks like
		m_handler = std::make_unique<UIHandler<Func, Args...>>(std::forward<Func>(f), std::forward<Args>(args)...);
	}

	void runHandler()
	{
		m_handler->execute();
	};

	std::unique_ptr<BaseHandler>& getHandler()
	{
		return m_handler;
	};

private:
	std::unique_ptr<BaseHandler> m_handler = nullptr;
	//...
};