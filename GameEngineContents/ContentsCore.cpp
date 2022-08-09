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


	//RTTI(Run-Time Type information): 런타임 형식 정보. 
	//프로그램 실행 중간에 데이터 타입을 받아오는 방법. 

	//GameEngineGUI::CreateGUIWindow<GameEngineStatusWindow>("EngineStatus", nullptr);
	//ImGui 창이 필요하면 복원.

	CreateLevel<TestLevel>("TestLevel");


	ChangeLevel("TestLevel");

	//GameEngineDebug::ConsoleOpen(); 콘솔창이 필요하면 복원.
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
