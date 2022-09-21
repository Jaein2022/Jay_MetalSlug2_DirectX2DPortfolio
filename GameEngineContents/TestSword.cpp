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
	isAirborne_(true),
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	swordWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	rotationSpeed_(0.f),
	flickeringPeriod_(0.1f),
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
	swordCollisionBody_->SetCollisionMode(CollisionMode::Single);
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

	isAirborne_ ? Fly(_deltaTime) : StickOnGround(_deltaTime);

	if (true == isAirborne_)
	{
		swordCollisionBody_->IsCollision(
			CollisionType::CT_AABB,
			CollisionBodyOrder::Player,
			CollisionType::CT_AABB,
			std::bind(&TestSword::Hit, this, std::placeholders::_1, std::placeholders::_2)
		);
	}
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

void TestSword::StickOnGround(float _deltaTime)
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


			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (TestLevel::groundColor_.color_ <= swordWorldPosPointer_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				releaseSpeed_ = float4::Zero;
			}
		}
		else if (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
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

CollisionReturn TestSword::Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _playerCollision)
{
	_playerCollision->GetActor<TestPlayer>()->TakeDamage(this->GetOrder());

	this->Death();

	return CollisionReturn::Stop;
}

void TestSword::Flicker(
	float _deltaTime,
	bool _isFlickeringOn,
	const float4& _plusColor,
	const float4& _originalColor /*= float4::Zero*/
)
{
	static float passedTime;
	static bool flickeringSwitch;

	if (false == _isFlickeringOn && true == flickeringSwitch)
	{
		passedTime = flickeringPeriod_;
	}

	if (flickeringPeriod_ <= passedTime)
	{
		flickeringSwitch = !flickeringSwitch;

		passedTime = 0.f;
	}
	else
	{
		if (true == _isFlickeringOn)
		{
			passedTime += _deltaTime;
		}
		return;
	}

	if (true == flickeringSwitch)
	{
		//for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
		//	iter != allTextureRenderers_.end(); iter++)
		//{
		//	(*iter)->GetPixelData().plusColor_ = _plusColor;
		//}
		stuckSwordRenderer_->GetPixelData().plusColor_ = _plusColor;
	}
	else
	{
		//for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
		//	iter != allTextureRenderers_.end(); iter++)
		//{
		//	(*iter)->GetPixelData().plusColor_ = _originalColor;
		//}
		stuckSwordRenderer_->GetPixelData().plusColor_ = _originalColor;
	}
}
