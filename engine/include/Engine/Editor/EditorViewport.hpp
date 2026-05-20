#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class EditorViewport
    {
    public:
        void Draw();

        bool IsFocused() const { return m_Focused; }
        bool IsHovered() const { return m_Hovered; }

        glm::vec2 GetSize() const { return m_Size; }

    private:
        bool m_Focused = false;
        bool m_Hovered = false;
        glm::vec2 m_Size = { 0.0f, 0.0f };
    };
}