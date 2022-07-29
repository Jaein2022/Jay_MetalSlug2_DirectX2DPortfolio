#include "PreCompile.h"
#include "GameEngineStatusWindow.h"

GameEngineStatusWindow::GameEngineStatusWindow()
{
}

GameEngineStatusWindow::~GameEngineStatusWindow()
{
}

void GameEngineStatusWindow::Initialize(GameEngineLevel* _level)
{
}

void GameEngineStatusWindow::OnGUI(GameEngineLevel* _level, float _deltaTime)
{
	int fps = static_cast<int>(1.f / _deltaTime);

	//std::string fpsText = "FPS: " + std::to_string(fps);
	ImGui::Text(std::to_string(fps).c_str());
}
