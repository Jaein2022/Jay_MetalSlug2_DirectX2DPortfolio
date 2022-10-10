#include "PreCompile.h"
#include "Sword.h"
#include "TestLevel.h"
#include "Indicator.h"
#include "PixelIndicator.h"
#include "Soldier.h"

Sword::Sword()
	: swordRendererLocalPos_(0, 55, 0),
	flyingSwordRenderer_(nullptr),
	stuckSwordRenderer_(nullptr),
	swordCollisionBody_(nullptr),
	isAirborne_(true),
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	swordWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	rotationSpeed_(0.f),
	flickeringPeriod_(0.1f),
	remainingPeriod_(0.f),
	flickeringSwitch_(true),
	releaseSpeed_(float4::Zero)
{
}

Sword::~Sword()
{
}

void Sword::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	if (0 == GameEngineTexture::Find("FlyingSword.png")->GetCutCount())
	{
		GameEngineTexture::Cut("FlyingSword.png", 3, 1);
	}

	flyingSwordRenderer_ = CreateComponent<GameEngineTextureRenderer>("FlyingSwordRenderer");
	flyingSwordRenderer_->SetTexture("FlyingSword.png");
	flyingSwordRenderer_->SetPivot(PivotMode::Center);
	flyingSwordRenderer_->CreateFrameAnimation_CutTexture("Fly",
		FrameAnimation_Desc("FlyingSword.png", 0.05f)
	);
	flyingSwordRenderer_->GetTransform().SetLocalScale(40, 132, 1);
	flyingSwordRenderer_->GetTransform().SetLocalPosition(swordRendererLocalPos_);
	flyingSwordRenderer_->ChangeFrameAnimation("Fly");


	stuckSwordRenderer_ = CreateComponent<GameEngineTextureRenderer>("StuckSwordRenderer");
	stuckSwordRenderer_->SetTexture("StuckSword.png");
	stuckSwordRenderer_->ScaleToTexture();
	stuckSwordRenderer_->SetPivot(PivotMode::Center);
	stuckSwordRenderer_->GetTransform().SetLocalPosition(swordRendererLocalPos_);
	stuckSwordRenderer_->Off();

	swordCollisionBody_ = CreateComponent<GameEngineCollision>("SwordCollision");
	swordCollisionBody_->ChangeOrder(this->GetOrder());
	swordCollisionBody_->SetCollisionMode(CollisionMode::Single);
	swordCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	swordCollisionBody_->GetTransform().SetLocalScale(70, 70, 10);
	swordCollisionBody_->GetTransform().SetLocalPosition(swordRendererLocalPos_.x, swordRendererLocalPos_.y, 0.f);


	renderPivotPointer_ = Indicator::CreateIndicator<Indicator>(
		"RenderPivotPointer",
		this,
		float4::Cyan,
		float4(swordRendererLocalPos_.x, swordRendererLocalPos_.y, -5.f),
		float4(5, 5, 1)
		);

	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
		);

	swordWorldPosPointer_ = Indicator::CreateIndicator<PixelIndicator>(
		"SwordWorldPosPointer",
		this,
		float4::Red,
		float4(0, 0, -5),
		float4(5, 5, 1)
		);

	lowerLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"LowerLandingChecker",
		this,
		float4::Black,
		float4(0, -5, -5),
		float4(5, 5, 1)
		);
	



}

void Sword::Update(float _deltaTime)
{
	CheckGround();

	isAirborne_ ? Fly(_deltaTime) : StickOnGround(_deltaTime);

	if (true == isAirborne_)
	{
		swordCollisionBody_->IsCollision(
			CollisionType::CT_AABB,
			ObjectOrder::Soldier,
			CollisionType::CT_AABB,
			std::bind(&Sword::Hit, this, std::placeholders::_1, std::placeholders::_2)
		);
	}
}

void Sword::End()
{
}

void Sword::Fly(float _deltaTime)
{
	if (false == flyingSwordRenderer_->IsUpdate())
	{
		flyingSwordRenderer_->On();
	}

	flyingSwordRenderer_->GetTransform().SetAddWorldRotation(
		float4::Forward * _deltaTime * rotationSpeed_ * playSpeed_
	);

	this->GetTransform().SetWorldMove(
		releaseSpeed_ * _deltaTime * playSpeed_
	);

	releaseSpeed_.y -= gravity_ * _deltaTime;
}

void Sword::StickOnGround(float _deltaTime)
{
	if (false == stuckSwordRenderer_->IsUpdate())
	{
		swordCollisionBody_->Off();
		flyingSwordRenderer_->Off();
		stuckSwordRenderer_->On();
		this->ResetAccTime();
		Death(2.f);
	}

	if (1.f <= this->GetAccTime())
	{
		Flicker(_deltaTime, true, float4(0, 0, 0, -1));
	}
}

void Sword::CheckGround()
{
	if (0 >= releaseSpeed_.y)
	{
		if ((groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= swordWorldPosPointer_->GetColorValue_UINT()))
		{
			//PixelColor magenta = PixelColor(255, 0, 255, 255);
			//magenta.color_;		//4294902015
			//PixelColor yellow = PixelColor(255, 255, 0, 255);
			//yellow.color_;		//4278255615
			//PixelColor cyan = PixelColor(0, 255, 255, 255);
			//cyan.color_;			//4294967040


			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (groundColor_.color_ <= swordWorldPosPointer_->GetColorValue_UINT()
			&& groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else
		{
			if (false == isAirborne_)
			{
				isAirborne_ = true;
			}
		}
	}
}

CollisionReturn Sword::Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _soldierCollision)
{
	_soldierCollision->GetRoot<Soldier>()->TakeDamage(this->GetOrder());

	this->Death();

	return CollisionReturn::Stop;
}

void Sword::Flicker(
	float _deltaTime,
	bool _isFlickeringOn,
	const float4& _plusColor,
	const float4& _originalColor /*= float4::Zero*/
)
{
	if (false == _isFlickeringOn && true == flickeringSwitch_)
	{
		remainingPeriod_ = 0.f;
	}

	if (0.f >= remainingPeriod_)
	{
		flickeringSwitch_ = !flickeringSwitch_;

		remainingPeriod_ = flickeringPeriod_;
	}
	else
	{
		if (true == _isFlickeringOn)
		{
			remainingPeriod_ -= _deltaTime;
		}
		return;
	}

	if (true == flickeringSwitch_)
	{
		stuckSwordRenderer_->GetPixelData().plusColor_ = _plusColor;
	}
	else
	{
		stuckSwordRenderer_->GetPixelData().plusColor_ = _originalColor;
	}
}
