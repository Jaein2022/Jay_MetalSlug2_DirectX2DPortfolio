#include "PreCompile.h"
#include "TestLevel.h"
#include "TestPlayer.h"
#include "TestBackground.h"
#include "TestIndicator.h"
#include <GameEngineCore\GameEngineDevice.h>

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
	TestIndicator::SetTexture("TestBG_PC.png");

	testPlayer_ = CreateActor<TestPlayer>(0, "TestPlayer");

	testBackground_ = CreateActor<TestBackground>(0, "TestBackground");

	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
	}

	//GameEngineStatusWindow::AddDebugRenderTarget("TestLevel MainCamera", GetMainCamera()->GetCameraRenderTarget());
	//GameEngineStatusWindow::AddDebugRenderTarget("BackBuffer", GameEngineDevice::GetBackBuffer());

}

void TestLevel::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	if (GetMainCameraActor()->GetTransform().GetWorldPosition().x
		<= testPlayer_->GetTransform().GetWorldPosition().x)
	{
		GetMainCameraActor()->GetTransform().SetWorldPosition(
			testPlayer_->GetTransform().GetWorldPosition().IX(),
			0, 
			-100
		);
	}


}

void TestLevel::End()
{
}
