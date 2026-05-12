#include <Engine/Engine.hpp>

class GameApp final : public Engine::Application
{
protected:
	void Onstart() override {}
	void OnUpdate(float deltaTime) override {}
	void OnShutdown() override {}
};

int main()
{
	GameApp editor;
	editor.Run();
	return 0;
}