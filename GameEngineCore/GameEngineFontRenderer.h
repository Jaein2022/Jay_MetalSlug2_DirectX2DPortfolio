#pragma once
#include "GameEngineDefaultRenderer.h"

class GameEngineRenderTarget;
class GameEngineFont;
class GameEngineFontRenderer : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유:

	static GameEngineRenderTarget* fontTarget_;

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

	void SetScreenPosition(const float4& _pos)
	{
		screenPosition_ = _pos;
	}

	void SetColor(const float4& _color)
	{
		fontColor_ = _color;
	}

	void SetSize(float _size)
	{
		fontSize_ = _size;
	}

protected:
	virtual void Start() override;
	virtual void Render(float _deltaTime) override;

private:
	std::string text_;
	GameEngineFont* font_;
	float fontSize_;
	float4 fontColor_;
	float4 screenPosition_;
};