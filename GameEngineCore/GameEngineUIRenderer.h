#pragma once
#include "GameEngineTextureRenderer.h"

class GameEngineUIRenderer : public GameEngineTextureRenderer
{
	//UI만 그리는 렌더러. 텍스처렌더러와 다른 점이라면 카메라 효과가 적용되는 메인카메라가 아니라, 
	// 윈도우좌표계만 적용되는 UI카메라에 등록되는것이 기본값이라는 차이밖에 없다.
public:
	GameEngineUIRenderer();
	~GameEngineUIRenderer();

protected:
	GameEngineUIRenderer(const GameEngineUIRenderer& _other) = delete;
	GameEngineUIRenderer(GameEngineUIRenderer&& _other) noexcept = delete;

private:
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer& _other) = delete;
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer&& _other) = delete;


protected:
	void Start() override;



private:

};

