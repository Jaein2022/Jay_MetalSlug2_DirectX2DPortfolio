#pragma once
#include "GameEngineRes.h"

class GameEngineTexture;
class GameEngineDepthStencilTexture : public GameEngineRes<GameEngineDepthStencilTexture>
{
	//

	friend GameEngineRes<GameEngineDepthStencilTexture>;
	//GameEngineDepthStencilTexture 클래스의 프라이빗 소멸자를 GameEngineRes클래스에서 호출하기 위한 방법.


private:
	GameEngineDepthStencilTexture();
	~GameEngineDepthStencilTexture();
	//외부에서 제멋대로 리소스를 생성/삭제하는걸 막기 위해서 생성자/소멸자를 프라이빗으로 지정해서 외부 접근을 막는다.
	//이 프레임워크의 리소스는 반드시 소멸자가 아니라 ResourceDestroy()함수에서 제거해야 한다.
	//프로그램 끝날때까지 리소스삭제를 안하면 끝나는 문제지만 그래도 최대한 막아둔다.

	GameEngineDepthStencilTexture(const GameEngineDepthStencilTexture& _other) = delete;
	GameEngineDepthStencilTexture(GameEngineDepthStencilTexture&& _other) noexcept = delete;
	GameEngineDepthStencilTexture& operator=(const GameEngineDepthStencilTexture& _other) = delete;
	GameEngineDepthStencilTexture& operator=(const GameEngineDepthStencilTexture&& _other) = delete;


public:
	static GameEngineDepthStencilTexture* Create(const float4& _scale);
	ID3D11DepthStencilView* CreateDepthStencilView();


private:
	void CreateDepthTexture(const float4& _scale);

private:
	GameEngineTexture* depthBuffer_;	//깊이데이터를 저장할 텍스처.

};

