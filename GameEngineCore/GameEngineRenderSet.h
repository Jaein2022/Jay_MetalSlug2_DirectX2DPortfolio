#pragma once
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderingPipeLine;
class GameEngineRenderSet
{
	//�������� �ʿ��� �ּ����� ���� ����� ������ ���������ΰ� ���̴����ҽ����۸� ���� ��Ƴ��� Ŭ����.

	friend class GameEngineRenderTarget;

public:
	GameEngineRenderSet();
	~GameEngineRenderSet();

protected:
	GameEngineRenderSet(const GameEngineRenderSet& _other) = delete;
	GameEngineRenderSet(GameEngineRenderSet&& _other) noexcept = delete;

private:
	GameEngineRenderSet& operator=(const GameEngineRenderSet& _other) = delete;
	GameEngineRenderSet& operator=(const GameEngineRenderSet&& _other) = delete;


public:
	void SetPipeLine(const std::string& _renderingPipeLineName);
	GameEngineRenderingPipeLine* GetPipeLine();

	void Render();

public:
	GameEngineShaderResourceHelper shaderResourceHelper_;


private:
	GameEngineRenderingPipeLine* renderingPipeLine_;
};
