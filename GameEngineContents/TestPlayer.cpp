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
	leg_(PlayerLegStatus::Standing),
	top_(PlayerTopStatus::Aiming),
	direction_(AimingDirection::Forward),
	isLanded_(true),
	isRight_(true),
	runningSpeed_(100.f),
	ducksteppingSpeed_(50.f),
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
	UpdateInputInfo(_deltaTime);

	UpdatePlayerState();
	playerStateManager_.Update(_deltaTime);
}

void TestPlayer::End()
{
}

void TestPlayer::UpdateInputInfo(float _deltaTime)
{



	//좌우방향키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsPressed("Right"))
	{
		if (PlayerLegStatus::Standing == leg_)
		{
			leg_ = PlayerLegStatus::Running;
		}
		else if (PlayerLegStatus::Ducking == leg_ && PlayerTopStatus::Firing != top_)
		{
			top_ = PlayerTopStatus::DuckStepping;
		}

		
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("Left"))
	{
		if (PlayerLegStatus::Standing == leg_)
		{
			leg_ = PlayerLegStatus::Running;
		}
		else if (PlayerLegStatus::Ducking == leg_ && PlayerTopStatus::Firing != top_)
		{
			top_ = PlayerTopStatus::DuckStepping;
		}

	}
	else
	{
		if (PlayerLegStatus::Running == leg_)
		{
			leg_ = PlayerLegStatus::Standing;
		}
		else if (PlayerLegStatus::Ducking == leg_ && PlayerTopStatus::DuckStepping == top_)
		{
			top_ = PlayerTopStatus::Aiming;
		}
	}

	//윗방향 키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsPressed("Up"))
	{
		if (PlayerLegStatus::Running == leg_ || PlayerLegStatus::Standing == leg_)
		{
			direction_ = AimingDirection::Upward;
		}
		else 
		{
			if (PlayerTopStatus::Firing == top_)
			{
				direction_ = AimingDirection::Upward;
			}
			else
			{
				direction_ = AimingDirection::Forward;
			}
		}


	}
	else 
	{
		direction_ = AimingDirection::Forward;
	}


	//아랫방향 키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsPressed("Down"))
	{
		if (PlayerLegStatus::Standing == leg_)
		{
			leg_ = PlayerLegStatus::Ducking;
		}

		if (PlayerLegStatus::VerticalJumping == leg_ || PlayerLegStatus::ForwardJumping == leg_)
		{
			direction_ = AimingDirection::Downward;
		}
	}
	else 
	{
		if (PlayerLegStatus::Ducking == leg_)
		{
			leg_ = PlayerLegStatus::Standing;
			if (PlayerTopStatus::DuckStepping == top_)
			{
				top_ = PlayerTopStatus::Aiming;
			}
		}
	}

	//공격키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsPressed("Attack"))
	{
		top_ = PlayerTopStatus::Firing;
	}
	else 
	{
		if (PlayerTopStatus::DuckStepping != top_)
		{
			top_ = PlayerTopStatus::Aiming;
		}
	}


	//점프키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsPressed("Jump"))
	{
		if (PlayerLegStatus::Standing == leg_ || PlayerLegStatus::Ducking == leg_)
		{
			if (true == GameEngineInput::GetInst()->IsPressed("Right"))
			{
				leg_ = PlayerLegStatus::ForwardJumping;
			}
			else if (true == GameEngineInput::GetInst()->IsPressed("Left"))
			{
				leg_ = PlayerLegStatus::ForwardJumping;
			}
			else
			{
				leg_ = PlayerLegStatus::VerticalJumping;
			}
		}
	}
	//임시조치. 픽셀충돌 완정되면 지울 것.
	else
	{
		if (PlayerLegStatus::VerticalJumping == leg_ || PlayerLegStatus::ForwardJumping == leg_)
		{
			leg_ = PlayerLegStatus::Standing;
		}
	}


	//특수공격 키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsDown("Special"))
	{
		top_ = PlayerTopStatus::ThrowingGrenade;
	}
}

void TestPlayer::UpdatePlayerState()
{
	int intCurrentState =
		static_cast<int>(weapon_)
		+ static_cast<int>(leg_)
		+ static_cast<int>(top_)
		+ static_cast<int>(direction_);

	currentState_ = allPlayerStates_.find(intCurrentState)->second.first;

	playerStateManager_.ChangeState(allPlayerStates_.find(intCurrentState)->second.second);


	if (true == topPistolRenderer_->IsUpdate() && true == wholePistolRenderer_->IsUpdate()
		|| true == topPistolRenderer_->IsUpdate() && true == topWeaponRenderer_->IsUpdate()
		//|| true == wholeWeaponRenderer_->IsUpdate() && true == topWeaponRenderer_->IsUpdate()
		//|| true == wholeWeaponRenderer_->IsUpdate() && true == wholePistolRenderer_->IsUpdate()
	)
	{
		MsgBoxAssert("성립할 수 없는 렌더링 상태입니다.");
		return;
	}



	//if (PlayerLegStatus::Running == leg_)
	//{
	//	wholePistolRenderer_->Off();
	//	legRenderer_->On();
	//	legRenderer_->ChangeFrameAnimation("Running");
	//	topPistolRenderer_->On();
	//	topPistolRenderer_->ChangeFrameAnimation("Running_Aiming_Forward");
	//}
	//else if (PlayerLegStatus::Ducking == leg_)
	//{
	//	wholePistolRenderer_->On();
	//	legRenderer_->Off();
	//	topPistolRenderer_->Off();
	//	if (PlayerTopStatus::DuckStepping == top_)
	//	{
	//		wholePistolRenderer_->ChangeFrameAnimation("Ducking_Ducksteping_Forward");
	//	}
	//	else
	//	{
	//		wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming_Forward");
	//	}
	//}
	//else
	//{
	//	wholePistolRenderer_->Off();
	//	legRenderer_->On();
	//	legRenderer_->ChangeFrameAnimation("Standing");
	//	topPistolRenderer_->On();
	//	topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
	//}

}
