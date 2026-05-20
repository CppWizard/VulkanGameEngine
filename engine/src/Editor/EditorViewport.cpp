#include <Engine/Editor/EditorViewport.hpp>

#include <imgui.h>

namespace Engine
{
    void EditorViewport::Draw()
    {
        ImGui::Begin("Viewport");

        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();

        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        m_Size = { availableSize.x, availableSize.y };

        ImGui::Text("Scene Viewport");
        ImGui::Text("Size: %.0f x %.0f", m_Size.x, m_Size.y);
        ImGui::Text("Focused: %s", m_Focused ? "true" : "false");
        ImGui::Text("Hovered: %s", m_Hovered ? "true" : "false");

        ImGui::Dummy(availableSize);

        ImGui::End();
    }
}