#include "PreCompile.h"
#include "TestLevel.h"
#include "TestPlayer.h"
#include "TestBackground.h"
#include "TestPointer.h"
#include "TestPointerBase.h"

//#include <GameEngineCore\GameEngineDevice.h>

const float TestLevel::gravity_ = 9.80665f;
const float TestLevel::playSpeed_ = 100.f;

TestLevel::TestLevel()
	: testPlayer_(nullptr),
	testBackground_(nullptr),
	currentFocusPointer_(nullptr),
	destFocus_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	TestPointer::SetTexture("TestBG_PC.png");

	testPlayer_ = CreateActor<TestPlayer>(0, "TestPlayer");

	testBackground_ = CreateActor<TestBackground>(0, "TestBackground");

	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
	}

	currentFocusPointer_ = TestPointer::CreatePointer(
		"CurrentFocusPointer",
		this->GetMainCameraActor(),
		float4::Black,
		float4(0, 0, 5),
		float4(15, 15, 1)
	);

	destFocus_ = CreateActor<TestPointerBase>(0, "DestFocus");
	destFocus_->SetPointerColor(float4::Yellow);
	destFocus_->GetTransform().SetWorldPosition(
		float4(0, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));


	//GameEngineStatusWindow::AddDebugRenderTarget("TestLevel MainCamera", GetMainCamera()->GetCameraRenderTarget());
	//GameEngineStatusWindow::AddDebugRenderTarget("BackBuffer", GameEngineDevice::GetBackBuffer());

}

void TestLevel::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
		currentFocusPointer_->Off();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	if (destFocus_->GetTransform().GetWorldPosition().x
		<= testPlayer_->GetTransform().GetWorldPosition().x + 75.f)
	{
		destFocus_->GetTransform().SetWorldMove(
			float4::Right * _deltaTime * 3.f * TestLevel::playSpeed_);
	}


	UpdateCameraActorMovement(_deltaTime);

}

void TestLevel::End()
{
}

void TestLevel::UpdateCameraActorMovement(float _deltaTime)
{
	GetMainCameraActor()->GetTransform().SetWorldPosition(
		float4::Lerp(
			GetMainCameraActor()->GetTransform().GetWorldPosition(),
			destFocus_->GetTransform().GetWorldPosition(),
			_deltaTime * 10.0f
		)
	);

	


}
