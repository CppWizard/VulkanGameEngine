#pragma once

#include <Engine/Core/Event.hpp>

#include <functional>

namespace Engine
{
    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event)
            : m_Event(event) {}

        template<typename T>
        bool Dispatch(const std::function<bool(T&)>& function)
        {
            T* event = dynamic_cast<T*>(&m_Event);

            if (event)
            {
                m_Event.Handled = function(*event);
                return true;
            }

            return false;
        }

    private:
        Event& m_Event;
    };
}