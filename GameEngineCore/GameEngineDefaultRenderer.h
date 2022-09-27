#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderingPipeLine;
class GameEngineDefaultRenderer : public GameEngineRenderer
{
	//�� �����ӿ�ũ���� �������� �ʿ��� �ּ����� ���� ����� ������ ���������ΰ� ���̴����ҽ����۸� ������ 
	// �ڱⰡ ���� ������ ���ӿ���ī�޶��� ������ ü�迡�� �亯ȯ, ������ȯ, ����Ʈ��ȯ�� ���� ������ ���
	// ������ ������������ ���� ������ ���� ���� �ȼ��� �������� �ٲ� ����Ÿ�ٿ� �����ϴ� ������Ʈ.
	//�������� �������� �����ϴ� ������Ʈ�̴�.

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
	GameEngineRenderingPipeLine* GetClonePipeLine();


protected:
	virtual void Start();
	virtual void Update(float _deltaTime);
	virtual void End();

protected:	//�����̺��̸� �ڽ� ���������� ���̴����ҽ��� ������ ���� ����.
	GameEngineShaderResourceHelper shaderResourceHelper_;
	//������ ����: ���������� ������ ���̴����ҽ����۸� ������ �ϱ� ���ؼ�.

private:
	GameEngineRenderingPipeLine* renderingPipeLine_;
	//�����ؾ��ϴ� �޸𸮸� ������ �ִ�??
};

