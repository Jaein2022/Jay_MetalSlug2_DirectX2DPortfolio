#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderingPipeLine;
class GameEngineDefaultRenderer : public GameEngineRenderer
{
	//���������� �ݵ�� ������ ������������ ��� �ִ�. ������ ������������ ���̷�ƮX�� ���������� ����Ǿ� �ִ�.
	//�ϳ��� ������ ������������ �ϳ��� �޽�, �ϳ��� ���׸���, �ϳ��� ����Ÿ���� ������.


public:
	GameEngineDefaultRenderer();
	~GameEngineDefaultRenderer();

protected:
	GameEngineDefaultRenderer(const GameEngineDefaultRenderer& _other) = delete;
	GameEngineDefaultRenderer(GameEngineDefaultRenderer&& _other) noexcept = delete;

private:
	GameEngineDefaultRenderer& operator=(const GameEngineDefaultRenderer& _other) = delete;
	GameEngineDefaultRenderer& operator=(const GameEngineDefaultRenderer&& _other) = delete;


public:
	virtual void Render(float _deltaTime) override;
	void SetPipeLine(const std::string& _name);
	GameEngineRenderingPipeLine* GetPipeLine();


protected:
	virtual void Start();
	virtual void Update(float _deltaTime);
	virtual void End();

protected:	//�����̺��̸� �ڽ� ���������� ���̴����ҽ��� ������ ���� ����.
	GameEngineShaderResourceHelper shaderResources_;
	//������ ����: ���������� ������ ���̴����ҽ����۸� ������ �ϱ� ���ؼ�.

private:
	GameEngineRenderingPipeLine* renderingPipeLine_;
	//�����ؾ��ϴ� �޸𸮸� ������ �ִ�??
};

