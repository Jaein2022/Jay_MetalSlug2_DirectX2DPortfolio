#include "PreCompile.h"
#include "TestPlayer.h"
#include "TestLevel.h"
#include "TestBackground.h"

TestPlayer::TestPlayer()
	: currentState_(PlayerState::Pistol_Standing_Aiming_Forward),
	weapon_(PlayerWeaponType::Pistol),
	leg_(PlayerLegState::Standing),
	top_(PlayerTopState::Aiming),
	direction_(AimingDirection::Forward),
	isJumping_(false),
	playerRenderPivotX_(2),
	playerRenderPivotY_(38),
	playerRenderPivotZ_(0),
	legRenderer_(nullptr),
	topPistolRenderer_(nullptr),
	wholePistolRenderer_(nullptr),
	topWeaponRenderer_(nullptr),
	wholeWeaponRenderer_(nullptr),
	horizontalInputValue_(0),
	verticalInputValue_(0),
	isJumpKeyDown_(false),
	isAttackKeyDown_(false),
	isSpecialKeyDown_(false),
	isTestKeyDown_(false),
	renderPivotPointer_(nullptr),
	upperMidfootPointer_(nullptr),
	playerWorldPosPointer_(nullptr),
	lowerMidfootPointer_(nullptr),
	frontPointer_(nullptr),
	initialJumpSpeed_(4.f),
	fallingSpeed_(0.f),
	runningSpeed_(100.f),
	bulletCount_(-1),
	grenadeCount_(0)
{
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Start()
{
	//플레이어 크기: 44, 78.
	this->GetTransform().SetWorldPosition(0, 0, 0);
	this->GetTransform().SetWorldScale(1, 1, 1);

	magenta_.r = 255;
	magenta_.g = 0;
	magenta_.b = 255;
	magenta_.a = 255;

	CreatePlayerAnimations();
	CreatePlayerStates();

	renderPivotPointer_ = CreateComponent<GameEngineCollision>("RenderPivotPointer");
	renderPivotPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Cyan);
	renderPivotPointer_->GetTransform().SetLocalScale(5, 5, 10);
	renderPivotPointer_->GetTransform().SetLocalPosition(
		playerRenderPivotX_,
		playerRenderPivotY_,
		playerRenderPivotZ_ - 5
	);

	upperMidfootPointer_ = CreateComponent<GameEngineCollision>("UpperMidfootPointer");
	upperMidfootPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Red);
	upperMidfootPointer_->GetTransform().SetLocalScale(5, 5, 10);
	upperMidfootPointer_->GetTransform().SetLocalPosition(0, 5, -5);
	playerWorldPosPointer_ = CreateComponent<GameEngineCollision>("PlayerWorldPosPointer");
	playerWorldPosPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Black);
	playerWorldPosPointer_->GetTransform().SetLocalScale(5, 5, 10);
	playerWorldPosPointer_->GetTransform().SetLocalPosition(0, 0, -5);
	lowerMidfootPointer_ = CreateComponent<GameEngineCollision>("LowerMidfootPointer");
	lowerMidfootPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Red);
	lowerMidfootPointer_->GetTransform().SetLocalScale(5, 5, 10);
	lowerMidfootPointer_->GetTransform().SetLocalPosition(0, -5, -5);

	frontPointer_ = CreateComponent<GameEngineCollision>("FrontPointer1");
	frontPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Red);
	frontPointer_->GetTransform().SetLocalScale(5, 5, 10);
	frontPointer_->GetTransform().SetLocalPosition(22, 0, -5);	






}

void TestPlayer::Update(float _deltaTime)
{
	UpdateInputInfo();
	ConvertInputToPlayerStates();


	FallAndLand(_deltaTime);


	UpdatePlayerState(_deltaTime);
	

	GameEngineDebug::DrawBox(renderPivotPointer_->GetTransform(), float4::Cyan);

	GameEngineDebug::DrawBox(upperMidfootPointer_->GetTransform(), float4::Red);
	GameEngineDebug::DrawBox(playerWorldPosPointer_->GetTransform(), float4::Black);
	GameEngineDebug::DrawBox(lowerMidfootPointer_->GetTransform(), float4::Red);

	GameEngineDebug::DrawBox(frontPointer_->GetTransform(), float4::Red);

}

void TestPlayer::End()
{
}

void TestPlayer::UpdateInputInfo()
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

	if (true == GameEngineInput::GetInst()->IsDown("Test"))
	{
		isTestKeyDown_ = true;
	}
	else
	{
		isTestKeyDown_ = false;
	}
}

void TestPlayer::ConvertInputToPlayerStates()
{
	//1922


	//상하방향 입력 반응.
	switch (verticalInputValue_)
	{
	case -1:	//아랫방향 입력.
	{
		if (true == isJumping_)
		{
			if (AimingDirection::ForwardToDownward != direction_ && AimingDirection::Downward != direction_)
			{
				if (PlayerTopState::Aiming == top_)
				{
					direction_ = AimingDirection::ForwardToDownward;
				}
				else if (PlayerTopState::Firing == top_)
				{
					if (weapon_ == PlayerWeaponType::HeavyMachineGun)
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
			if (PlayerLegState::Standing == leg_
				|| PlayerLegState::Running == leg_
				|| PlayerLegState::RunningToStanding == leg_
				|| PlayerLegState::JumpingToStanding == leg_)
			{
				leg_ = PlayerLegState::StandingToDucking;
			}
		}
		break;
	}

	case 1:		//윗방향 입력.
	{
		if (AimingDirection::Upward != direction_ && AimingDirection::ForwardToUpward != direction_)
		{
			if (PlayerTopState::Aiming == top_)
			{
				if (PlayerLegState::Running == leg_ || PlayerLegState::Standing == leg_)
				{
					direction_ = AimingDirection::ForwardToUpward;
				}
			}
			else if (PlayerTopState::Firing == top_)
			{
				if (PlayerLegState::Ducking != leg_)
				{
					if (PlayerWeaponType::HeavyMachineGun == weapon_)
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
			if (PlayerTopState::Aiming == top_)
			{
				direction_ = AimingDirection::UpwardToForward;
			}
			else if (PlayerTopState::Firing == top_)
			{
				if (PlayerWeaponType::HeavyMachineGun == weapon_)
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
			if (PlayerTopState::Aiming == top_)
			{
				direction_ = AimingDirection::DownwardToForward;
			}
			else if (PlayerTopState::Firing == top_)
			{
				if (PlayerWeaponType::HeavyMachineGun == weapon_)
				{
					direction_ = AimingDirection::DownwardToForward;
				}
				else
				{
					direction_ = AimingDirection::Forward;
				}
			}
		}
		
		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Standing;
			if (PlayerTopState::DuckStepping == top_
				|| PlayerTopState::FiringToAiming == top_
				|| PlayerTopState::ThrowingGrenadeToAiming == top_)
			{
				top_ = PlayerTopState::Aiming;
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
		if ( (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
			&& (PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_) )
		{
			leg_ = PlayerLegState::Ducking;
			top_ = PlayerTopState::DuckStepping;
		}
		else if (true == isJumping_ 
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_))
		{
			//다리 스테이트 변화 없음.
		}
		else 
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 1:		//우측 입력.
	{
		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
			if ((PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_))
			{
				top_ = PlayerTopState::DuckStepping;
			}
		}
		else if (true == isJumping_
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_))
		{
			//다리 스테이트 변화 없음.
		}
		else
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 0:		//중립.
	{
		if (PlayerLegState::Ducking == leg_ && PlayerTopState::DuckStepping == top_)
		{
			top_ = PlayerTopState::Aiming;
		}
		else if (PlayerLegState::Running == leg_ )
		{
			if (0 == verticalInputValue_)
			{
				leg_ = PlayerLegState::RunningToStanding;
				direction_ = AimingDirection::Forward;
			}
			else if (1 == verticalInputValue_)
			{
				leg_ = PlayerLegState::Standing;
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
		if (false == isJumping_)
		{
			isJumping_ = true;
			fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.

			if (0 == horizontalInputValue_)
			{
				leg_ = PlayerLegState::VerticalJumping;
			}
			else
			{
				leg_ = PlayerLegState::ForwardJumping;
			}

			if (PlayerTopState::DuckStepping == top_
				|| PlayerTopState::FiringToAiming ==top_
				|| PlayerTopState::ThrowingGrenadeToAiming ==top_)
			{
				top_ = PlayerTopState::Aiming;
			}

			if (PlayerTopState::Firing != top_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
		else
		{
			////착지시 스테이트 변화. 점프&픽셀충돌 완성되면 이전 혹은 삭제.
			//if (true == isJumping_)
			//{
			//	isJumping_ = false;
			//	if (PlayerLegState::VerticalJumping == leg_
			//		|| PlayerLegState::ForwardJumping == leg_)
			//	{
			//		leg_ = PlayerLegState::JumpingToStanding;

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
		if (PlayerTopState::ThrowingGrenade != top_)
		{
			if (true /*근접공격 발동조건 설정은 여기에*/)
			{
				top_ = PlayerTopState::Firing;
			}
			else
			{
				top_ = PlayerTopState::MeleeAttack;
				direction_ = AimingDirection::Forward;
			}
		}
	}
	
	//특수공격 키 입력 반응.
	if (true == isSpecialKeyDown_)
	{
		if (PlayerTopState::Firing != top_)
		{
			top_ = PlayerTopState::ThrowingGrenade;
			direction_ = AimingDirection::Forward;
		}
	}


	//공격중일때 다른 부위 스테이트 강제변경.
	if (PlayerTopState::Firing == top_ 
		|| PlayerTopState::ThrowingGrenade == top_ 
		|| PlayerTopState::MeleeAttack == top_)
	{
		if (PlayerLegState::RunningToStanding == leg_ || PlayerLegState::JumpingToStanding == leg_)
		{
			leg_ = PlayerLegState::Standing;
		}
		else if (PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
		}

		if (PlayerTopState::Firing == top_ && PlayerWeaponType::HeavyMachineGun != weapon_)
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

		if (PlayerTopState::ThrowingGrenade == top_ || PlayerTopState::MeleeAttack == top_)
		{
			direction_ = AimingDirection::Forward;
		}
	}

	//중간동작들끼리 겹칠때 대응하는 코드.
	if (PlayerLegState::RunningToStanding == leg_
		|| PlayerLegState::JumpingToStanding == leg_
		|| PlayerLegState::StandingToDucking == leg_)
	{
		if (PlayerTopState::FiringToAiming == top_ || PlayerTopState::ThrowingGrenadeToAiming == top_)
		{
			top_ = PlayerTopState::Aiming;
			direction_ = AimingDirection::Forward;
		}
	}
}

void TestPlayer::UpdatePlayerState(float _deltaTime)
{
	int intNewState =
		static_cast<int>(weapon_)
		+ static_cast<int>(leg_)
		+ static_cast<int>(top_)
		+ static_cast<int>(direction_);


	if (allPlayerStates_.end() == allPlayerStates_.find(intNewState))
	{
		MsgBoxAssertString(std::to_string(intNewState) + ": 존재하지 않는 플레이어 스테이트입니다.");
		return;
	}

	if (allPlayerStates_.find(intNewState)->second.first != currentState_)
	{
		currentState_ = allPlayerStates_.find(intNewState)->second.first;
		playerStateManager_.ChangeState(allPlayerStates_.find(intNewState)->second.second);
	}

	playerStateManager_.Update(_deltaTime);
}

void TestPlayer::FallAndLand(float _deltaTime)
{
	if (true == isJumping_)
	{
		fallingSpeed_ += TestLevel::gravity_ * _deltaTime;
	}
	else
	{
		fallingSpeed_ = 0.f;

		//착지시 스테이트 변화.
		if (PlayerLegState::VerticalJumping == leg_
			|| PlayerLegState::ForwardJumping == leg_
			|| PlayerLegState::Falling == leg_)
		{
			if (PlayerTopState::Firing == top_
				|| PlayerTopState::ThrowingGrenade == top_)
			{
				leg_ = PlayerLegState::Standing;
			}
			else
			{
				leg_ = PlayerLegState::JumpingToStanding;
			}

			if (AimingDirection::Downward == direction_
				|| AimingDirection::DownwardToForward == direction_)
			{
				direction_ = AimingDirection::Forward;
			}

			if (PlayerTopState::FiringToAiming == top_ 
				|| PlayerTopState::ThrowingGrenadeToAiming == top_)
			{
				top_ = PlayerTopState::Aiming;
			}
		}
	}

	this->GetTransform().SetWorldMove(float4::Down * _deltaTime * fallingSpeed_ * TestLevel::playSpeed_);


	float4 upperMidfootPos 
		= float4(upperMidfootPointer_->GetTransform().GetWorldPosition().x, upperMidfootPointer_->GetTransform().GetWorldPosition().y);
	float4 playerWorldPos 
		= float4(playerWorldPosPointer_->GetTransform().GetWorldPosition().x, playerWorldPosPointer_->GetTransform().GetWorldPosition().y);
	float4 lowerMidfootPos 
		= float4(lowerMidfootPointer_->GetTransform().GetWorldPosition().x, lowerMidfootPointer_->GetTransform().GetWorldPosition().y);

	if (0 <= fallingSpeed_)
	{
		if (0 < fallingSpeed_ 
			&& (PlayerLegState::Standing == leg_
			|| PlayerLegState::Running == leg_
			|| PlayerLegState::Ducking == leg_))
		{
			leg_ = PlayerLegState::Falling;
		}

		if ((magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
			playerWorldPos.IX(), playerWorldPos.IY()).color_)
			&& (magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
				lowerMidfootPos.IX(), lowerMidfootPos.IY()).color_)
			&& (magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
				upperMidfootPos.IX(), upperMidfootPos.IY()).color_))
		{
			this->GetTransform().SetWorldMove(float4::Up * 5.0f);
			isJumping_ = false;
		}
		else if ((magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
			playerWorldPos.IX(), playerWorldPos.IY()).color_)
			&& (magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
				lowerMidfootPos.IX(), lowerMidfootPos.IY()).color_))
		{
			isJumping_ = false;
		}
		else if ((magenta_.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(
			lowerMidfootPos.IX(), lowerMidfootPos.IY()).color_))
		{
			this->GetTransform().SetWorldMove(float4::Down * 5.f);
			isJumping_ = false;
		}
		else
		{
			isJumping_ = true;
		}
	}
}
