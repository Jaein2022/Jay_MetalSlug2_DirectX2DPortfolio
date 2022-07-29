#include "PreCompile.h"
#include "ContentsCore.h"
#include "TempLevel.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::Start()
{

	//RTTI(Run-Time Type information): 런타임 형식 정보. 
	//프로그램 실행 중간에 데이터 타입을 받아오는 방법. 

	GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);

	CreateLevel<TempLevel>("Temp");


	ChangeLevel("Temp");

	//GameEngineDebug::ConsoleOpen(); 콘솔창이 필요하면 복원.
}

void ContentsCore::Update(float _deltaTime)
{
}

void ContentsCore::End()
{
}
