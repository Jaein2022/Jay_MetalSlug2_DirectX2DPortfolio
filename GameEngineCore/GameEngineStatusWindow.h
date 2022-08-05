#pragma once
#include "GameEngineGUI.h"

class GameEngineImageShotWindow : public GameEngineGUIWindow
{

public:
	GameEngineImageShotWindow() : renderTexture_(nullptr)
	{
	}

public:
	void RenderTextureSetting(ImTextureID _renderTexture, ImVec2 _size);

	void Initialize(class GameEngineLevel* _level);
	void OnGUI(class GameEngineLevel* _level, float _deltaTime) override;

private:
	ImTextureID renderTexture_;
	ImVec2 size_;
};

class GameEngineStatusWindow : public GameEngineGUIWindow
{
	//이 클래스의 존재 이유:

public:
	GameEngineStatusWindow();
	~GameEngineStatusWindow();

protected:
	GameEngineStatusWindow(const GameEngineStatusWindow& _other) = delete;
	GameEngineStatusWindow(GameEngineStatusWindow&& _other) noexcept = delete;

private:
	GameEngineStatusWindow& operator=(const GameEngineStatusWindow& _other) = delete;
	GameEngineStatusWindow& operator=(const GameEngineStatusWindow&& _other) = delete;

private:
	void Initialize(class GameEngineLevel* _level) override;
	void OnGUI(class GameEngineLevel* _level, float _deltaTime) override;

private:
	std::list<GameEngineImageShotWindow*> imageshotWindows_;

};



