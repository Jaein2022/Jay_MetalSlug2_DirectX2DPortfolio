#include "PreCompile.h"
#include "TestPlayer.h"
#include "TestLevel.h"
#include "TestBackground.h"
#include "TestPointer.h"

TestPlayer::TestPlayer()
	: currentState_(PlayerState::Pistol_Standing_Aiming_Forward),
	weapon_(PlayerWeaponType::Pistol),
	leg_(PlayerLegState::Standing),
	top_(PlayerTopState::Aiming),
	direction_(AimingDirection::Forward),
	isJumping_(false),
	playerRendererLocalPosX_(4),
	playerRendererLocalPosY_(76),
	playerRendererLocalPosZ_(0),
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
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	playerWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	slopeCheckerLocalPosX_Int_(10),		//두 숫자는 반드시 같아야 한다.
	slopeCheckerLocalPosX_Float_(10.f),	//두 숫자는 반드시 같아야 한다.
	slopeChecker_(nullptr),
	ascendingSlopeChecker_(nullptr),
	flatSlopeChecker_(nullptr),
	descendingSlopeChecker_(nullptr),
	magenta_(255, 0, 255, 255),
	initialJumpSpeed_(6.f),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	bulletCount_(-1),
	grenadeCount_(0)
{
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetWorldPosition(
		500 - GameEngineWindow::GetInst()->GetScale().HIX(),
		0,
		0
	);

	//magenta_.r = 255;
	//magenta_.g = 0;
	//magenta_.b = 255;
	//magenta_.a = 255;

	CreatePlayerAnimations();
	CreatePlayerStates();

	//renderPivotPointer_ = TestPointer::CreatePointer(
	//	"RenderPivotPointer",
	//	this,
	//	float4::Cyan,
	//	float4(playerRenderPivotX_, playerRenderPivotY_, -5),
	//	float4(5, 5, 1)
	//);

	upperLandingChecker_ = TestPointer::CreatePointer(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	playerWorldPosPointer_ = TestPointer::CreatePointer(
		"PlayerWorldPosPointer",
		this,
		float4::Red,
		float4(0, 0, -5),
		float4(5, 5, 1)
	);	
	
	lowerLandingChecker_ = TestPointer::CreatePointer(
		"LowerLandingChecker",
		this,
		float4::Black,
		float4(0, -5, -5),
		float4(5, 5, 1)
	);

	slopeChecker_ = TestPointer::CreatePointer(
		"SlopeChecker",
		this,
		float4::Red,
		float4(slopeCheckerLocalPosX_Int_, 0, -5),
		float4(5, 5, 1)
	);

	ascendingSlopeChecker_ = TestPointer::CreatePointer(
		"AscendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_Int_, 10, -5),
		float4(5, 5, 1)
	);

	flatSlopeChecker_ = TestPointer::CreatePointer(
		"FlatSlopeChecker_",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_Int_, 0, -5),
		float4(5, 5, 1)
	);

	descendingSlopeChecker_ = TestPointer::CreatePointer(
		"DescendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_Int_, -10, -5),
		float4(5, 5, 1)
	);





}

void TestPlayer::Update(float _deltaTime)
{
	CheckFalling();
	UpdateInputInfo();
	ConvertInputToPlayerStates();

	UpdatePlayerState(_deltaTime);

	if (0 <= this->GetTransform().GetWorldPosition().x)
	{
		float4 test2 = this->GetTransform().GetWorldPosition();
		float4 test = playerWorldPosPointer_->GetTransform().GetWorldPosition();
		int i = 0;
	}

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
		//TestPointer::RenderingOnOffSwitch();

		//this->GetTransform().PixLocalNegativeX();

	}
}

void TestPlayer::ConvertInputToPlayerStates()
{

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
		this->GetTransform().PixLocalNegativeX();
		//플레이어 로컬포지션 전환.

		if ( (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
			&& (PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_) )
		{
			leg_ = PlayerLegState::Ducking;
			top_ = PlayerTopState::DuckStepping;
		}
		else if (true == isJumping_ 
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_ || PlayerLegState::Falling == leg_))
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
		this->GetTransform().PixLocalPositiveX();
		//플레이어 로컬포지션 전환.

		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
			if ((PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_))
			{
				top_ = PlayerTopState::DuckStepping;
			}
		}
		else if (true == isJumping_
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_ || PlayerLegState::Falling == leg_))
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
			////착지시 스테이트 변화. 점프 텍스처 피벗 맞출때만 복원.
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

		if (AimingDirection::UpwardToForward == direction_)
		{
			direction_ = AimingDirection::Forward;
		}
	}

	//아래방향 조준은 공중에서만.
	if (false == isJumping_ && (AimingDirection::Downward == direction_
		|| AimingDirection::DownwardToForward == direction_
		|| AimingDirection::ForwardToDownward == direction_))
	{
		direction_ = AimingDirection::Forward;
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

void TestPlayer::Run(float _deltaTime)
{
	this->GetTransform().SetWorldMove(
		float4::Right * horizontalInputValue_ * _deltaTime * runningSpeed_ * TestLevel::playSpeed_);

	this->GetTransform().SetWorldMove(
		float4::Up * CheckSlope() * _deltaTime * runningSpeed_ * TestLevel::playSpeed_);
}

float TestPlayer::CheckSlope()
{
	if (false == isJumping_)
	{
		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (magenta_.color_ == ascendingSlopeChecker_->GetColorValue_UINT()
			&& magenta_.color_ == flatSlopeChecker_->GetColorValue_UINT()
			&& magenta_.color_ == descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_Int_,
				ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5,
				-5
			);

			if (magenta_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				//절벽.
				return 0.f;
			}
			else
			{
				beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5;
				endPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			}
		}
		else if (magenta_.color_ == flatSlopeChecker_->GetColorValue_UINT()
			&& magenta_.color_ == descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else if (magenta_.color_ == descendingSlopeChecker_->GetColorValue_UINT())
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
				slopeCheckerLocalPosX_Int_,
				slopeCheckPosY,
				-5
			);
			if (magenta_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				break;
			}
		}
		
		return static_cast<float>(slopeCheckPosY) / slopeCheckerLocalPosX_Float_;
		//플레이어가 앞으로 1픽셀 전진했을때 위로는 몇픽셀 이동해야하는지에 대한 값 반환.
	}
	else
	{
		MsgBoxAssert("점프중일때는 의미 없는 기능입니다.");
		return 0.f;
	}
}

void TestPlayer::Fall(float _deltaTime)
{
	fallingSpeed_ += TestLevel::gravity_ * _deltaTime;
	
	this->GetTransform().SetWorldMove(
		float4::Down * _deltaTime * fallingSpeed_ * TestLevel::playSpeed_);
	
	this->GetTransform().SetWorldMove(
		float4::Right * horizontalInputValue_ * _deltaTime * runningSpeed_ * TestLevel::playSpeed_);
}

void TestPlayer::CheckFalling()
{
	if (0 <= fallingSpeed_)
	{
		if ((magenta_.color_ == upperLandingChecker_->GetColorValue_UINT())
			&& (magenta_.color_ == lowerLandingChecker_->GetColorValue_UINT())
			&& (magenta_.color_ == playerWorldPosPointer_->GetColorValue_UINT()))
		{
			if (true == isJumping_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isJumping_ = false;
				fallingSpeed_ = 0.f;

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
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
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
		else if (magenta_.color_ == playerWorldPosPointer_->GetColorValue_UINT()
			&& magenta_.color_ == lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isJumping_)
			{
				isJumping_ = false;
				fallingSpeed_ = 0.f;

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
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
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
		else if (magenta_.color_ == lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isJumping_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isJumping_ = false;
				fallingSpeed_ = 0.f;

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
					|| AimingDirection::DownwardToForward == direction_
					|| AimingDirection::ForwardToDownward == direction_)
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
		else
		{
			if (false == isJumping_)
			{
				leg_ = PlayerLegState::Falling;
				isJumping_ = true;
			}
		}
	}
}
