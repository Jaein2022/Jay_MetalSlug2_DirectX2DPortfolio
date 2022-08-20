#include "PreCompile.h"
#include "ContentsCore.h"
#include "TestLevel.h"
#include "TestBackground.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::Start()
{
	LoadContentsResource();


	//RTTI(Run-Time Type information): ��Ÿ�� ���� ����. 
	//���α׷� ���� �߰��� ������ Ÿ���� �޾ƿ��� ���. 

	//GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);
	//ImGui â�� �ʿ��ϸ� ����.

	CreateLevel<TestLevel>("TestLevel");


	ChangeLevel("TestLevel");

	//GameEngineDebug::ConsoleOpen(); �ܼ�â�� �ʿ��ϸ� ����.


	if (false == GameEngineInput::GetInst()->IsKey("Left"))
	{
		GameEngineInput::GetInst()->CreateKey("Left", 'J');
		GameEngineInput::GetInst()->CreateKey("Right", 'L');
		GameEngineInput::GetInst()->CreateKey("Up", 'I');
		GameEngineInput::GetInst()->CreateKey("Down", 'K');

		GameEngineInput::GetInst()->CreateKey("Attack", 'X');
		GameEngineInput::GetInst()->CreateKey("Jump", 'Z');
		GameEngineInput::GetInst()->CreateKey("Special", 'S');

		GameEngineInput::GetInst()->CreateKey("Test", 'T');
	}


}

void ContentsCore::Update(float _deltaTime)
{
}

void ContentsCore::End()
{
}

void ContentsCore::LoadContentsResource()
{
	GameEngineDirectory contentsResourceDir;
	contentsResourceDir.MoveParentToExistChildDirectory("ContentsResource");
	contentsResourceDir.MoveToChild("ContentsResource");
	contentsResourceDir.MoveToChild("Texture");

	std::vector<GameEngineFile> contentsTextures = contentsResourceDir.GetAllFiles();

	for (size_t i = 0; i < contentsTextures.size(); i++)
	{
		GameEngineTexture::Load(contentsTextures[i].GetFullPath());
	}

}
