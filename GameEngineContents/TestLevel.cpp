#include "PreCompile.h"
#include "TestLevel.h"
#include "TestPlayer.h"

TestLevel::TestLevel(): testPlayer_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	testPlayer_ = CreateActor<TestPlayer>(0, "TestPlayer");

	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
	}
}

void TestLevel::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();

	}
}

void TestLevel::End()
{
}
