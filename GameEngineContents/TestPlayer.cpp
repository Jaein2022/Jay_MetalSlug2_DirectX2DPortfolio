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

	legRenderer_ = CreateComponent<GameEngineTextureRenderer>("RegRenderer");
	legRenderer_->GetTransform().SetLocalScale(300, 300, 1);



	if (0 == GameEngineTexture::Find("Tarma_Leg.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Leg.png", 6, 5);
	}
	legRenderer_->SetTexture("Tarma_Leg.png");

	legRenderer_->CreateFrameAnimation_CutTexture("Standing",
		FrameAnimation_Desc("Tarma_Leg.png", 0, 0, 0.5f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Tarma_Leg.png", 6, 17, 0.05f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 18, 23, 0.05f, false));
	legRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 24, 29, 0.05f, false));

	legRenderer_->SetPivot(PivotMode::Custom);
	legRenderer_->GetTransform().SetLocalPosition(0, 0, 10);
	legRenderer_->ChangeFrameAnimation("Running");



	topPistolRenderer_ = CreateComponent<GameEngineTextureRenderer>("TopPistolRenderer");
	topPistolRenderer_->GetTransform().SetLocalScale(300, 300, 1);

	
	if (0 == GameEngineTexture::Find("Tarma_Top_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Top_Pistol.png", 10, 18);
	}
	topPistolRenderer_->SetTexture("Tarma_Top_Pistol.png");	

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 0, 3, 0.2f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Running_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 10, 21, 0.05f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 30, 35, 0.05f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 40, 45, 0.05f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_ForwardToUpward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 50, 51, 0.05f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 60, 63, 0.05f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_ForwardToUpward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 70, 71, 0.05f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_ForwardToDownward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 80, 81, 0.05f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 90, 90, 0.05f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Firing_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 100, 105, 0.05f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 110, 113, 0.01f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Firing_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 120, 125, 0.01f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_FiringToAiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 130, 133, 0.01f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Firing_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 140, 145, 0.01f, true));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 150, 155, 0.01f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 160, 165, 0.01f, false));

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 170, 175, 0.01f, false));

	topPistolRenderer_->SetPivot(PivotMode::Center);
	topPistolRenderer_->GetTransform().SetLocalPosition(0, 0, 0);
	topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
}

void TestPlayer::Update(float _deltaTime)
{
	UpdateInputInfo(_deltaTime);

	UpdatePlayerState();
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

		if (PlayerTopStatus::Firing == top_)
		{
			direction_ = AimingDirection::Upward;
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
		}
	}

	//공격키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsDown("Attack"))
	{
		top_ = PlayerTopStatus::Firing;
	}
	else 
	{
		top_ = PlayerTopStatus::Aiming;
	}


	//점프키 입력 대응.
	if (true == GameEngineInput::GetInst()->IsDown("Jump"))
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

	if (PlayerLegStatus::Running == leg_)
	{
		legRenderer_->ChangeFrameAnimation("Running");
		topPistolRenderer_->ChangeFrameAnimation("Running_Aiming_Forward");

	}
	else
	{
		legRenderer_->ChangeFrameAnimation("Standing");
		topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
	}

}
