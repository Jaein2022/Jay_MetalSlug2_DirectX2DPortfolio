#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"

class GameEngineRenderer : public GameEngineTransformComponent
{
	friend class GameEngineCamera;
	friend GameEngineLevel;

public:

	GameEngineRenderer();
	~GameEngineRenderer();

	GameEngineRenderer(const GameEngineRenderer& _other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _other) = delete;

public:
	void ChangeCamera(CameraOrder _order);


protected:
	virtual void Start();
	virtual void Update(float _deltaTime) {}
	virtual void Render(float _deltaTime) = 0;
	virtual void End() {}

protected:
	void PushRendererToMainCamera();	//렌더러가 메인카메라에 등록하는 함수.
	void PushRendererToUICamera();		//렌더러가 UI카메라에 등록하는 함수.

protected:
	CameraOrder cameraOrder_;

};