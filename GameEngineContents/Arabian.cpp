#include "PreCompile.h"
#include "Arabian.h"
#include "Indicator.h"
#include "PixelIndicator.h"
#include "Sword.h"
#include "Soldier.h"

Arabian::Arabian()
	: currentArabianState_(ArabianState::Idling),
	isAirborne_(false),
	isEngaging_(false),
	arabianRendererLocalPos_(0, 75, 0),
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
	currentSteppingColor_(0, 255, 255, 255),
	initialJumpSpeed_(1.5f),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	shufflingSpeed_(0.5f),
	shuffleDirection_(1),
	movementFor1Second_(float4::Zero),
	enemySoldier_(nullptr),
	currentTurningDelay_(0.f),
	turningDelay_(1.f),
	isArabiansDirectionWrong_(false),
	releasePoint_(nullptr),
	releaseAngle_(60.f),
	releaseVelocity_(5.f),
	horizontalDistance_(0.f),
	recognitionDistance_(900.f),
	engagementDistance_(600.f),
	chargeDistance_(400.f),
	hp_(1),
	flickeringPeriod_(0.05f),
	remainingPeriod_(0.f),
	flickeringSwitch_(true)
{
}

Arabian::~Arabian()
{
}

void Arabian::Start()
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
	arabianCloseCombatCollisionBody_->SetCollisionMode(CollisionMode::Multiple);
	arabianCloseCombatCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	arabianCloseCombatCollisionBody_->GetTransform().SetLocalScale(160, 190, 10);
	arabianCloseCombatCollisionBody_->GetTransform().SetLocalPosition(80, 95, 10);



	renderPivotPointer_ = Indicator::CreateIndicator<Indicator>(
		"RenderPivotPointer",
		this,
		float4::Cyan,
		float4(arabianRendererLocalPos_.x, arabianRendererLocalPos_.y, -5.f),
		float4(5, 5, 1)
	);

	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	arabianWorldPosPointer_ = Indicator::CreateIndicator<PixelIndicator>(
		"ArabianWorldPosPointer",
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

	slopeChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"SlopeChecker",
		this,
		float4::Blue,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
	);

	ascendingSlopeChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"AscendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 10, -5),
		float4(5, 5, 1)
	);

	flatSlopeChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"FlatSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
	);

	descendingSlopeChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"DescendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, -10, -5),
		float4(5, 5, 1)
	);

	releasePoint_ = Indicator::CreateIndicator<Indicator>(
		"ReleasePoint",
		this,
		float4::Red,
		float4(100, 115, 10),
		float4(5, 5, 1)
	);

	enemySoldier_ = this->GetLevel()->GetConvertedGroup<Soldier>(CollisionBodyOrder::Soldier).front();
	//아라비안 생성은 반드시 솔저 생성 이후로 할 것.
	//솔저(플레이어)가 한명 이상으로 늘어나면 문제 발생.

	if (nullptr == enemySoldier_)
	{
		MsgBoxAssert("모든 반란군 액터들은 솔저 생성 후 생성되어야 합니다.");
		return;
	}


	CreateArabianAnimations();
	CreateArabianStates();
}

void Arabian::Update(float _deltaTime)
{
	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	UpdateDistanceAndDirection(_deltaTime);

	if (recognitionDistance_ > horizontalDistance_ && false == isEngaging_ && false == isAirborne_)
	{
		currentArabianState_ = ArabianState::Running;
		isEngaging_ = true;
	}

	UpdateArabianState(_deltaTime);
	MoveArabian(_deltaTime);
}

void Arabian::End()
{
}

void Arabian::TakeDamage(
	int _damage,
	GameEngineCollision* _soldierWeaponCollision,
	GameEngineCollision* _rebelCollision
)
{
	hp_ -= _damage;
}

void Arabian::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((currentSteppingColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (currentSteppingColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (currentSteppingColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()))
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
				currentSteppingColor_ = arabianWorldPosPointer_->GetColorValue_UINT();

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
		else if (currentSteppingColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()
			&& currentSteppingColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
				currentSteppingColor_ = arabianWorldPosPointer_->GetColorValue_UINT();

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
		else if (currentSteppingColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
				currentSteppingColor_ = arabianWorldPosPointer_->GetColorValue_UINT();

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

void Arabian::Fall(float _deltaTime)
{
	fallingSpeed_ += gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void Arabian::UpdateDistanceAndDirection(float _deltaTime)
{ 
	float soldierWorldPosX = enemySoldier_->GetTransform().GetWorldPosition().x;

	float thisWorldPosX = this->GetTransform().GetWorldPosition().x; 
	horizontalDistance_ = abs(thisWorldPosX - soldierWorldPosX);

	//static float currentTurningDelay; 모든 아라비안들이 방향전환 딜레이를 공유하는 문제 발생.

	if ((soldierWorldPosX < thisWorldPosX && 0 < this->GetTransform().GetWorldScale().x)
		|| (soldierWorldPosX > thisWorldPosX && 0 > this->GetTransform().GetWorldScale().x))
	{
		currentTurningDelay_ += _deltaTime;
		if (turningDelay_ <= currentTurningDelay_)
		{
			isArabiansDirectionWrong_ = true;
			currentTurningDelay_ = 0.f;
		}
	}
	else
	{
		//아라비안 보는 방향에 솔저 있으면 방향전환 리셋.
		currentTurningDelay_ = 0.f;
	}
}

void Arabian::ReactInShuffling()
{
	if (true == isArabiansDirectionWrong_)
	{
		currentArabianState_ = ArabianState::Turning;
		return;
	}

	if (groundColor_ > currentSteppingColor_)
	{
		SelectNextState(0, 6, 1, ArabianState::MeleeAttack);
		return;
	}

	if (chargeDistance_ > horizontalDistance_)
	{
		SelectNextState(0, 3, 1, ArabianState::JumpingBackward);
	}
	else if (engagementDistance_ > horizontalDistance_)
	{
		SelectNextState(0, 4, 1, ArabianState::JumpingBackward);
	}
	else if (engagementDistance_ < horizontalDistance_ && true == isEngaging_)
	{
		currentArabianState_ = ArabianState::Running;
	}
}

void Arabian::UpdateArabianState(float _deltaTime)
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

void Arabian::MoveArabian(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void Arabian::SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...)
{
	if (_maxStateIndex < _minStateIndex)
	{
		MsgBoxAssert("최대값이 최소값보다 작습니다. 값을 다시 지정하세요.");
		return;
	}

	if (_maxStateIndex >= static_cast<int>(ArabianState::Dead) || 0 > _maxStateIndex)
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

void Arabian::Shuffle()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * shuffleDirection_ * shufflingSpeed_;

	movementFor1Second_ += float4::Up * GetSlope(shuffleDirection_) * shufflingSpeed_;
}

void Arabian::Run()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_ ;

	movementFor1Second_ += float4::Up * GetSlope(1) * runningSpeed_ ;
}

void Arabian::Jump(const FrameAnimation_Desc& _desc)
{
	if (5 == _desc.curFrame_ && false == isAirborne_)	//5번프레임부터 점프 시작.
	{
		fallingSpeed_ = -initialJumpSpeed_;
		isAirborne_ = true;
		currentSteppingColor_
			= PixelColor(
				static_cast<unsigned char>(currentSteppingColor_.r + 2),
				currentSteppingColor_.g,
				currentSteppingColor_.b,
				currentSteppingColor_.a
			);

	}
	else if (5 < _desc.curFrame_ && true == isAirborne_)
	{
		//점프중 횡이동.
		movementFor1Second_ += float4::Right * this->GetTransform().GetWorldScale().x * shufflingSpeed_;
	}

}

void Arabian::SetSlopeCheckerDirection(char _localDirection)
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

float Arabian::GetSlope(char _localDirection)
{
	if (false == isAirborne_)
	{
		SetSlopeCheckerDirection(_localDirection);

		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (currentSteppingColor_.color_ <= ascendingSlopeChecker_->GetColorValue_UINT()
			&& currentSteppingColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& currentSteppingColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_ * _localDirection,
				ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5,
				-5
			);

			if (currentSteppingColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				return 0.f;
			}
			else
			{
				beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5;
				endPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			}
		}
		else if (currentSteppingColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& currentSteppingColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else if (currentSteppingColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
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

			if (currentSteppingColor_.color_ <= slopeChecker_->GetColorValue_UINT())
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

void Arabian::ThrowSword()
{
	Sword* newSword = this->GetLevel()->CreateActor<Sword>(
		CollisionBodyOrder::RebelAttack_FlyingSword,
		"FlyingSword"
	);

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

void Arabian::MoveInJumpDeath(const FrameAnimation_Desc& _desc)
{
	if (8 <= _desc.curFrame_)
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 1.f;

		if (false == isAirborne_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 1.f;
		}
	}
	else
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 2.f;

		if (false == isAirborne_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 5.f;
		}
	}
}

void Arabian::MeleeAttack()
{
	arabianCloseCombatCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodyOrder::Soldier,
		CollisionType::CT_AABB,
		[this](GameEngineCollision* _thisCollision, GameEngineCollision* _playerCollision)->CollisionReturn {
			_playerCollision->GetActor<Soldier>()->TakeDamage(arabianCloseCombatCollisionBody_->GetOrder());
			return CollisionReturn::Stop;
		}
	);
}

void Arabian::JumpBackWard()
{
	movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 4.5f;
}

void Arabian::Flicker(float _deltaTime, bool _isFlickeringOn, const float4& _plusColor, const float4& _originalColor)
{
	if (false == _isFlickeringOn && true == flickeringSwitch_)
	{
		remainingPeriod_ = 0.f;
	}

	if (0 >= remainingPeriod_)
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
		arabianRenderer_->GetPixelData().plusColor_ = _plusColor;
		
	}
	else
	{
		arabianRenderer_->GetPixelData().plusColor_ = _originalColor;
	}
}

