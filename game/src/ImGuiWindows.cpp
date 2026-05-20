#include <imgui.h>
#include "Input.h" // adjust include path as needed

void DrawEditorUI()
{
	ImGui::Begin("Scene Hierarchy");
	ImGui::Text("Scene");
	ImGui::BulletText("Quad");
	ImGui::End();

	// Static position storage for the inspector
	static float position[3] = { 0.0f, 0.0f, 0.0f };

	ImGui::Begin("Inspector");
	ImGui::Text("Selected: Quad");
	ImGui::DragFloat3("Position", position);
	ImGui::End();

	ImGui::Begin("Input Debug");
	ImGui::Text("Mouse Position: %.1f, %.1f",
		Input::GetMouseX(),
		Input::GetMouseY()
	);
	ImGui::Text("W: %s", Input::IsKeyDown(GLFW_KEY_W) ? "Down" : "Up");
	ImGui::End();
}
