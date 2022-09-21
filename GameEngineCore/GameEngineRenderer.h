#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"

struct RenderOption
{
    float deltaTime_;
    int isMasked_;      
    int option01_;    
    int option02_;
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
    RenderOption renderOption_;

private:
    CameraOrder cameraOrder_;
    int renderingOrder_;
};