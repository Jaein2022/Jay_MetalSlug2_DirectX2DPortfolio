#include "PreCompile.h"
#include "GameEngineStatusWindow.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineLevel.h"
#include "GameEngineCameraActor.h"
#include "GEngine.h"

void GameEngineImageShotWindow::RenderTextureSetting(ImTextureID _renderTexture, ImVec2 _size)
{
	renderTexture_ = _renderTexture;
	size_ = _size;
}

void GameEngineImageShotWindow::Initialize(GameEngineLevel* _level)
{
}

void GameEngineImageShotWindow::OnGUI(GameEngineLevel* _level, float _deltaTime)
{
	if (true == ImGui::ImageButton(renderTexture_, size_))
	{
		this->Off();
	}
}


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
	std::string currentFPS = "Current FPS : " + std::to_string(fps);
	ImGui::Text(currentFPS.c_str());


	if (true == ImGui::Button("Collision Debug Switch"))
	{
		GEngine::CollisionDebugSwitch();
	}

	if (true == ImGui::Button("FreeCamera On/Off"))
	{
		GEngine::GetCurrentLevel()->GetMainCameraActor()->FreeCameraModeOnOff();
	}


	ImGui::Text("Level Selection");
	for (std::pair<std::string, GameEngineLevel*> levelPair : GameEngineCore::allLevels_)
	{
		if (true == ImGui::Button(levelPair.first.c_str()))
		{
			GameEngineCore::ChangeLevel(levelPair.first);
		}

		ImGui::SameLine();
	}

	ImGui::NewLine();



	ImGui::Text("All RenderTargets");

	for (std::pair<std::string, GameEngineRenderTarget*> renderTargetPair : GameEngineRenderTarget::namedRes_)
	{
		if (true == ImGui::TreeNodeEx(renderTargetPair.first.c_str(), 0))
		{
			GameEngineRenderTarget* renderTarget = renderTargetPair.second;

			for (ID3D11ShaderResourceView* shaderResourceView : renderTarget->shaderResourceViews_)
			{
				float4 renderTargetScale = GameEngineWindow::GetScale() * 0.2f;

				if (true == ImGui::ImageButton(static_cast<ImTextureID>(shaderResourceView),
					{ renderTargetScale.x, renderTargetScale.y }
				))
				{
					GameEngineImageShotWindow* newImageShotWindow
						= GameEngineGUI::CreateGUIWindow<GameEngineImageShotWindow>("Image Shot", nullptr);

					newImageShotWindow->RenderTextureSetting(static_cast<ImTextureID>(shaderResourceView),
						{ GameEngineWindow::GetScale().x, GameEngineWindow::GetScale().y });
				}

			}
			ImGui::TreePop();
		}
	}
}
