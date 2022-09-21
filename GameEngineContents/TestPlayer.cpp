#include "PreCompile.h"
#include "TestPlayer.h"
#include "TestLevel.h"
#include "TestBackground.h"
#include "TestIndicator.h"
#include "TestPixelIndicator.h"
#include "TestPistolBullet.h"
#include "TestArabian.h"

TestPlayer::TestPlayer()
	: currentPlayerState_(PlayerState::Pistol_Standing_Aiming_Forward),
	weapon_(PlayerWeaponType::Pistol),
	leg_(PlayerLegState::Standing),
	top_(PlayerTopState::Aiming),
	direction_(AimingDirection::Forward),
	isAirborne_(false),
	playerRendererLocalPosX_(0),
	playerRendererLocalPosY_(75),
	playerRendererLocalPosZ_(0),
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
	//renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	playerWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	slopeCheckerLocalPosX_(10),	//0 ����!	
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
	playerLifeCollisionBody_(nullptr),
	playerCloseCombatCollisionBody_(nullptr),
	playerLifeCollisionBodyScale_Standing_(80, 150, 10),
	playerLifeCollisionBodyPosition_Standing_(0, 75, 10),
	playerLifeCollisionBodyScale_Ducking_(100, 100, 10),
	playerLifeCollisionBodyPosition_Ducking_(0, 50, 10),
	meleeAttackDamage_(3),
	isMeleeAttack1_(true),
	causeOfDeath_(0),
	isDamageProof_(false),
	flickeringPeriod_(0.1f)
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
		200,
		0
	);

	if (0 == slopeCheckerLocalPosX_)
	{
		MsgBoxAssert("slopeCheckerLocalPosX_�� 0 ���� �� ��!");
		return;
	}


	//renderPivotPointer_ = TestIndicator::CreatePointer(
	//	"RenderPivotPointer",
	//	this,
	//	float4::Cyan,
	//	float4(playerRenderPivotX_, playerRenderPivotY_, -5),
	//	float4(5, 5, 1)
	//);

	upperLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	playerWorldPosPointer_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
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

	muzzleIndicator_ = TestIndicator::CreateIndicator<TestIndicator>(
		"MuzzleIndicator",
		this,
		float4::Red,
		pistolForwardMuzzlePosition_,
		float4(5, 5, 1)
	);

	playerLifeCollisionBody_ = CreateComponent<GameEngineCollision>("PlayerLifeCollisionBody");
	playerLifeCollisionBody_->ChangeOrder(this->GetOrder());
	playerLifeCollisionBody_->SetCollisionMode(CollisionMode::Multiple);
	playerLifeCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	playerLifeCollisionBody_->GetTransform().SetLocalScale(playerLifeCollisionBodyScale_Standing_);
	playerLifeCollisionBody_->GetTransform().SetLocalPosition(playerLifeCollisionBodyPosition_Standing_);

	playerCloseCombatCollisionBody_ = CreateComponent<GameEngineCollision>("PlayerCloseCombatCollisionBody");
	playerCloseCombatCollisionBody_->ChangeOrder(this->GetOrder() + 1);
	playerCloseCombatCollisionBody_->SetCollisionMode(CollisionMode::Multiple);
	playerCloseCombatCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(1.f, 0.f, 0.f, 0.5f));
	playerCloseCombatCollisionBody_->GetTransform().SetLocalScale(120, 160, 10);
	playerCloseCombatCollisionBody_->GetTransform().SetLocalPosition(60, 80, 10);

	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.



	CreatePlayerAnimations();
	CreatePlayerStates();
}

void TestPlayer::Update(float _deltaTime)
{
	CheckGround();

	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	if (PlayerState::Pistol_Redeploying != currentPlayerState_ && 0 == causeOfDeath_ )
	{
		UpdateInputInfo();
	}

	if (true == isDamageProof_)
	{
		Flicker(_deltaTime, float4(0.25f, 0.25f, 0.25f, 0.f));
	}

	ConvertInputToPlayerStates();
	UpdatePlayerState(_deltaTime);
	ControlMuzzle();
	MovePlayer(_deltaTime);


}

void TestPlayer::End()
{
}

void TestPlayer::TakeDamage(int _rebelWeaponType)
{
	if (true == isDamageProof_)
	{
		return;
	}

	causeOfDeath_ = _rebelWeaponType;
}

void TestPlayer::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((TestLevel::groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= playerWorldPosPointer_->GetColorValue_UINT()))
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

				if (PlayerTopState::Firing == top_
					|| PlayerTopState::ThrowingGrenade == top_)
				{
					leg_ = PlayerLegState::Standing;
				}
				else
				{
					leg_ = PlayerLegState::RunningOrJumpingToStanding;
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
		else if (TestLevel::groundColor_.color_ <= playerWorldPosPointer_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (PlayerTopState::Firing == top_
					|| PlayerTopState::ThrowingGrenade == top_)
				{
					leg_ = PlayerLegState::Standing;
				}
				else
				{
					leg_ = PlayerLegState::RunningOrJumpingToStanding;
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
		else if (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				if (PlayerTopState::Firing == top_
					|| PlayerTopState::ThrowingGrenade == top_)
				{
					leg_ = PlayerLegState::Standing;
				}
				else
				{
					leg_ = PlayerLegState::RunningOrJumpingToStanding;
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
			if (false == isAirborne_)
			{
				leg_ = PlayerLegState::Falling;
				isAirborne_ = true;
			}
		}
	}
}

void TestPlayer::Fall(float _deltaTime)
{
	fallingSpeed_ += TestLevel::gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;

	movementFor1Second_ += float4::Right * horizontalInputValue_ * runningSpeed_;
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
		//TestIndicator::RenderingOnOffSwitch();
		////�ε������͵鸸 Ư�� ī�޶� ���Ƴְ� �� ī�޶� �����״��ϴ� �ڵ�� �ٲ� �� ��.

		isDamageProof_ = !isDamageProof_;
	}
}

void TestPlayer::ConvertInputToPlayerStates()
{

	//���Ϲ��� �Է� ����.
	switch (verticalInputValue_)
	{
	case -1:	//�Ʒ����� �Է�.
	{
		if (true == isAirborne_)
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
				|| PlayerLegState::RunningOrJumpingToStanding == leg_)
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
		this->GetTransform().PixLocalNegativeX();
		//�÷��̾� ���������� ��ȯ.

		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
			if ((PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_))
			{
				top_ = PlayerTopState::DuckStepping;
			}
		}
		else if (true == isAirborne_
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_ || PlayerLegState::Falling == leg_))
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
		this->GetTransform().PixLocalPositiveX();
		//�÷��̾� ���������� ��ȯ.

		if (PlayerLegState::Ducking == leg_ || PlayerLegState::StandingToDucking == leg_)
		{
			leg_ = PlayerLegState::Ducking;
			if ((PlayerTopState::Firing != top_ && PlayerTopState::ThrowingGrenade != top_))
			{
				top_ = PlayerTopState::DuckStepping;
			}
		}
		else if (true == isAirborne_
			&& (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_ || PlayerLegState::Falling == leg_))
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
				leg_ = PlayerLegState::RunningOrJumpingToStanding;
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
		if (false == isAirborne_)
		{
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
			////������ ������Ʈ ��ȭ. ���� �ؽ�ó �Ǻ� ���⶧�� ����.
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

	//����Ű �Է� ����.
	if (true == isAttackKeyDown_)
	{
		if (PlayerTopState::ThrowingGrenade != top_)
		{
			if (true == playerCloseCombatCollisionBody_->IsCollision(
				CollisionType::CT_AABB,
				CollisionBodyOrder::Rebel,
				CollisionType::CT_AABB, 
				nullptr)
			)
			{
				top_ = PlayerTopState::MeleeAttack;
				direction_ = AimingDirection::Forward;
			}
			else 
			{
				top_ = PlayerTopState::Firing;
			}
		}
	}
	
	//Ư������ Ű �Է� ����.
	if (true == isSpecialKeyDown_)
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
		if (PlayerLegState::RunningOrJumpingToStanding == leg_)
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
	if (PlayerLegState::RunningOrJumpingToStanding == leg_
		|| PlayerLegState::StandingToDucking == leg_)
	{
		if (PlayerTopState::FiringToAiming == top_ || PlayerTopState::ThrowingGrenadeToAiming == top_)
		{
			top_ = PlayerTopState::Aiming;
		}

		direction_ = AimingDirection::Forward;		
	}

	//�Ʒ����� ������ ���߿�����.
	if (false == isAirborne_ && (AimingDirection::Downward == direction_
		|| AimingDirection::DownwardToForward == direction_
		|| AimingDirection::ForwardToDownward == direction_))
	{
		direction_ = AimingDirection::Forward;
	}
	
}

void TestPlayer::UpdatePlayerState(float _deltaTime)
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

	if (allPlayerStates_.end() == allPlayerStates_.find(intNewState))
	{
		MsgBoxAssertString(std::to_string(intNewState) + ": �������� �ʴ� �÷��̾� ������Ʈ�Դϴ�.");
		return;
	}

	if (allPlayerStates_.find(intNewState)->second.first != currentPlayerState_)
	{
		currentPlayerState_ = allPlayerStates_.find(intNewState)->second.first;
		playerStateManager_.ChangeState(allPlayerStates_.find(intNewState)->second.second);
	}

	playerStateManager_.Update(_deltaTime);
}

void TestPlayer::ControlMuzzle()
{
	switch (direction_)
	{
	case AimingDirection::Forward:
	{
		aimingAngle_ = 90 - (90 * this->GetTransform().GetLocalScale().x);

		if (PlayerLegState::Ducking == leg_)
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

void TestPlayer::MovePlayer(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * TestLevel::playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void TestPlayer::Run()
{
	if (this->GetTransform().GetWorldPosition().x - 50.f/*ĳ���� �¿�ũ�� ����*/
			< (- GameEngineWindow::GetInst()->GetScale().HX() 
				+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& -1 == horizontalInputValue_)
	{
		return;		//�÷��̾�� ������ ������踦 �Ѿ �� ����.
	}

	if (this->GetTransform().GetWorldPosition().x + 50.f/*ĳ���� �¿�ũ�� ����*/
			> (GameEngineWindow::GetInst()->GetScale().HX()
				+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& 1 == horizontalInputValue_)
	{
		return;		//�÷��̾�� ������ ������踦 �Ѿ �� ����.
	}

	movementFor1Second_ += float4::Right * horizontalInputValue_ * runningSpeed_;

	movementFor1Second_ += float4::Up * GetSlope() * runningSpeed_;
}

void TestPlayer::DuckStep()
{
	if (this->GetTransform().GetWorldPosition().x - 50.f/*ĳ���� �¿�ũ�� ����*/
		< (-GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& -1 == horizontalInputValue_)
	{
		return;		//�÷��̾�� ������ ������踦 �Ѿ �� ����.
	}

	if (this->GetTransform().GetWorldPosition().x + 50.f/*ĳ���� �¿�ũ�� ����*/
		> (GameEngineWindow::GetInst()->GetScale().HX()
			+ this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x)
		&& 1 == horizontalInputValue_)
	{
		return;		//�÷��̾�� ������ ������踦 �Ѿ �� ����.
	}

	movementFor1Second_ += float4::Right * horizontalInputValue_ * duckStepSpeed_;

	movementFor1Second_ += float4::Up * GetSlope() * duckStepSpeed_;
}

float TestPlayer::GetSlope()
{
	if (false == isAirborne_)
	{
		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (TestLevel::groundColor_.color_ <= ascendingSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_,
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
				slopeCheckerLocalPosX_,
				slopeCheckPosY,
				-5
			);
			if (TestLevel::groundColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				break;
			}
		}
		
		return static_cast<float>(slopeCheckPosY) / static_cast<float>(slopeCheckerLocalPosX_);
		//���Ͱ� ����������� 1�ȼ� ���������� �������δ� ���ȼ� �̵��ؾ��ϴ����� ���� �� ��ȯ.
	}
	else
	{
#ifdef _DEBUG
		MsgBoxAssert("�������϶��� �ǹ� ���� ����Դϴ�.");
#endif // DEBUG
		return 0.f;
	}
}

void TestPlayer::Fire()
{
	switch (weapon_)
	{
	case PlayerWeaponType::Pistol:
	{
		TestPistolBullet* newBullet = this->GetLevel<TestLevel>()->GetPistolBullet();
		newBullet->GetTransform().SetWorldPosition(muzzleIndicator_->GetTransform().GetWorldPosition());
		newBullet->GetTransform().SetWorldRotation(muzzleIndicator_->GetTransform().GetWorldRotation());
		newBullet->SetFiringDirection(aimingAngle_);
		break;
	}
	case PlayerWeaponType::HeavyMachineGun:
	{
		MsgBoxAssert("���� �غ���� ���� ���� Ÿ���Դϴ�.");
		break;
	}

	default:
		MsgBoxAssert("���� �غ���� ���� ���� Ÿ���Դϴ�.");
		break;
	}
}

void TestPlayer::MeleeAttack()
{
	playerCloseCombatCollisionBody_->IsCollision(
		CollisionType::CT_AABB,
		CollisionBodyOrder::Rebel,
		CollisionType::CT_AABB,
		[this](GameEngineCollision* _thisCollision, GameEngineCollision* _rebelCollision)->CollisionReturn {
			_rebelCollision->GetActor<TestArabian>()->TakeDamage(meleeAttackDamage_);
			return CollisionReturn::Stop;
		}
	);
}

void TestPlayer::Flicker(
	float _deltaTime,
	const float4& _plusColor
)
{
	static float passedTime;
	static bool flickeringSwitch;
	
	if (flickeringPeriod_ <= passedTime)
	{
		passedTime = 0.f;

		if (true == flickeringSwitch)
		{
			for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
				iter != allTextureRenderers_.end(); iter++)
			{
				(*iter)->GetPixelData().plusColor_ = _plusColor;
			}
			flickeringSwitch = false;
		}
		else
		{
			for (std::list<GameEngineTextureRenderer*>::iterator iter = allTextureRenderers_.begin();
				iter != allTextureRenderers_.end(); iter++)
			{
				(*iter)->GetPixelData().plusColor_ = float4::Zero;
			}
			flickeringSwitch = true;
		}
	}
	else
	{
		passedTime += _deltaTime;
	}
}
