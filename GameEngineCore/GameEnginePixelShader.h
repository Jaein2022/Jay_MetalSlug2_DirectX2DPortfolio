#pragma once
#include "GameEngineRes.h"
#include "GameEngineShader.h"

class GameEnginePixelShader : public GameEngineRes<GameEnginePixelShader>, public GameEngineShader
{
	//픽셀셰이더: 각 픽셀의 색깔을 결정한다.

	friend GameEngineRes<GameEnginePixelShader>;
	//GameEnginePixelShader 클래스의 프라이빗 소멸자를 GameEngineRes클래스에서 호출하기 위한 방법.

	friend GameEngineShader;

private:
	GameEnginePixelShader();
	~GameEnginePixelShader();
	//외부에서 제멋대로 리소스를 생성/삭제하는걸 막기 위해서 생성자/소멸자를 프라이빗으로 지정해서 외부 접근을 막는다.
	//이 프레임워크의 리소스는 반드시 소멸자가 아니라 ResourceDestroy()함수에서 제거해야 한다.
	//프로그램 끝날때까지 리소스삭제를 안하면 끝나는 문제지만 그래도 최대한 막아둔다.

	GameEnginePixelShader(const GameEnginePixelShader& _other) = delete;
	GameEnginePixelShader(GameEnginePixelShader&& _other) noexcept = delete;
	GameEnginePixelShader& operator=(const GameEnginePixelShader& _other) = delete;
	GameEnginePixelShader& operator=(const GameEnginePixelShader&& _other) = delete;


public:
	void Setting();	//해당 리소스를 렌더링 파이프라인에 연결하는 함수.

private:
	static GameEnginePixelShader* Load(
		const std::string& _path,
		const std::string& _entryPoint,
		UINT _versionHigh = 5,
		UINT _versionLow = 0);
	static GameEnginePixelShader* Load(
		const std::string& _path,
		const std::string& _name,
		const std::string& _entryPoint,
		UINT _versionHigh = 5,
		UINT _versionLow = 0);
	void CreatePixelShader();

private:
	ID3D11PixelShader* pixelShader_;

};

