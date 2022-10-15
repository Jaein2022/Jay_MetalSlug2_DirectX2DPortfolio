#include "PreCompile.h"
#include "Berserker.h"
#include "Indicator.h"
#include "PixelIndicator.h"

Berserker::Berserker()
	: currentBerserkerState_(BerserkerState::MeleeAttack),
	isAirborne_(false),
	isEngaging_(false),
	berserkerRendererLocalPos_(0, 75, 0),
	berserkerWholeRenderer_(nullptr),
	berserkerTopRenderer_(nullptr),
	berserkerLegRenderer_(nullptr),
	berserkerLifeCollisionBody_(nullptr),
	berserkerCloseCombatCollisionBody_(nullptr),
	upperLandingChecker_(nullptr),
	midLandingChecker_(nullptr),
	lowerLandingChecker_(nullptr),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	movementFor1Second_(float4::Zero),
	enemySoldier_(nullptr),
	currentTurningDelay_(0.f),
	turningDelay_(1.f),
	isBerserkerDirectionWrong_(false),
	horizontalDistance_(0.f),
	hp_(3),
	flickeringPeriod_(0.05f),
	remainingPeriod_(0.f),
	flickeringSwitch_(true)
{
}

Berserker::~Berserker()
{
}

void Berserker::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	midLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"MidLandingChecker",
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


	if (0 == GameEngineTexture::Find("Berserker_Whole.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Berserker_Whole.png", 10, 6);
	}	
	
	if (0 == GameEngineTexture::Find("Berserker_Top.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Berserker_Top.png", 10, 2);
	}	
	
	if (0 == GameEngineTexture::Find("Berserker_Leg.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Berserker_Leg.png", 10, 1);
	}

	berserkerWholeRenderer_ = CreateComponent<GameEngineTextureRenderer>("BerserkerWholeRenderer");
	berserkerWholeRenderer_->SetPivot(PivotMode::Center);
	berserkerWholeRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	berserkerWholeRenderer_->GetTransform().SetLocalPosition(berserkerRendererLocalPos_);
	berserkerWholeRenderer_->SetTexture("Berserker_Whole.png");
	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("Waiting_SittingDown",
		FrameAnimation_Desc("Berserker_Whole.png", 0, 5, 0.15f, true)
	);
	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("Waiting_Standing",
		FrameAnimation_Desc("Berserker_Whole.png", 10, 15, 0.2f, true)
	);
	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("Idling",
		FrameAnimation_Desc("Berserker_Whole.png", 20, 23, 0.15f, true)
	);
	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("Turning",
		FrameAnimation_Desc("Berserker_Whole.png", 30, 32, 0.1f, false)
	);
	berserkerWholeRenderer_->AnimationBindEnd("Turning",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentBerserkerState_ = BerserkerState::Idling;

			if (0 > this->GetTransform().GetWorldScale().x)
			{
				this->GetTransform().PixLocalPositiveX();
			}
			else
			{
				this->GetTransform().PixLocalNegativeX();
			}
		}
	);


	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("JumpDeath",
		FrameAnimation_Desc("Berserker_Whole.png", 40, 48, 0.5f, true)
	);
	berserkerWholeRenderer_->CreateFrameAnimation_CutTexture("Death",
		FrameAnimation_Desc("Berserker_Whole.png", 50, 58, 0.05f, true)
	);
	berserkerWholeRenderer_->ChangeFrameAnimation("Idling");


	berserkerTopRenderer_ = CreateComponent<GameEngineTextureRenderer>("BerserkerTopRenderer");
	berserkerTopRenderer_->SetPivot(PivotMode::Center);
	berserkerTopRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	berserkerTopRenderer_->GetTransform().SetLocalPosition(berserkerRendererLocalPos_);
	berserkerTopRenderer_->SetTexture("Berserker_Top.png");
	berserkerTopRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Berserker_Top.png", 0, 5, 0.1f, true)
	);

	berserkerTopRenderer_->CreateFrameAnimation_CutTexture("MeleeAttack",
		FrameAnimation_Desc("Berserker_Top.png", 10, 15, 0.1f, true)
	);

	berserkerTopRenderer_->ChangeFrameAnimation("Running");

	berserkerLegRenderer_ = CreateComponent<GameEngineTextureRenderer>("BerserkerLegRenderer");
	berserkerLegRenderer_->SetPivot(PivotMode::Center);
	berserkerLegRenderer_->SetTexture("Berserker_Leg.png");
	berserkerLegRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	berserkerLegRenderer_->GetTransform().SetLocalPosition(
		berserkerRendererLocalPos_.x,
		berserkerRendererLocalPos_.y,
		berserkerRendererLocalPos_.z + 1.f
	);
	berserkerLegRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Berserker_Leg.png", 0, 5, 0.1f, true)
	);
	berserkerLegRenderer_->AnimationBindTime("Running",
		std::bind(&Berserker::Run, this)
	);

	berserkerTopRenderer_->ChangeFrameAnimation("Running");




	berserkerStateManager_.CreateState("Waiting_SittingDown",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->On();
			berserkerTopRenderer_->Off();
			berserkerLegRenderer_->Off();

			berserkerWholeRenderer_->ChangeFrameAnimation("Waiting_SittingDown");
		}
	);

	berserkerStateManager_.CreateState("Waiting_Standing",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->On();
			berserkerTopRenderer_->Off();
			berserkerLegRenderer_->Off();

			berserkerWholeRenderer_->ChangeFrameAnimation("Waiting_Standing");
		}
	);

	berserkerStateManager_.CreateState("Idling",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->On();
			berserkerTopRenderer_->Off();
			berserkerLegRenderer_->Off();

			berserkerWholeRenderer_->ChangeFrameAnimation("Idling");
		}
	);

	berserkerStateManager_.CreateState("Turning",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->On();
			berserkerTopRenderer_->Off();
			berserkerLegRenderer_->Off();

			berserkerWholeRenderer_->ChangeFrameAnimation("Turning");
		}
	);

	berserkerStateManager_.CreateState("Running",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->Off();
			berserkerTopRenderer_->On();
			berserkerLegRenderer_->On();

			berserkerTopRenderer_->ChangeFrameAnimation("Running");
			berserkerLegRenderer_->ChangeFrameAnimation("Running");
		}
	);

	berserkerStateManager_.CreateState("MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->Off();
			berserkerTopRenderer_->On();
			berserkerLegRenderer_->On();

			berserkerTopRenderer_->ChangeFrameAnimation("MeleeAttack");
			berserkerLegRenderer_->ChangeFrameAnimation("Running");
		}
	);

	berserkerStateManager_.CreateState("Dead",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			berserkerWholeRenderer_->On();
			berserkerTopRenderer_->Off();
			berserkerLegRenderer_->Off();

			berserkerWholeRenderer_->ChangeFrameAnimation("Death");
		}
	);


	std::vector<std::pair<BerserkerState, std::string_view>> berserkerStateEntries(
		magic_enum::enum_entries<BerserkerState>().begin(), magic_enum::enum_entries<BerserkerState>().end()
	);

	size_t berserkerStateCount = berserkerStateEntries.size();
	allBerserkerStates_.reserve(berserkerStateCount);

	for (size_t i = 0; i < berserkerStateCount; i++)
	{
		if (allBerserkerStates_.end() != allBerserkerStates_.find(berserkerStateEntries[i].first))
		{
			MsgBoxAssertString(std::string(berserkerStateEntries[i].second) + std::string(": 이미 존재하는 스테이트입니다."));
			return;
		}

		allBerserkerStates_.insert(
			std::make_pair(
				berserkerStateEntries[i].first, berserkerStateEntries[i].second.data()
			)
		);
	}

	berserkerStateManager_.ChangeState(allBerserkerStates_[currentBerserkerState_]);

}

void Berserker::Update(float _deltaTime)
{
	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	UpdateDistanceAndDirection(_deltaTime);

	UpdateBerserkerState(_deltaTime);
	MoveBerserker(_deltaTime);
}

void Berserker::End()
{
}

void Berserker::TakeDamage(int _damage, GameEngineCollision* _soldierWeaponCollision, GameEngineCollision* _rebelCollision)
{
}

void Berserker::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if (true == upperLandingChecker_->IsOnSteppablePixel()
			&& true == lowerLandingChecker_->IsOnSteppablePixel()
			&& true == midLandingChecker_->IsOnSteppablePixel())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (true == lowerLandingChecker_->IsOnSteppablePixel()
			&& true == midLandingChecker_->IsOnSteppablePixel())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (true == lowerLandingChecker_->IsOnSteppablePixel())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
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

void Berserker::Fall(float _deltaTime)
{
	fallingSpeed_ += gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void Berserker::UpdateDistanceAndDirection(float _deltaTime)
{
}

void Berserker::UpdateBerserkerState(float _deltaTime)
{
	if (0 >= hp_)
	{
		currentBerserkerState_ = BerserkerState::Dead;
	}

	if (allBerserkerStates_[currentBerserkerState_] != berserkerStateManager_.GetCurrentStateName())
	{
		berserkerStateManager_.ChangeState(allBerserkerStates_[currentBerserkerState_]);
	}

	berserkerStateManager_.Update(_deltaTime);
}

void Berserker::MoveBerserker(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void Berserker::Run()
{
}

float Berserker::GetSlope(char _localDirection)
{
	return 0.0f;
}

void Berserker::MoveInJumpDeath(const FrameAnimation_Desc& _desc)
{
}

void Berserker::MeleeAttack()
{
}

void Berserker::Flicker(float _deltaTime, bool _isFlickeringOn, const float4& _plusColor, const float4& _originalColor)
{
}
