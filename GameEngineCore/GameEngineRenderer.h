#pragma once
#include "GameEngineTransformComponent.h"

class GameEngineLevel;
class GameEngineRenderer : public GameEngineTransformComponent
{
	friend GameEngineLevel;
	friend class GameEngineCamera;

public:

	GameEngineRenderer();
	~GameEngineRenderer();

	GameEngineRenderer(const GameEngineRenderer& _other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _other) = delete;

public:


protected:
	virtual void Start();
	virtual void Render(float _deltaTime) = 0;


protected:
	void PushRendererToMainCamera();	//�������� ����ī�޶� ����ϴ� �Լ�.
	void PushRendererToUICamera();		//�������� UIī�޶� ����ϴ� �Լ�.

};