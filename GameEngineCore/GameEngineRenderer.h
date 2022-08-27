#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"

struct RenderOption
{
    float deltaTime_;
    int isMask_;
    int isJump_;
    int option00_;
    int option01_;
    int option02_;
    int option03_;
    int option04_;
    int option05_;
    int option06_;
    int option07_;
    int option08_;
    int option09_;
    int option10_;
    int option11_;
    int option12_;
    int option13_;
    int option14_;
    int option15_;
    int option16_;
    int option17_;
    int option18_;
    int option19_;
    int option20_;
};

class GameEngineRenderingPipeLine;
class GameEngineRenderer : public GameEngineTransformComponent
{
    //모든 렌더러가 공통적으로 가져야 하는 기능만을 최소한도로 가진 컴포넌트 클래스.
    //실질적인 역할은 트랜스폼 정보를 가지고 게임엔진카메라에 등록되어 렌더링 대상이 되는 것이다.

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
    GameEngineRenderingPipeLine* GetClonePipeLine(GameEngineRenderingPipeLine* _renderingPipeLine);
    void SetRenderingOrder(int _renderingOrder);

public:
    inline int GetRenderingOrder()
    {
        return renderingOrder_;
    }

protected:
	virtual void Start();
	virtual void Update(float _deltaTime) {}
	virtual void Render(float _deltaTime) = 0;
	virtual void End() {}

protected:
	void PushRendererToMainCamera();	//렌더러가 메인카메라에 등록하는 함수.
	void PushRendererToUICamera();		//렌더러가 UI카메라에 등록하는 함수.

protected:
    class GameEngineCamera* camera_;
    CameraOrder cameraOrder_;
    RenderOption renderOption_;
    int renderingOrder_;
};