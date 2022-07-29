#pragma once
#include "GameEngineGUI.h"

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


public:

private:
	void Initialize(class GameEngineLevel* _level) override;
	void OnGUI(class GameEngineLevel* _level, float _deltaTime) override;

};



