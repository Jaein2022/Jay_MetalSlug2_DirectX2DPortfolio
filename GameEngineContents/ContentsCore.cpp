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
	//GameEngineDebug::ConsoleOpen(); �ܼ�â�� �ʿ��ϸ� ����.

	//RTTI(Run-Time Type information): ��Ÿ�� ���� ����. 
	//���α׷� ���� �߰��� ������ Ÿ���� �޾ƿ��� ���. 

	GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);

	CreateLevel<TempLevel>("Temp");


	ChangeLevel("Temp");

}

void ContentsCore::Update(float _deltaTime)
{
}

void ContentsCore::End()
{
}
