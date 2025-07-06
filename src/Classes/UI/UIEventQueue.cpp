#include "UIEventQueue.h"

// !!! depricated !!!

//UIEventQueue& UIEventQueue::GetInstance() {
//    static UIEventQueue instance;
//    return instance;
//}
//
//void UIEventQueue::Push(const UIEvent& event) {
//    std::lock_guard<std::mutex> lock(m_mutex);
//    m_queue.push(event);
//}
//
//bool UIEventQueue::Pop(UIEvent& event) {
//    std::lock_guard<std::mutex> lock(m_mutex);
//    if (m_queue.empty()) {
//        return false;
//    }
//    event = m_queue.front();
//    m_queue.pop();
//    return true;
//}
//
//bool UIEventQueue::IsEmpty() {
//    std::lock_guard<std::mutex> lock(m_mutex);
//    return m_queue.empty();
//}
//
//void UIEventQueue::Clear() {
//    std::lock_guard<std::mutex> lock(m_mutex);
//    while (!m_queue.empty())
//    {
//        m_queue.pop();
//    }
//}