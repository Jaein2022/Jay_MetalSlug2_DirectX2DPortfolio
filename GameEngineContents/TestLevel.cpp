#include "PreCompile.h"
#include "TestLevel.h"
#include "TestPlayer.h"
#include "TestBackground.h"
#include "TestIndicator.h"

const float TestLevel::gravity_ = 9.80665f;
const float TestLevel::playSpeed_ = 100.f;

TestLevel::TestLevel(): testPlayer_(nullptr), testBackground_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	TestIndicator::SetTexture("TestBg_PixelCollision.png");

	testPlayer_ = CreateActor<TestPlayer>(0, "TestPlayer");

	testBackground_ = CreateActor<TestBackground>(0, "TestBackground");

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
