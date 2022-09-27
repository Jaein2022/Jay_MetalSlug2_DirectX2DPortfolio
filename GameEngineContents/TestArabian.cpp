#include "PreCompile.h"
#include "TestArabian.h"
#include "TestIndicator.h"
#include "TestPixelIndicator.h"
#include "TestLevel.h"
#include "TestSword.h"
#include "TestPlayer.h"

TestArabian::TestArabian()
	: currentArabianState_(ArabianState::Idling),
	isAirborne_(false),
	isEngaging_(false),
	arabianRendererLocalPosX_(0),
	arabianRendererLocalPosY_(75),
	arabianRendererLocalPosZ_(0),
	arabianRenderer_(nullptr),
	arabianLifeCollisionBody_(nullptr),
	arabianCloseCombatCollisionBody_(nullptr),
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	arabianWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	slopeCheckerLocalPosX_(10),				//0 금지!	
	slopeChecker_(nullptr),
	ascendingSlopeChecker_(nullptr),
	flatSlopeChecker_(nullptr),
	descendingSlopeChecker_(nullptr),
	initialJumpSpeed_(5.f),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	shufflingSpeed_(0.5f),
	shuffleDirection_(1),
	movementFor1Second_(float4::Zero),
	turningDelay_(1.f),
	nextWorldDirection_(1),
	releasePoint_(nullptr),
	releaseAngle_(60.f),
	releaseVelocity_(5.f),
	horizontalDistance_(0.f),
	recognitionDistance_(800.f),
	engagementDistance_(600.f),
	chargeDistance_(400.f),
	hp_(3)
{
}

TestArabian::~TestArabian()
{
}

void TestArabian::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	if (0 == slopeCheckerLocalPosX_)
	{
		MsgBoxAssert("slopeCheckerLocalPosX_에 0 넣지 말 것!");
		return;
	}

	arabianLifeCollisionBody_ = CreateComponent<GameEngineCollision>("ArabianLifeCollision");
	arabianLifeCollisionBody_->ChangeOrder(this->GetOrder());
	arabianLifeCollisionBody_->SetCollisionMode(CollisionMode::Single);
	arabianLifeCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	arabianLifeCollisionBody_->GetTransform().SetLocalScale(80, 150, 10);
	arabianLifeCollisionBody_->GetTransform().SetLocalPosition(0, 75, 10);

	arabianCloseCombatCollisionBody_ = CreateComponent<GameEngineCollision>("ArabianCloseCombatCollisionBody");
	arabianCloseCombatCollisionBody_->ChangeOrder(this->GetOrder() + 1);
	arabianCloseCombatCollisionBody_->SetCollisionMode(CollisionMode::Single);
	arabianCloseCombatCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	arabianCloseCombatCollisionBody_->GetTransform().SetLocalScale(160, 190, 10);
	arabianCloseCombatCollisionBody_->GetTransform().SetLocalPosition(80, 95, 10);



	renderPivotPointer_ = TestIndicator::CreateIndicator<TestIndicator>(
		"RenderPivotPointer",
		this,
		float4::Cyan,
		float4(arabianRendererLocalPosX_, arabianRendererLocalPosY_, -5),
		float4(5, 5, 1)
		);

	upperLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
		);

	arabianWorldPosPointer_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"PlayerWorldPosPointer",
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

	slopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"SlopeChecker",
		this,
		float4::Blue,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
		);

	ascendingSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"AscendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 10, -5),
		float4(5, 5, 1)
		);

	flatSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"FlatSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
		);

	descendingSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"DescendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, -10, -5),
		float4(5, 5, 1)
		);

	releasePoint_ = TestIndicator::CreateIndicator<TestIndicator>(
		"ReleasePoint",
		this,
		float4::Red,
		float4(100, 115, 10),
		float4(5, 5, 1)
	);

	CreateArabianAnimations();
	CreateArabianStates();
}

void TestArabian::Update(float _deltaTime)
{
	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	GetDistance(_deltaTime);

	if (recognitionDistance_ > horizontalDistance_ && false == isEngaging_ && false == isAirborne_)
	{
		currentArabianState_ = ArabianState::Running;
		isEngaging_ = true;
	}

	UpdateArabianState(_deltaTime);
	MoveArabian(_deltaTime);
}

void TestArabian::End()
{
}

void TestArabian::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((TestLevel::groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()))
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
				fallingSpeed_ = 0.f;

				if (currentArabianState_ == ArabianState::JumpingBackward)
				{
					currentArabianState_ = ArabianState::Shuffling;
				}
				else
				{
					currentArabianState_ = ArabianState::FallingToIdling;
				}
			}
		}
		else if (TestLevel::groundColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (currentArabianState_ == ArabianState::JumpingBackward)
				{
					currentArabianState_ = ArabianState::Shuffling;
				}
				else
				{
					currentArabianState_ = ArabianState::FallingToIdling;
				}
			}
		}
		else if (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (currentArabianState_ == ArabianState::JumpingBackward)
				{
					currentArabianState_ = ArabianState::Shuffling;
				}
				else
				{
					currentArabianState_ = ArabianState::FallingToIdling;
				}
			}
		}
		else
		{
			if (false == isAirborne_)
			{
				isAirborne_ = true;
				currentArabianState_ = ArabianState::Falling;
			}
		}
	}
}

void TestArabian::Fall(float _deltaTime)
{
	fallingSpeed_ += TestLevel::gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void TestArabian::GetDistance(float _deltaTime)
{
	float playerWorldPosX = GetLevel<TestLevel>()->GetPlayerWorldPosition().x;
	float thisWorldPosX = this->GetTransform().GetWorldPosition().x; 
	horizontalDistance_ = abs(thisWorldPosX - playerWorldPosX);

	static float currentTurningDelay;

	if (playerWorldPosX < thisWorldPosX && 0 < this->GetTransform().GetWorldScale().x)
	{
		//아라비안 보는방향 오른쪽 && 플레이어는 아라비안 왼쪽 위치.
		currentTurningDelay += _deltaTime;
		if (turningDelay_ <= currentTurningDelay)
		{
			nextWorldDirection_ = -1;
		}

	}
	else if(playerWorldPosX > thisWorldPosX && 0 > this->GetTransform().GetWorldScale().x)
	{
		//아라비안 보는방향 왼쪽 && 플레이어는 아라비안 오른쪽 위치.
		currentTurningDelay += _deltaTime;
		if (turningDelay_ <= currentTurningDelay)
		{
			nextWorldDirection_ = 1;
		}
	}
	else
	{
		//아라비안 보는 방향에 플레이어 있으면 방향전환 리셋.
		nextWorldDirection_ = 0;
		currentTurningDelay = 0.f;
	}
}

void TestArabian::UpdateArabianState(float _deltaTime)
{
	if (0 >= hp_)
	{
		currentArabianState_ = ArabianState::Dead;
	}

	if (allArabianStates_[currentArabianState_] != arabianStateManager_.GetCurrentStateName())
	{
		arabianStateManager_.ChangeState(allArabianStates_[currentArabianState_]);
	}

	arabianStateManager_.Update(_deltaTime);
}

void TestArabian::MoveArabian(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * TestLevel::playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void TestArabian::SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...)
{
	if (_maxStateIndex <= _minStateIndex)
	{
		MsgBoxAssert("최대값이 최소값보다 작거나 같습니다. 값을 다시 지정하세요.");
		return;
	}

	if (_maxStateIndex >= static_cast<int>(ArabianState::Dead) || 1 > _maxStateIndex)
	{
		MsgBoxAssert("최대값이 스테이트 범위를 벗어났습니다. 값을 다시 지정하세요.");
		return;
	}

	int nextStateIndex 
		= GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);

	if (0 < _exclusionCount)	//제외할 아라비안스테이트가 1개 이상일때만 진입.
	{
		va_list exclusionList;		//제외할 아라비안스테이트 리스트.
		va_start(exclusionList, _exclusionCount);	//exclusionList 초기화.
		ArabianState exclusion;		//제외할 아라비안스테이트.

		for (int i = 0; i < _exclusionCount; i++)
		{
			exclusion = va_arg(exclusionList, ArabianState);

			if (static_cast<ArabianState>(nextStateIndex) == exclusion)
			{
				//랜덤으로 뽑은 숫자가 다음 스테이트에서 제외할 아라비안스테이트 번호와 같다면 다시 뽑는다.
				nextStateIndex = GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);
				va_end(exclusionList);
				va_start(exclusionList, _exclusionCount);
				i = -1;
				continue;
			}

			if (exclusion > ArabianState::Idling)
			{
				MsgBoxAssert("제외값이 아라비안스테이트 범위를 벗어났습니다. 값을 다시 지정하세요.");
				return;
			}
		}
		//랜덤으로 뽑은 숫자가 제외할 스테이트와 하나도 겹치지 않는다면 통과.
		va_end(exclusionList);
	}


	if (0 >= nextStateIndex)
	{
		nextStateIndex = 0;
	}

	currentArabianState_ = static_cast<ArabianState>(nextStateIndex);
}

void TestArabian::Shuffle()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * shuffleDirection_ * shufflingSpeed_;

	movementFor1Second_ += float4::Up * GetSlope(shuffleDirection_) * shufflingSpeed_;
}

void TestArabian::Run()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_ ;

	movementFor1Second_ += float4::Up * GetSlope(1) * runningSpeed_ ;
}

void TestArabian::SetSlopeCheckerDirection(char _localDirection)
{
	slopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		slopeChecker_->GetTransform().GetLocalPosition().IY(),
		slopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	ascendingSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		ascendingSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	flatSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		flatSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		flatSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	descendingSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		descendingSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		descendingSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
}

float TestArabian::GetSlope(char _localDirection)
{
	if (false == isAirborne_)
	{
		SetSlopeCheckerDirection(_localDirection);

		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (TestLevel::groundColor_.color_ <= ascendingSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_ * _localDirection,
				ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5,
				-5
			);

			if (TestLevel::groundColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				return 0.f;
			}
			else
			{
				beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5;
				endPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			}
		}
		else if (TestLevel::groundColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else if (TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else
		{
			beginPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY() - 5;
		}

		for (slopeCheckPosY = beginPosY; slopeCheckPosY >= endPosY; slopeCheckPosY--)
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_ * _localDirection,
				slopeCheckPosY,
				-5
			);

			if (TestLevel::groundColor_.color_ <= slopeChecker_->GetColorValue_UINT())
			{
				break;
			}
		}

		return static_cast<float>(slopeCheckPosY) / static_cast<float>(slopeCheckerLocalPosX_);
		//액터가 수평방향으로 1픽셀 이동했을때 수직으로는 몇픽셀 이동해야하는지에 대한 값 반환.
	}
	else
	{
#ifdef _DEBUG
		MsgBoxAssert("점프중일때는 의미 없는 기능입니다.");
#endif	
		return 0.f;
	}
}

void TestArabian::ThrowSword()
{
	TestSword* newSword = this->GetLevel<TestLevel>()->GetSword();
	newSword->GetTransform().SetWorldPosition(releasePoint_->GetTransform().GetWorldPosition());

	if (0 > this->GetTransform().GetWorldScale().x)
	{
		newSword->GetTransform().PixLocalPositiveX();
	}
	else
	{
		newSword->GetTransform().PixLocalNegativeX();
	}

	newSword->SetReleaseSpeed(
		abs((90.f - releaseAngle_) - (GetTransform().GetWorldScale().x * 90.f)),
		releaseVelocity_
	);
}

void TestArabian::MoveInJumpDeath(const FrameAnimation_Desc& _desc)
{
	if (8 <= _desc.curFrame_)
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 1.f / _desc.interval_;

		if (false == isAirborne_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 1.f / _desc.interval_;
		}
	}
	else
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 5.f / _desc.interval_;

		if (false == isAirborne_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 5.f / _desc.interval_;
		}
	}
}

void TestArabian::MeleeAttack()
{
	arabianCloseCombatCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodyOrder::Player,
		CollisionType::CT_AABB,
		[this](GameEngineCollision* _thisCollision, GameEngineCollision* _playerCollision)->CollisionReturn {
			_playerCollision->GetActor<TestPlayer>()->TakeDamage(arabianCloseCombatCollisionBody_->GetOrder());
			return CollisionReturn::Stop;
		}
	);
}

void TestArabian::JumpBackWard()
{
	movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 4.5f;
}

