#include "PreCompile.h"
#include "TestLevel.h"
#include "TestPlayer.h"
#include "TestBackground.h"
#include "TestIndicator.h"
#include "TestIndicatorBase.h"
#include "TestPixelIndicator.h"
#include "TestPistolBullet.h"
#include "TestArabian.h"
#include "TestSword.h"

//#include <GameEngineCore\GameEngineDevice.h>

const float TestLevel::gravity_ = 9.80665f;
const float TestLevel::playSpeed_ = 100.f;
const PixelColor TestLevel::groundColor_(0, 255, 255, 255);

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
	TestPixelIndicator::SetTexture("TestBG_PC.png");

	testPlayer_ = CreateActor<TestPlayer>(CollisionBodyOrder::Player, "TestPlayer");

	testBackground_ = CreateActor<TestBackground>(CollisionBodyOrder::Background, "TestBackground");

	testArabian_ = CreateActor<TestArabian>(CollisionBodyOrder::Rebel, "TestArabian");
	testArabian_->GetTransform().SetWorldPosition(950, 0, 0);

	if (false == GameEngineInput::GetInst()->IsKey("FreeCameraOnOff"))
	{
		GameEngineInput::GetInst()->CreateKey("FreeCameraOnOff", 'O');
	}

	currentFocusPointer_ = TestIndicator::CreateIndicator<TestIndicator>(
		"CurrentFocusPointer",
		this->GetMainCameraActor(),
		float4::Black,
		float4(0, 0, 5),
		float4(15, 15, 1)
	);

	destFocus_ = CreateActor<TestIndicatorBase>(CollisionBodyOrder::UI, "DestFocus");
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
			<= testPlayer_->GetTransform().GetWorldPosition().x + 75.f
		&& GameEngineTexture::Find("TestBG_PC.png")->GetScale().x - GameEngineWindow::GetScale().x
			>= destFocus_->GetTransform().GetWorldPosition().x)
	{
		destFocus_->GetTransform().SetWorldMove(
			float4::Right * _deltaTime * 3.f * TestLevel::playSpeed_);
	}

	UpdateCameraActorMovement(_deltaTime);
}

void TestLevel::End()
{
}

TestPistolBullet* TestLevel::GetPistolBullet()
{
	return CreateActor<TestPistolBullet>(CollisionBodyOrder::PlayerProjectile, "TestPistolBullet");
}

TestSword* TestLevel::GetSword()
{
	return CreateActor<TestSword>(CollisionBodyOrder::RebelAttack_FlyingSword, "TestSword");
}

const float4& TestLevel::GetPlayerWorldPosition()
{
	return testPlayer_->GetTransform().GetWorldPosition();
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
