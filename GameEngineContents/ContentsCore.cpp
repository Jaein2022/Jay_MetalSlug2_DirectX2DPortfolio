#include "PreCompile.h"
#include "ContentsCore.h"
#include "TestLevel.h"

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
