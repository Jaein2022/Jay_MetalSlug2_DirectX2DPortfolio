#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineShaderResourceHelper.h"

class GameEngineRenderingPipeLine;
class GameEngineDefaultRenderer : public GameEngineRenderer
{
	//이 프레임워크에서 렌더링에 필요한 최소한의 구성 요소인 렌더링 파이프라인과 셰이더리소스헬퍼를 가지고 
	// 자기가 가진 정보를 게임엔진카메라의 렌더링 체계에서 뷰변환, 투영변환, 뷰포트변환을 거쳐 변형된 대로
	// 렌더링 파이프라인을 통해 윈도우 범위 내의 픽셀별 색상값으로 바꿔 렌더타겟에 전달하는 컴포넌트.
	//실질적인 렌더링을 수행하는 컴포넌트이다.

	//렌더러들은 반드시 렌더링 파이프라인을 들고 있다. 렌더링 파이프라인은 다이렉트X와 직접적으로 연결되어 있다.
	//하나의 렌더링 파이프라인은 하나의 메쉬, 하나의 메테리얼, 하나의 렌더타겟을 가진다.


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

protected:	//프라이빗이면 자식 렌더러들이 셰이더리소스에 접근할 수가 없다.
	GameEngineShaderResourceHelper shaderResourceHelper_;
	//값형인 이유: 렌더러마다 각각의 셰이더리소스헬퍼를 가지게 하기 위해서.

private:
	GameEngineRenderingPipeLine* renderingPipeLine_;
	//공유해야하는 메모리를 가지고 있다??
};

