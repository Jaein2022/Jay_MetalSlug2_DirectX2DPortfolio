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
	isAttackKeyDowned_(false),
	isSpecialKeyDowned_(false),
	isTestKeyDowned_(false),
	renderPivotPointer_(nullptr),
	playerWorldPosPointer_(nullptr),
	frontCollision_(nullptr),
	initialJumpSpeed_(2.75f),
	fallingSpeed_(0.f),
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
	//�÷��̾� ũ��: 44, 78.
	this->GetTransform().SetWorldPosition(0, 0, 0);
	this->GetTransform().SetWorldScale(1, 1, 1);

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

	playerWorldPosPointer_ = CreateComponent<GameEngineCollision>("PlayerWorldPosPointer");
	playerWorldPosPointer_->SetDebugSetting(CollisionType::CT_AABB, float4::Red);
	playerWorldPosPointer_->GetTransform().SetLocalScale(5, 5, 10);
	playerWorldPosPointer_->GetTransform().SetLocalPosition(0, 0, -5);

	frontCollision_ = CreateComponent<GameEngineCollision>("FrontCollision");
	frontCollision_->SetDebugSetting(CollisionType::CT_AABB, float4::Red);
	frontCollision_->GetTransform().SetLocalScale(5, 5, 10);
	frontCollision_->GetTransform().SetLocalPosition(22, 10, -5);





}

void TestPlayer::Update(float _deltaTime)
{
	UpdateInputInfo();
	ConvertInputToPlayerStates();




	UpdatePlayerState(_deltaTime);
	
	FallAndLand(_deltaTime);

	GameEngineDebug::DrawBox(renderPivotPointer_->GetTransform(), float4::Cyan);
	GameEngineDebug::DrawBox(playerWorldPosPointer_->GetTransform(), float4::Red);
	GameEngineDebug::DrawBox(frontCollision_->GetTransform(), float4::Red);
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

	if (true == GameEngineInput::GetInst()->IsDown("Test"))
	{
		isTestKeyDowned_ = true;
	}
	else
	{
		isTestKeyDowned_ = false;
	}
}

void TestPlayer::ConvertInputToPlayerStates()
{



	//���Ϲ��� �Է� ����.
	switch (verticalInputValue_)
	{
	case -1:	//�Ʒ����� �Է�.
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

	case 1:		//������ �Է�.
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

	case 0:		//�߸�.
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
		MsgBoxAssert("�Ұ����� �Է� �����Դϴ�.");
		return;
	}


	//�¿���� �Է� ����.
	switch (horizontalInputValue_)
	{

	case -1:	//���� �Է�.
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
			//�ٸ� ������Ʈ ��ȭ ����.
		}
		else 
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 1:		//���� �Է�.
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
			//�ٸ� ������Ʈ ��ȭ ����.
		}
		else
		{
			leg_ = PlayerLegState::Running;
		}
		break;
	}

	case 0:		//�߸�.
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
		MsgBoxAssert("�Ұ����� �Է� �����Դϴ�.");
		return;
	}

	//����Ű �Է� ����.
	if (true == isJumpKeyDown_)
	{
		if (false == isJumping_)
		{
			isJumping_ = true;
			fallingSpeed_ = -initialJumpSpeed_;		//���� ����.
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
			////������ ������Ʈ ��ȭ. ����&�ȼ��浹 �ϼ��Ǹ� ���� Ȥ�� ����.
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

	//����Ű �Է� ����.
	if (true == isAttackKeyDowned_)
	{
		if (PlayerTopState::ThrowingGrenade != top_)
		{
			if (true /*�������� �ߵ����� ������ ���⿡*/)
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
	
	//Ư������ Ű �Է� ����.
	if (true == isSpecialKeyDowned_)
	{
		if (PlayerTopState::Firing != top_)
		{
			top_ = PlayerTopState::ThrowingGrenade;
			direction_ = AimingDirection::Forward;
		}
	}


	//�������϶� �ٸ� ���� ������Ʈ ��������.
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

	//�߰����۵鳢�� ��ĥ�� �����ϴ� �ڵ�.
	if (PlayerLegState::RunningToStanding == leg_
		|| PlayerLegState::JumpingToStanding == leg_
		|| PlayerLegState::StandingToDucking == leg_)
	{
		if (PlayerTopState::FiringToAiming == top_ || PlayerTopState::ThrowingGrenadeToAiming == top_)
		{
			top_ = PlayerTopState::Aiming;
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
		MsgBoxAssertString(std::to_string(intNewState) + ": �������� �ʴ� �÷��̾� ������Ʈ�Դϴ�.");
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

		if (PlayerLegState::VerticalJumping == leg_
			|| PlayerLegState::ForwardJumping == leg_)
		{
			leg_ = PlayerLegState::JumpingToStanding;

			if (AimingDirection::Downward == direction_
				|| AimingDirection::DownwardToForward == direction_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
	}

	this->GetTransform().SetWorldMove(float4::Down * _deltaTime * fallingSpeed_ * TestLevel::playSpeed_);

	PixelColor magenta;
	magenta.r = 255;
	magenta.g = 0;
	magenta.b = 255;
	magenta.a = 255;

	float4 playerWorldPos = float4(this->GetTransform().GetWorldPosition().x, this->GetTransform().GetWorldPosition().y);


	if (magenta.color_ == reinterpret_cast<TestLevel*>(this->GetLevel())->GetPixelColor(playerWorldPos.IX(), playerWorldPos.IY()).color_)
	{
		isJumping_ = false;
	}


}
