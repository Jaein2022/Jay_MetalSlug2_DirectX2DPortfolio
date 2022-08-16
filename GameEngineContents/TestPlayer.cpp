#include "PreCompile.h"
#include "TestPlayer.h"



TestPlayer::TestPlayer()
	: legRenderer_(nullptr),
	topPistolRenderer_(nullptr),
	wholePistolRenderer_(nullptr),
	topWeaponRenderer_(nullptr),
	wholeWeaponRenderer_(nullptr),
	currentState_(PlayerState::Pistol_Standing_Aiming_Forward),
	weapon_(PlayerWeaponType::Pistol),
	leg_(PlayerLegState::Standing),
	top_(PlayerTopState::Aiming),
	direction_(AimingDirection::Forward),
	movingDirection_(0),
	isJumping_(false),
	isUpKeyPressed_(false),
	isDownKeyPressed_(false),
	isJumpKeyDown_(false),
	isAttackKeyDowned_(false),
	isSpecialKeyDowned_(false),
	playerSpeed_(100.f),
	bulletCount_(-1),
	grenadeCount_(0)
{
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Start()
{

	this->GetTransform().SetWorldPosition(0, 0, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	if (false == GameEngineInput::GetInst()->IsKey("Left"))
	{
		GameEngineInput::GetInst()->CreateKey("Left", 'J');
		GameEngineInput::GetInst()->CreateKey("Right", 'L');
		GameEngineInput::GetInst()->CreateKey("Up", 'I');
		GameEngineInput::GetInst()->CreateKey("Down", 'K');

		GameEngineInput::GetInst()->CreateKey("Attack", 'X');
		GameEngineInput::GetInst()->CreateKey("Jump", 'Z');
		GameEngineInput::GetInst()->CreateKey("Special", 'S');
		GameEngineInput::GetInst()->CreateKey("Test", 'A');
	}

	CreatePlayerAnimations();
	CreatePlayerStates();


}

void TestPlayer::Update(float _deltaTime)
{
	UpdateInputInfo();
	ConvertInputToPlayerStates();

	UpdatePlayerState(_deltaTime);
	
}

void TestPlayer::End()
{
}

void TestPlayer::UpdateInputInfo()
{
	if (true == GameEngineInput::GetInst()->IsPressed("Right"))
	{
		movingDirection_ = 1;
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("Left"))
	{
		movingDirection_ = -1;
	}
	else
	{
		movingDirection_ = 0;
	}

	if (true == GameEngineInput::GetInst()->IsPressed("Up"))
	{
		isUpKeyPressed_ = true;
	}
	else 
	{
		isUpKeyPressed_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsPressed("Down"))
	{
		isDownKeyPressed_ = true;
	}
	else 
	{
		isDownKeyPressed_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Attack"))
	{
		isAttackKeyDowned_ = true;
	}
	else
	{
		isAttackKeyDowned_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Jump"))
	{
		isJumpKeyDown_ = true;
	}
	else
	{
		//점프체계 완성되면 제거.
		isJumpKeyDown_ = false;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Special"))
	{
		isSpecialKeyDowned_ = true;
	}
	else
	{
		isSpecialKeyDowned_ = false;
	}
}

void TestPlayer::ConvertInputToPlayerStates()
{
	if (true == isJumpKeyDown_)
	{
		if (false == isJumping_)
		{
			isJumping_ = true;
			if (0 == movingDirection_)
			{
				leg_ = PlayerLegState::VerticalJumping;
			}
			else
			{
				leg_ = PlayerLegState::ForwardJumping;
			}

			if (PlayerTopState::DuckStepping == top_)
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
			//착지시 스테이트 변화. 점프/픽셀충돌 완성되면 이전 혹은 삭제.
			if (true == isJumping_)
			{
				isJumping_ = false;
				if (PlayerLegState::VerticalJumping == leg_
					|| PlayerLegState::ForwardJumping == leg_)
				{
					leg_ = PlayerLegState::JumpingToStanding;
					if (AimingDirection::Downward == direction_)
					{
						direction_ = AimingDirection::Forward;
					}
				}
			}
		}
	}

	if (true == isDownKeyPressed_)
	{
		if (true == isJumping_ 
			&& (AimingDirection::ForwardToDownward != direction_ && AimingDirection::Downward != direction_))
		{
			if (top_ == PlayerTopState::Aiming 
				|| weapon_ == PlayerWeaponType::HeavyMachineGun && top_ == PlayerTopState::Firing)
			{
				direction_ = AimingDirection::ForwardToDownward;
			}
			else
			{
				direction_ = AimingDirection::Downward;
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
	}
	else 
	{
		if (true == isJumping_ && AimingDirection::Downward == direction_)
		{
			if (top_ == PlayerTopState::Aiming 
				|| weapon_ == PlayerWeaponType::HeavyMachineGun && top_ == PlayerTopState::Firing)
			{
				direction_ = AimingDirection::DownwardToForward;
			}
			else
			{
				direction_ = AimingDirection::Forward;
			}
		}
		else if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Standing;
			if (PlayerTopState::DuckStepping == top_)
			{
				top_ = PlayerTopState::Aiming;
			}
		}
	}
		
		
	if (true == isUpKeyPressed_)
	{
		if (PlayerLegState::Ducking != leg_)
		{
			if (AimingDirection::Upward != direction_)
			{

				if ( (PlayerTopState::Aiming == top_ 
					&& PlayerLegState::VerticalJumping != leg_ && PlayerLegState::ForwardJumping != leg_)
					|| (PlayerWeaponType::HeavyMachineGun == weapon_ && PlayerTopState::Firing == top_))
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
	else
	{
		if (AimingDirection::Upward == direction_)
		{
			if ((PlayerTopState::Aiming == top_
				&& PlayerLegState::VerticalJumping != leg_ && PlayerLegState::ForwardJumping != leg_)
				|| (PlayerWeaponType::HeavyMachineGun == weapon_ && PlayerTopState::Firing == top_))
			{
				direction_ = AimingDirection::UpwardToForward;
			}
			else
			{
				direction_ = AimingDirection::Forward;
			}
		}
	}

	switch (movingDirection_)
	{

	case -1:
	{
		if ( (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
			&& (PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_) )
		{
			leg_ = PlayerLegState::Ducking;
			top_ = PlayerTopState::DuckStepping;
		}
		else if (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_)
		{
			//다리 스테이트 변화 없음.
		}
		else 
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 1:
	{
		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
			if ((PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_))
			{
				top_ = PlayerTopState::DuckStepping;
			}
		}
		else if (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_)
		{
			//다리 스테이트 변화 없음.
		}
		else
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 0:
	{
		if (PlayerLegState::Ducking == leg_ && PlayerTopState::DuckStepping == top_)
		{
			top_ = PlayerTopState::Aiming;
		}
		else if (PlayerLegState::Running == leg_ )
		{
			leg_ = PlayerLegState::RunningToStanding;
		}
		break;
	}

	default:
		MsgBoxAssert("불가능한 이동 방향입니다.");
		return;
	}

	if (true == isAttackKeyDowned_)
	{
		if (PlayerTopState::ThrowingGrenade != top_)
		{
			top_ = PlayerTopState::Firing;
		}
	}
	
	if (true == isSpecialKeyDowned_)
	{
		if (PlayerTopState::Firing != top_)
		{
			top_ = PlayerTopState::ThrowingGrenade;
			direction_ = AimingDirection::Forward;
		}
	}









	if (PlayerTopState::Firing == top_ || PlayerTopState::ThrowingGrenade == top_)
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

		if (PlayerTopState::ThrowingGrenade == top_)
		{
			direction_ = AimingDirection::Forward;
		}
	}

}

void TestPlayer::UpdatePlayerState(float _deltaTime)
{
	int intCurrentState =
		static_cast<int>(weapon_)
		+ static_cast<int>(leg_)
		+ static_cast<int>(top_)
		+ static_cast<int>(direction_);


	if (allPlayerStates_.end() == allPlayerStates_.find(intCurrentState))
	{
		MsgBoxAssertString(std::to_string(intCurrentState) + ": 존재하지 않는 플레이어 스테이트입니다.");
		return;
	}

	currentState_ = allPlayerStates_.find(intCurrentState)->second.first;

	playerStateManager_.ChangeState(allPlayerStates_.find(intCurrentState)->second.second);


	playerStateManager_.Update(_deltaTime);

}
