#include <Engine/Engine.hpp>

class EditorApp final : public Engine::Application
{
protected:
	void OnStart() override {}
	void OnUpdate(float deltaTime) override {}
	void OnShutdown() override {}
};

int main()
{
	EditorApp editor;
	editor.Run();
	return 0;
}