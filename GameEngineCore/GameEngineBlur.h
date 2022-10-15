#pragma once
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderTarget;
class GameEngineBlur
{
	//�� Ŭ������ ���� ����:
public:
	GameEngineBlur();
	~GameEngineBlur();

protected:
	GameEngineBlur(const GameEngineBlur& _other) = delete;
	GameEngineBlur(GameEngineBlur&& _other) noexcept = delete;

private:
	GameEngineBlur& operator=(const GameEngineBlur& _other) = delete;
	GameEngineBlur& operator=(const GameEngineBlur&& _other) = delete;


public:
	virtual void EffectInit();
	virtual void Effect(GameEngineRenderTarget* _renderTarget);


private:
	GameEngineRenderTarget* copiedRenderTarget_;

	GameEngineShaderResourceHelper shaderResourceHelper_;

	GameEngineRenderingPipeLine* renderingPipeLine_;

};
