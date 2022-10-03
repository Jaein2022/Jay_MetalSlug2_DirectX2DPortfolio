#include "PreCompile.h"
#include "Soldier.h"
#include "Indicator.h"
#include "PixelIndicator.h"
#include "PistolBullet.h"
#include "Arabian.h"

Soldier::Soldier()
	: currentSoldierState_(SoldierState::Pistol_Standing_Aiming_Forward),
	weapon_(SoldierWeaponType::Pistol),
	leg_(SoldierLegState::Standing),
	top_(SoldierTopState::Aiming),
	direction_(AimingDirection::Forward),
	isAirborne_(false),
	soldierRendererLocalPos_(0, 75, 0),
	legRenderer_(nullptr),
	topPistolRenderer_(nullptr),
	wholePistolRenderer_(nullptr),
	topWeaponRenderer_(nullptr),
	wholeWeaponRenderer_(nullptr),
	redeployingRenderer_(nullptr),
	horizontalInputValue_(0),
	verticalInputValue_(0),
	isJumpKeyDown_(false),
	isAttackKeyDown_(false),
	isSpecialKeyDown_(false),
	upperLandingChecker_(nullptr),
	soldierWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	slopeCheckerLocalPosX_(10),	//0 금지!	
	slopeChecker_(nullptr),
	ascendingSlopeChecker_(nullptr),
	flatSlopeChecker_(nullptr),
	descendingSlopeChecker_(nullptr),
	muzzleIndicator_(nullptr),
	pistolForwardMuzzlePosition_(100, 105, 10),
	pistolUpwardMuzzlePosition_(-10, 220, 10),
	pistolDownwardMuzzlePosition_(15, -5, 10),
	pistolDuckingMuzzlePosition_(95, 65, 10),
	initialJumpSpeed_(7.f),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	duckStepSpeed_(1.f),
	movementFor1Second_(float4::Zero),
	aimingAngle_(0.f),
	soldierLifeCollisionBody_(nullptr),
	soldierCloseCombatCollisionBody_(nullptr),
	soldierLifeCollisionBodyScale_Standing_(80, 150, 10),
	soldierLifeCollisionBodyPosition_Standing_(0, 75, 10),
	soldierLifeCollisionBodyScale_Ducking_(100, 100, 10),
	soldierLifeCollisionBodyPosition_Ducking_(0, 50, 10),
	meleeAttackDamage_(3),
	isMeleeAttack1_(true),
	causeOfDeath_(0),
	isDamageProof_(false),
	remainedDamageProofDuration_(0.f),
	damageProofTimeLimit_(3.f),
	flickeringPeriod_(0.1f)
{
}

Soldier::~Soldier()
{
}

void Soldier::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	if (0 == slopeCheckerLocalPosX_)
	{
		MsgBoxAssert("slopeCheckerLocalPosX_에 0 넣지 말 것!");
		return;
	}

	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	soldierWorldPosPointer_ = Indicator::CreateIndicator<PixelIndicator>(
		"SoldierWorldPosPointer",
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

	muzzleIndicator_ = Indicator::CreateIndicator<Indicator>(
		"MuzzleIndicator",
		this,
		float4::Red,
		pistolForwardMuzzlePosition_,
		float4(5, 5, 1)
	);

	soldierLifeCollisionBody_ = CreateComponent<GameEngineCollision>("SoldierLifeCollisionBody");
	soldierLifeCollisionBody_->ChangeOrder(this->GetOrder());
	soldierLifeCollisionBody_->SetCollisionMode(CollisionMode::Multiple);
	soldierLifeCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
	soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

	soldierCloseCombatCollisionBody_ = CreateComponent<GameEngineCollision>("SoldierCloseCombatCollisionBody");
	soldierCloseCombatCollisionBody_->ChangeOrder(this->GetOrder() + 1);
	soldierCloseCombatCollisionBody_->SetCollisionMode(CollisionMode::Single);
	soldierCloseCombatCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	soldierCloseCombatCollisionBody_->GetTransform().SetLocalScale(120, 160, 10);
	soldierCloseCombatCollisionBody_->GetTransform().SetLocalPosition(60, 80, 10);

	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.



	CreateSoldierAnimations();
	CreateSoldierStates();
}

void Soldier::Update(float _deltaTime)
{
	CheckGround();

	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	if (0 == causeOfDeath_)
	{
		Flicker(_deltaTime, isDamageProof_, float4(0.25f, 0.25f, 0.25f, 0.f));		
		if (SoldierState::Pistol_Redeploying != currentSoldierState_)
		{
			UpdateInputInfo();
			ConvertInputToSoldierStates();
		}
	}

	UpdateSoldierState(_deltaTime);
	ControlMuzzle();
	MoveSoldier(_deltaTime);


}

void Soldier::End()
{
}

void Soldier::TakeDamage(int _rebelWeaponType)
{
	if (true == isDamageProof_)
	{
		return;
	}

	causeOfDeath_ = _rebelWeaponType;
}

void Soldier::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((steppableObjectColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (steppableObjectColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (steppableObjectColor_.color_ <= soldierWorldPosPointer_->GetColorValue_UINT()))
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

				if (SoldierTopState::Firing == top_
					|| SoldierTopState::ThrowingGrenade == top_)
				{
					leg_ = SoldierLegState::Standing;
				}
				else
				{
					leg_ = SoldierLegState::RunningOrJumpingToStanding;
				}

				if (AimingDirection::Downward == direction_
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
				{
					direction_ = AimingDirection::Forward;
				}

				if (SoldierTopState::FiringToAiming == top_
					|| SoldierTopState::ThrowingGrenadeToAiming == top_)
				{
					top_ = SoldierTopState::Aiming;
				}
			}
		}
		else if (steppableObjectColor_.color_ <= soldierWorldPosPointer_->GetColorValue_UINT()
			&& steppableObjectColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (SoldierTopState::Firing == top_
					|| SoldierTopState::ThrowingGrenade == top_)
				{
					leg_ = SoldierLegState::Standing;
				}
				else
				{
					leg_ = SoldierLegState::RunningOrJumpingToStanding;
				}

				if (AimingDirection::Downward == direction_
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
				{
					direction_ = AimingDirection::Forward;
				}

				if (SoldierTopState::FiringToAiming == top_
					|| SoldierTopState::ThrowingGrenadeToAiming == top_)
				{
					top_ = SoldierTopState::Aiming;
				}
			}
		}
		else if (steppableObjectColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (SoldierTopState::Firing == top_
					|| SoldierTopState::ThrowingGrenade == top_)
				{
					leg_ = SoldierLegState::Standing;
				}
				else
				{
					leg_ = SoldierLegState::RunningOrJumpingToStanding;
				}

				if (AimingDirection::Downward == direction_
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
				{
					direction_ = AimingDirection::Forward;
				}

				if (SoldierTopState::FiringToAiming == top_
					|| SoldierTopState::ThrowingGrenadeToAiming == top_)
				{
					top_ = SoldierTopState::Aiming;
				}
			}
		}
		else
		{
			if (false == isAirborne_)
			{
				leg_ = SoldierLegState::Falling;
				isAirborne_ = true;
			}
		}
	}
}

void Soldier::Fall(float _deltaTime)
{
	fallingSpeed_ += gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;

	movementFor1Second_ += float4::Right * horizontalInputValue_ * runningSpeed_;
}

void Soldier::UpdateInputInfo()
{
	if (true == GameEngineInput::GetInst()->IsPressed("Right"))
	{
		horizontalInputValue_ = 1;
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("Left"))
	{
		horizontalInputValue_ = -1;
	}
	else
	{
		horizontalInputValue_ = 0;
	}

	if (true == GameEngineInput::GetInst()->IsPressed("Up"))
	{
		verticalInputValue_ = 1;
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("Down"))
	{
		verticalInputValue_ = -1;
	}
	else
	{
		verticalInputValue_ = 0;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Attack"))
	{
		isAttackKeyDown_ = true;
	}
	else
	{
		isAttackKeyDown_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Jump"))
	{
		isJumpKeyDown_ = true;
	}
	else
	{
		isJumpKeyDown_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Special"))
	{
		isSpecialKeyDown_ = true;
	}
	else
	{
		isSpecialKeyDown_ = false;
	}
}

void Soldier::ConvertInputToSoldierStates()
{
	//상하방향 입력 반응.
	switch (verticalInputValue_)
	{
	case -1:	//아랫방향 입력.
	{
		if (true == isAirborne_)
		{
			if (AimingDirection::ForwardToDownward != direction_ && AimingDirection::Downward != direction_)
			{
				if (SoldierTopState::Aiming == top_)
				{
					direction_ = AimingDirection::ForwardToDownward;
				}
				else if (SoldierTopState::Firing == top_)
				{
					if (weapon_ == SoldierWeaponType::HeavyMachineGun)
					{
						direction_ = AimingDirection::ForwardToDownward;
					}
					else
					{
						direction_ = AimingDirection::Downward;
					}
				}
			}
		}
		else
		{
			if (SoldierLegState::Standing == leg_
				|| SoldierLegState::Running == leg_
				|| SoldierLegState::RunningOrJumpingToStanding == leg_)
			{
				leg_ = SoldierLegState::StandingToDucking;
			}

			if (AimingDirection::Forward != direction_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
		break;
	}

	case 1:		//윗방향 입력.
	{
		if (AimingDirection::Upward != direction_ && AimingDirection::ForwardToUpward != direction_)
		{
			if (SoldierTopState::Aiming == top_)
			{
				if (SoldierLegState::Running == leg_ || SoldierLegState::Standing == leg_)
				{
					direction_ = AimingDirection::ForwardToUpward;
				}
			}
			else if (SoldierTopState::Firing == top_)
			{
				if (SoldierLegState::Ducking != leg_)
				{
					if (SoldierWeaponType::HeavyMachineGun == weapon_)
					{
						direction_ = AimingDirection::ForwardToUpward;
					}
					else
					{
						direction_ = AimingDirection::Upward;
					}
				}
			}
		}
		break;
	}

	case 0:		//중립.
	{
		if (AimingDirection::Upward == direction_)
		{
			if (SoldierTopState::Aiming == top_)
			{
				direction_ = AimingDirection::UpwardToForward;
			}
			else if (SoldierTopState::Firing == top_)
			{
				if (SoldierWeaponType::HeavyMachineGun == weapon_)
				{
					direction_ = AimingDirection::UpwardToForward;
				}
				else
				{
					direction_ = AimingDirection::Forward;
				}
			}
		}
		else if (AimingDirection::Downward == direction_)
		{
			if (SoldierTopState::Aiming == top_)
			{
				direction_ = AimingDirection::DownwardToForward;
			}
			else if (SoldierTopState::Firing == top_)
			{
				if (SoldierWeaponType::HeavyMachineGun == weapon_)
				{
					direction_ = AimingDirection::DownwardToForward;
				}
				else
				{
					direction_ = AimingDirection::Forward;
				}
			}
		}
		
		if (SoldierLegState::Ducking == leg_ || SoldierLegState::StandingToDucking == leg_)
		{
			leg_ = SoldierLegState::Standing;
			if (SoldierTopState::DuckStepping == top_
				|| SoldierTopState::FiringToAiming == top_
				|| SoldierTopState::ThrowingGrenadeToAiming == top_)
			{
				top_ = SoldierTopState::Aiming;
			}
		}

		break;
	}

	default:
		MsgBoxAssert("불가능한 입력 방향입니다.");
		return;
	}


	//좌우방향 입력 반응.
	switch (horizontalInputValue_)
	{

	case -1:	//좌측 입력.
	{
		this->GetTransform().PixLocalNegativeX();
		//플레이어 로컬포지션 전환.

		if (SoldierLegState::Ducking == leg_ || SoldierLegState::StandingToDucking == leg_)
		{
			leg_ = SoldierLegState::Ducking;
			if ((SoldierTopState::Firing != top_ && SoldierTopState::ThrowingGrenade != top_))
			{
				top_ = SoldierTopState::DuckStepping;
			}
		}
		else if (true == isAirborne_
			&& (SoldierLegState::VerticalJumping == leg_ || SoldierLegState::ForwardJumping == leg_ || SoldierLegState::Falling == leg_))
		{
			//다리 스테이트 변화 없음.
		}
		else 
		{
			leg_ = SoldierLegState::Running;
		}
		break;
	}

	case 1:		//우측 입력.
	{
		this->GetTransform().PixLocalPositiveX();
		//플레이어 로컬포지션 전환.

		if (SoldierLegState::Ducking == leg_ || SoldierLegState::StandingToDucking == leg_)
		{
			leg_ = SoldierLegState::Ducking;
			if ((SoldierTopState::Firing != top_ && SoldierTopState::ThrowingGrenade != top_))
			{
				top_ = SoldierTopState::DuckStepping;
			}
		}
		else if (true == isAirborne_
			&& (SoldierLegState::VerticalJumping == leg_ || SoldierLegState::ForwardJumping == leg_ || SoldierLegState::Falling == leg_))
		{
			//다리 스테이트 변화 없음.
		}
		else
		{
			leg_ = SoldierLegState::Running;
		}
		break;
	}

	case 0:		//중립.
	{
		if (SoldierLegState::Ducking == leg_ && SoldierTopState::DuckStepping == top_)
		{
			top_ = SoldierTopState::Aiming;
		}
		else if (SoldierLegState::Running == leg_ )
		{
			if (0 == verticalInputValue_)
			{
				leg_ = SoldierLegState::RunningOrJumpingToStanding;
				direction_ = AimingDirection::Forward;
			}
			else if (1 == verticalInputValue_)
			{
				leg_ = SoldierLegState::Standing;
			}
		}
		break;
	}

	default:
		MsgBoxAssert("불가능한 입력 방향입니다.");
		return;
	}

	//점프키 입력 반응.
	if (true == isJumpKeyDown_)
	{
		if (false == isAirborne_)
		{
			if (0 == horizontalInputValue_)
			{
				leg_ = SoldierLegState::VerticalJumping;
			}
			else
			{
				leg_ = SoldierLegState::ForwardJumping;
			}

			if (SoldierTopState::DuckStepping == top_
				|| SoldierTopState::FiringToAiming ==top_
				|| SoldierTopState::ThrowingGrenadeToAiming ==top_)
			{
				top_ = SoldierTopState::Aiming;
			}

			if (SoldierTopState::Firing != top_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
		else
		{
			////착지시 스테이트 변화. 점프 텍스처 피봇 맞출때만 복원.
			//if (true == isJumping_)
			//{
			//	isJumping_ = false;
			//	if (PlayerLegState::VerticalJumping == leg_
			//		|| PlayerLegState::ForwardJumping == leg_
			//		|| PlayerLegState::Falling == leg_)
			//	{
			//		leg_ = PlayerLegState::JumpingToStanding;
			//
			//		if (AimingDirection::Downward == direction_ 
			//			|| AimingDirection::DownwardToForward == direction_)
			//		{
			//			direction_ = AimingDirection::Forward;
			//		}
			//	}
			//} 
		}
	}

	//공격키 입력 반응.
	if (true == isAttackKeyDown_)
	{
		if (SoldierTopState::ThrowingGrenade != top_)
		{
			if (true == soldierCloseCombatCollisionBody_->IsCollision(
				CollisionType::CT_AABB,
				CollisionBodyOrder::Rebel,
				CollisionType::CT_AABB, 
				nullptr)
			)
			{
				top_ = SoldierTopState::MeleeAttack;
				direction_ = AimingDirection::Forward;
			}
			else 
			{
				top_ = SoldierTopState::Firing;
			}
		}
	}
	
	//특수공격 키 입력 반응.
	if (true == isSpecialKeyDown_)
	{
		if (SoldierTopState::Firing != top_)
		{
			top_ = SoldierTopState::ThrowingGrenade;
			direction_ = AimingDirection::Forward;
		}
	}


	//공격중일때 다른 부위 스테이트 강제변경.
	if (SoldierTopState::Firing == top_ 
		|| SoldierTopState::ThrowingGrenade == top_ 
		|| SoldierTopState::MeleeAttack == top_)
	{
		if (SoldierLegState::RunningOrJumpingToStanding == leg_)
		{
			leg_ = SoldierLegState::Standing;
		}
		else if (SoldierLegState::StandingToDucking == leg_)
		{
			leg_ = SoldierLegState::Ducking;
		}

		if (SoldierTopState::Firing == top_ && SoldierWeaponType::HeavyMachineGun != weapon_)
		{
			if (AimingDirection::ForwardToDownward == direction_)
			{
				direction_ = AimingDirection::Downward;
			}
			else if (AimingDirection::DownwardToForward == direction_)
			{
				direction_ = AimingDirection::Forward;
			}
			else if (AimingDirection::ForwardToUpward == direction_)
			{
				direction_ = AimingDirection::Upward;
			}
			else if (AimingDirection::UpwardToForward == direction_)
			{
				direction_ = AimingDirection::Forward;
			}
		}

		if (SoldierTopState::ThrowingGrenade == top_ || SoldierTopState::MeleeAttack == top_)
		{
			direction_ = AimingDirection::Forward;
		}
	}

	//중간동작들끼리 겹칠때 대응하는 코드.
	if (SoldierLegState::RunningOrJumpingToStanding == leg_
		|| SoldierLegState::StandingToDucking == leg_)
	{
		if (SoldierTopState::FiringToAiming == top_ || SoldierTopState::ThrowingGrenadeToAiming == top_)
		{
			top_ = SoldierTopState::Aiming;
		}

		direction_ = AimingDirection::Forward;		
	}

	//아래방향 조준은 공중에서만.
	if (false == isAirborne_ && (AimingDirection::Downward == direction_
		|| AimingDirection::DownwardToForward == direction_
		|| AimingDirection::ForwardToDownward == direction_))
	{
		direction_ = AimingDirection::Forward;
	}
	
}

void Soldier::UpdateSoldierState(float _deltaTime)
{
	int intNewState = 0;

	if (0 != causeOfDeath_)
	{
		intNewState = 1000 + causeOfDeath_;
	}
	else
	{
		intNewState
			= static_cast<int>(weapon_)
			+ static_cast<int>(leg_)
			+ static_cast<int>(top_)
			+ static_cast<int>(direction_);
	}
	
	if (true == isDamageProof_)
	{
		if (0.f >= remainedDamageProofDuration_)
		{
			isDamageProof_ = false;
			remainedDamageProofDuration_ = 0.f;
		}
		else if (0.f < remainedDamageProofDuration_)
		{
			remainedDamageProofDuration_ -= _deltaTime;
		}
	}

	if (allSoldierStates_.end() == allSoldierStates_.find(intNewState))
	{
		MsgBoxAssertString(std::to_string(intNewState) + ": 존재하지 않는 플레이어 스테이트입니다.");
		return;
	}

	if (allSoldierStates_[intNewState].first != currentSoldierState_)
	{
		currentSoldierState_ = allSoldierStates_[intNewState].first;
		soldierStateManager_.ChangeState(allSoldierStates_[intNewState].second);
	}

	soldierStateManager_.Update(_deltaTime);
}

void Soldier::ControlMuzzle()
{
	switch (direction_)
	{
	case AimingDirection::Forward:
	{
		aimingAngle_ = 90 - (90 * this->GetTransform().GetLocalScale().x);

		if (SoldierLegState::Ducking == leg_)
		{
			muzzleIndicator_->GetTransform().SetLocalPosition(pistolDuckingMuzzlePosition_);
		}
		else
		{
			muzzleIndicator_->GetTransform().SetLocalPosition(pistolForwardMuzzlePosition_);
		}
		break;
	}
	case AimingDirection::Upward:
	{
		aimingAngle_ = 90;
		muzzleIndicator_->GetTransform().SetLocalPosition(pistolUpwardMuzzlePosition_);
		break;
	}
	case AimingDirection::Downward:
	{
		aimingAngle_ = -90;
		muzzleIndicator_->GetTransform().SetLocalPosition(pistolDownwardMuzzlePosition_);
		break;
	}
	case AimingDirection::ForwardToUpward:
		break;
	case AimingDirection::UpwardToForward:
		break;
	case AimingDirection::ForwardToDownward:
		break;
	case AimingDirection::DownwardToForward:
		break;
	default:
		break;
	}

	muzzleIndicator_->GetTransform().SetLocalRotation(0.f, 0.f, aimingAngle_);

}

void Soldier::MoveSoldier(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void Soldier::Run()
{
	if (this->GetTransform().GetWorldPosition().x - 50.f/*캐릭터 좌우크기 절반*/
			< (- GameEngineWindow::GetInst()->GetScale().HX() 
				+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& -1 == horizontalInputValue_)
	{
		return;		//플레이어는 윈도우 좌측경계를 넘어갈 수 없음.
	}

	if (this->GetTransform().GetWorldPosition().x + 50.f/*캐릭터 좌우크기 절반*/
			> (GameEngineWindow::GetInst()->GetScale().HX()
				+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& 1 == horizontalInputValue_)
	{
		return;		//플레이어는 윈도우 우측경계를 넘어갈 수 없음.
	}

	movementFor1Second_ += float4::Right * horizontalInputValue_ * runningSpeed_;

	movementFor1Second_ += float4::Up * GetSlope() * runningSpeed_;
}

void Soldier::DuckStep()
{
	if (this->GetTransform().GetWorldPosition().x - 50.f/*캐릭터 좌우크기 절반*/
		< (-GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& -1 == horizontalInputValue_)
	{
		return;		//플레이어는 윈도우 좌측경계를 넘어갈 수 없음.
	}

	if (this->GetTransform().GetWorldPosition().x + 50.f/*캐릭터 좌우크기 절반*/
		> (GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& 1 == horizontalInputValue_)
	{
		return;		//플레이어는 윈도우 우측경계를 넘어갈 수 없음.
	}

	movementFor1Second_ += float4::Right * horizontalInputValue_ * duckStepSpeed_;

	movementFor1Second_ += float4::Up * GetSlope() * duckStepSpeed_;
}

float Soldier::GetSlope()
{
	if (false == isAirborne_)
	{
		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (steppableObjectColor_.color_ <= ascendingSlopeChecker_->GetColorValue_UINT()
			&& steppableObjectColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& steppableObjectColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_,
				ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5,
				-5
			);

			if (steppableObjectColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				return 0.f;
			}
			else
			{
				beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5;
				endPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			}
		}
		else if (steppableObjectColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& steppableObjectColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else if (steppableObjectColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
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
				slopeCheckerLocalPosX_,
				slopeCheckPosY,
				-5
			);
			if (steppableObjectColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				break;
			}
		}
		
		return static_cast<float>(slopeCheckPosY) / static_cast<float>(slopeCheckerLocalPosX_);
		//액터가 수평방향으로 1픽셀 전진했을때 수직으로는 몇픽셀 이동해야하는지에 대한 값 반환.
	}
	else
	{
#ifdef _DEBUG
		MsgBoxAssert("점프중일때는 의미 없는 기능입니다.");
#endif // DEBUG
		return 0.f;
	}
}

void Soldier::Fire()
{
	switch (weapon_)
	{
	case SoldierWeaponType::Pistol:
	{
		PistolBullet* newBullet 
			= this->GetLevel()->CreateActor<PistolBullet>(
				CollisionBodyOrder::Soldier_Projectile, "PistolBullet");

		newBullet->GetTransform().SetWorldPosition(muzzleIndicator_->GetTransform().GetWorldPosition());
		newBullet->GetTransform().SetWorldRotation(muzzleIndicator_->GetTransform().GetWorldRotation());
		newBullet->SetFiringDirection(aimingAngle_);
		break;
	}
	case SoldierWeaponType::HeavyMachineGun:
	{
		MsgBoxAssert("아직 준비되지 않은 무기 타입입니다.");
		break;
	}

	default:
		MsgBoxAssert("아직 준비되지 않은 무기 타입입니다.");
		break;
	}
}

void Soldier::MeleeAttack()
{
	soldierCloseCombatCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodyOrder::Rebel,
		CollisionType::CT_AABB,
		[this](GameEngineCollision* _thisCollision, GameEngineCollision* _rebelCollision)->CollisionReturn
		{
			_rebelCollision->GetRoot<Rebel>()->TakeDamage(meleeAttackDamage_, _thisCollision, _rebelCollision);
			return CollisionReturn::Stop;
		}
	);
}

void Soldier::Flicker(
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
		for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
			iter != allTextureRenderers_.end(); iter++)
		{
			(*iter)->GetPixelData().plusColor_ = _plusColor;
		}
	}
	else
	{
		for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
			iter != allTextureRenderers_.end(); iter++)
		{
			(*iter)->GetPixelData().plusColor_ = _originalColor;
		}
	}
}
