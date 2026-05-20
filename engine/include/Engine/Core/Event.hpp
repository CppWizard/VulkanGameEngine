#pragma once

#include <string>
#include <sstream>
#include <cstdint>

namespace Engine
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    class Event
    {
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual std::string ToString() const = 0;
    };

    class WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : Width(width), Height(height) {}

        EventType GetEventType() const override { return EventType::WindowResize; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << Width << ", " << Height;
            return ss.str();
        }

        uint32_t Width;
        uint32_t Height;
    };

    class WindowCloseEvent final : public Event
    {
    public:
        EventType GetEventType() const override { return EventType::WindowClose; }
        std::string ToString() const override { return "WindowCloseEvent"; }
    };

    class KeyPressedEvent final : public Event
    {
    public:
        KeyPressedEvent(int key, bool repeat)
            : Key(key), Repeat(repeat) {}

        EventType GetEventType() const override { return EventType::KeyPressed; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << Key << " Repeat=" << Repeat;
            return ss.str();
        }

        int Key;
        bool Repeat;
    };

    class KeyReleasedEvent final : public Event
    {
    public:
        explicit KeyReleasedEvent(int key)
            : Key(key) {}

        EventType GetEventType() const override { return EventType::KeyReleased; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << Key;
            return ss.str();
        }

        int Key;
    };

    class MouseButtonPressedEvent final : public Event
    {
    public:
        explicit MouseButtonPressedEvent(int button)
            : Button(button) {}

        EventType GetEventType() const override { return EventType::MouseButtonPressed; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << Button;
            return ss.str();
        }

        int Button;
    };

    class MouseButtonReleasedEvent final : public Event
    {
    public:
        explicit MouseButtonReleasedEvent(int button)
            : Button(button) {}

        EventType GetEventType() const override { return EventType::MouseButtonReleased; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << Button;
            return ss.str();
        }

        int Button;
    };

    class MouseMovedEvent final : public Event
    {
    public:
        MouseMovedEvent(double x, double y)
            : X(x), Y(y) {}

        EventType GetEventType() const override { return EventType::MouseMoved; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << X << ", " << Y;
            return ss.str();
        }

        double X;
        double Y;
    };

    class MouseScrolledEvent final : public Event
    {
    public:
        MouseScrolledEvent(double xOffset, double yOffset)
            : XOffset(xOffset), YOffset(yOffset) {}

        EventType GetEventType() const override { return EventType::MouseScrolled; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << XOffset << ", " << YOffset;
            return ss.str();
        }

        double XOffset;
        double YOffset;
    };
}