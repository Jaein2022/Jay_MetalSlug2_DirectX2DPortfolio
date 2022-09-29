#include "PreCompile.h"
#include "ContentsCore.h"
#include "TestLevel.h"
#include "Mission1.h"

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

	GameEngineRandom::mainRandom_;

	CreateLevel<TestLevel>("TestLevel");

	CreateLevel<Mission1>("Mission1");

	ChangeLevel("Mission1");

	//GameEngineDebug::ConsoleOpen(); 콘솔창이 필요하면 복원.


	//GameEngineTime::SetFrameLimit(60);	프레임 제한이 필요하면 복원.
	//그런데 프레임 제한을 거니 프레임 드랍이 너무 심하므로 사용 금지.



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

		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
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
