#pragma once

// !!! depricated !!!


#include <queue>
#include <mutex>

#include "UIEvent.h"

//class UIEventQueue {
//public:
//    static UIEventQueue& GetInstance();
//    void Push(const UIEvent& event);
//    bool Pop(UIEvent& event);
//    bool IsEmpty();
//    void Clear();
//
//private:
//    UIEventQueue() = default;
//    std::queue<UIEvent> m_queue;
//    std::mutex m_mutex; 
//};