#pragma once
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderingPipeLine;
class GameEngineRenderSet
{
	//렌더링에 필요한 최소한의 구성 요소인 렌더링 파이프라인과 셰이더리소스헬퍼만 따로 모아놓은 클래스.

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
