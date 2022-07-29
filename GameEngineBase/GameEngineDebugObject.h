#pragma once

class GameEngineDebugObject
{
	//이 클래스의 존재 이유:
public:

	GameEngineDebugObject();
	~GameEngineDebugObject();

	GameEngineDebugObject(const GameEngineDebugObject& _other) = delete;
	GameEngineDebugObject(GameEngineDebugObject&& _other) noexcept = delete;
	GameEngineDebugObject& operator=(const GameEngineDebugObject& _other) = delete;
	GameEngineDebugObject& operator=(GameEngineDebugObject&& _other) = delete;

public:
	void DebugOn()
	{
		isDebuging_ = true;
	}

	void DebugOff()
	{
		isDebuging_ = false;
	}

	bool IsDebuging()
	{
		return isDebuging_;
	}


private:
	bool isDebuging_;

};

