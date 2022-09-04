#pragma once
#include "GameEngineDefaultRenderer.h"

enum class LeftAndRightSort
{
	Left = 0x0,
	Center = 0x1,
	Right = 0x2,
};

enum class TopAndBotSort
{
	Top = 0x0,
	VCenter = 0x4,
	Bot = 0x8
};

class GameEngineRenderTarget;
class GameEngineFont;
class GameEngineFontRenderer : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유:

public:
	GameEngineFontRenderer();
	~GameEngineFontRenderer();

protected:
	GameEngineFontRenderer(const GameEngineFontRenderer& _other) = delete;
	GameEngineFontRenderer(GameEngineFontRenderer&& _other) noexcept = delete;

private:
	GameEngineFontRenderer& operator=(const GameEngineFontRenderer& _other) = delete;
	GameEngineFontRenderer& operator=(const GameEngineFontRenderer&& _other) = delete;


public:
	void SetText(const std::string& _text, const std::string& _font = "돋움");

public:
	const std::string& GetText()
	{
		return text_;
	}

	void SetScreenPosition(const float4& _position)
	{
		screenPosition_ = _position;
	}

	float4 GetScreenPostion()
	{
		return screenPosition_;
	}

	void SetColor(const float4& _color)
	{
		fontColor_ = _color;
	}

	void SetSize(float _size)
	{
		fontSize_ = _size;
	}

	void SetLeftAndRightSort(LeftAndRightSort _value)
	{
		lr_ = _value;
	}

	void SetTopAndBotSort(TopAndBotSort _value)
	{
		tb_ = _value;
	}

protected:
	virtual void Start() override;
	virtual void Render(float _deltaTime) override;

private:
	static GameEngineRenderTarget* fontTarget_;

	std::string text_;
	GameEngineFont* font_;
	float fontSize_;
	float4 fontColor_;
	float4 screenPosition_;

	LeftAndRightSort lr_;
	TopAndBotSort tb_;
};