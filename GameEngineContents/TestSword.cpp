#include "PreCompile.h"
#include "TestSword.h"
#include "TestLevel.h"
#include "TestIndicator.h"
#include "TestPixelIndicator.h"
#include "TestPlayer.h"

TestSword::TestSword()
	: swordRendererLocalPosX_(0),
	swordRendererLocalPosY_(55),
	swordRendererLocalPosZ_(0),
	flyingSwordRenderer_(nullptr),
	stuckSwordRenderer_(nullptr),
	swordCollisionBody_(nullptr),
	isFalling_(true),
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	swordWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	rotationSpeed_(0.f),
	releaseSpeed_(float4::Zero)
{
}

TestSword::~TestSword()
{
}

void TestSword::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);


	flyingSwordRenderer_ = CreateComponent<GameEngineTextureRenderer>("FlyingSwordRenderer");
	flyingSwordRenderer_->SetTexture("FlyingSword.png");
	flyingSwordRenderer_->ScaleToTexture();
	flyingSwordRenderer_->SetPivot(PivotMode::Center);
	flyingSwordRenderer_->GetTransform().SetLocalPosition(
		swordRendererLocalPosX_,
		swordRendererLocalPosY_,
		swordRendererLocalPosZ_ 
	);

	stuckSwordRenderer_ = CreateComponent<GameEngineTextureRenderer>("StuckSwordRenderer");
	stuckSwordRenderer_->SetTexture("StuckSword.png");
	stuckSwordRenderer_->ScaleToTexture();
	stuckSwordRenderer_->SetPivot(PivotMode::Center);
	stuckSwordRenderer_->GetTransform().SetLocalPosition(
		swordRendererLocalPosX_,
		swordRendererLocalPosY_,
		swordRendererLocalPosZ_
	);
	stuckSwordRenderer_->Off();

	swordCollisionBody_ = CreateComponent<GameEngineCollision>("SwordCollision");
	swordCollisionBody_->ChangeOrder(this->GetOrder());
	swordCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	swordCollisionBody_->GetTransform().SetLocalScale(70, 70, 10);
	swordCollisionBody_->GetTransform().SetLocalPosition(swordRendererLocalPosX_, swordRendererLocalPosY_, 0);


	renderPivotPointer_ = TestIndicator::CreateIndicator<TestIndicator>(
		"RenderPivotPointer",
		this,
		float4::Cyan,
		float4(swordRendererLocalPosX_, swordRendererLocalPosY_, -5),
		float4(5, 5, 1)
		);

	upperLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
		);

	swordWorldPosPointer_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"SwordWorldPosPointer",
		this,
		float4::Red,
		float4(0, 0, -5),
		float4(5, 5, 1)
		);

	lowerLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"LowerLandingChecker",
		this,
		float4::Black,
		float4(0, -5, -5),
		float4(5, 5, 1)
		);
	



}

void TestSword::Update(float _deltaTime)
{
	CheckGround();

	isFalling_ ? Fly(_deltaTime) : StickOnGround();


	swordCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodySorting::Player,
		CollisionType::CT_AABB,
		std::bind(&TestSword::Hit, this, std::placeholders::_1, std::placeholders::_2)
	);

}

void TestSword::End()
{
}

void TestSword::Fly(float _deltaTime)
{
	if (false == flyingSwordRenderer_->IsUpdate())
	{
		flyingSwordRenderer_->On();
	}

	flyingSwordRenderer_->GetTransform().SetAddWorldRotation(
		float4::Forward * _deltaTime * rotationSpeed_ * TestLevel::playSpeed_
	);

	this->GetTransform().SetWorldMove(
		releaseSpeed_ * _deltaTime * TestLevel::playSpeed_
	);

	releaseSpeed_.y -= TestLevel::gravity_ * _deltaTime;
}

void TestSword::StickOnGround()
{
	if (false == stuckSwordRenderer_->IsUpdate())
	{
		swordCollisionBody_->Off();
		flyingSwordRenderer_->Off();
		stuckSwordRenderer_->On();
		Death(1.f);
	}
}

void TestSword::CheckGround()
{
	if (0 >= releaseSpeed_.y)
	{
		if ((TestLevel::groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= swordWorldPosPointer_->GetColorValue_UINT()))
		{
			//PixelColor magenta = PixelColor(255, 0, 255, 255);
			//magenta.color_;		//4294902015
			//PixelColor yellow = PixelColor(255, 255, 0, 255);
			//yellow.color_;		//4278255615
			//PixelColor cyan = PixelColor(0, 255, 255, 255);
			//cyan.color_;			//4294967040


			if (true == isFalling_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isFalling_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (TestLevel::groundColor_.color_ <= swordWorldPosPointer_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isFalling_)
			{
				isFalling_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isFalling_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isFalling_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else
		{
			if (false == isFalling_)
			{
				isFalling_ = true;
			}
		}
	}
}

bool TestSword::Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _playerCollision)
{
	_playerCollision->GetActor<TestPlayer>()->TakeDamage(this->GetOrder());

	this->Death();

	return true;
}
