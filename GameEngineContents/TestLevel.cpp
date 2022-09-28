#include "PreCompile.h"
#include "TestLevel.h"
#include "Soldier.h"
#include "TestBackground.h"
#include "Indicator.h"
#include "IndicatorBase.h"
#include "PixelIndicator.h"
#include "PistolBullet.h"
#include "Arabian.h"
#include "Sword.h"

TestLevel::TestLevel()
	: testPlayer_(nullptr),
	testBackground_(nullptr),
	testArabian_(nullptr),
	currentFocusPointer_(nullptr),
	destFocus_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	PixelIndicator::SetPCTexture("TestBG_PC.png");

	testPlayer_ = CreateActor<Soldier>(CollisionBodyOrder::Soldier, "TestPlayer");
	testPlayer_->GetTransform().SetWorldPosition(1500, 0, 0);


	testArabian_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "TestArabian");
	testArabian_->GetTransform().SetWorldPosition(1925, 0, 0);

	testBackground_ = CreateActor<TestBackground>(CollisionBodyOrder::Background, "TestBackground");


	currentFocusPointer_ = Indicator::CreateIndicator<Indicator>(
		"CurrentFocusPointer",
		this->GetMainCameraActor(),
		float4::Black,
		float4(0, 0, 5),	//절대 변경하지 말 것.
		float4(15, 15, 1)
	);

	destFocus_ = CreateActor<IndicatorBase>(CollisionBodyOrder::UI, "DestFocus");
	destFocus_->SetPointerColor(float4::Yellow);
	destFocus_->GetTransform().SetWorldPosition(
		float4(1500, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));
	//시작 카메라 위치 변경이 필요하면 여기에서.

	
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
			<= testPlayer_->GetTransform().GetWorldPosition().x + 75.f
		&& GameEngineTexture::Find("TestBG_PC.png")->GetScale().x - GameEngineWindow::GetScale().x
			>= destFocus_->GetTransform().GetWorldPosition().x)
	{
		destFocus_->GetTransform().SetWorldMove(
			float4::Right * _deltaTime * 3.f * playSpeed_);
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
