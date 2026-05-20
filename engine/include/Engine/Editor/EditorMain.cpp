#include <Engine/Engine.hpp>

#include <GLFW/glfw3.h>

class EditorApp final : public Engine::Application
{
public:
    EditorApp()
        : Engine::Application({
            .Title = "Vulkan Editor",
            .Width = 1600,
            .Height = 900,
            .Resizable = true
            })
    {}

protected:
    void OnStart() override
    {
        ENGINE_INFO("Editor started.");
    }

    void OnUpdate(float) override
    {
        if (Engine::Input::IsKeyPressed(GLFW_KEY_ESCAPE))
        {
            Close();
        }
    }

    void OnRender() override
    {
        // Editor + Vulkan rendering will go here later.
    }

    void OnShutdown() override
    {
        ENGINE_INFO("Editor shutdown.");
    }
};

int main()
{
    EditorApp editor;
    editor.Run();

    return 0;
}