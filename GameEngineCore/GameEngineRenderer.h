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
    //��� �������� ���������� ������ �ϴ� ��ɸ��� �ּ��ѵ��� ���� ������Ʈ Ŭ����.
    //�������� ������ Ʈ������ ������ ������ ���ӿ���ī�޶� ��ϵǾ� ������ ����� �Ǵ� ���̴�.

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
	void PushRendererToMainCamera();	//�������� ����ī�޶� ����ϴ� �Լ�.
	void PushRendererToUICamera();		//�������� UIī�޶� ����ϴ� �Լ�.

protected:
    class GameEngineCamera* camera_;
    CameraOrder cameraOrder_;
    RenderOption renderOption_;
    int renderingOrder_;
};