#include "PreCompile.h"
#include "TestPlayer.h"

void TestPlayer::CreatePlayerAnimations()
{
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
	legRenderer_->Off();








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
	topPistolRenderer_->AnimationBindEnd("Standing, Running_Aiming_ForwardToUpward",
		[this](const FrameAnimation_Desc& _desc)->void {
			direction_ = AimingDirection::Upward;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Aiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 60, 63, 0.2f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_UpwardToForward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 70, 71, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running_Aiming_UpwardToForward",
		[this](const FrameAnimation_Desc& _desc)->void {
			direction_ = AimingDirection::Forward;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_ForwardToDownward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 80, 81, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Jumping_Aiming_ForwardToDownward",
		[this](const FrameAnimation_Desc& _desc)->void {
			direction_ = AimingDirection::Downward;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 90, 90, 0.05f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 100, 105, 0.05f, true));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 110, 113, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 120, 125, 0.05f, true));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 130, 133, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Firing_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 140, 145, 0.05f, true));
	topPistolRenderer_->AnimationBindEnd("Jumping_Firing_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 150, 155, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_ThrowingGrenade",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 160, 165, 0.05f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 170, 175, 0.05f, false));

	topPistolRenderer_->SetPivot(PivotMode::Center);
	topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
	topPistolRenderer_->Off();



	topWeaponRenderer_ = CreateComponent<GameEngineTextureRenderer>("TopWeaponRenderer");
	topWeaponRenderer_->GetTransform().SetLocalScale(300, 300, 1);

	if (0 == GameEngineTexture::Find("Tarma_Top_Weapon.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Top_Weapon.png", 10, 23);
	}
	topWeaponRenderer_->SetTexture("Tarma_Top_Weapon.png");

	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing_Aiming_Forward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 0, 3, 0.2f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Running_Aiming_Forward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 10, 21, 0.05f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping_Aiming_Forward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 30, 35, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping_Aiming_Forward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 40, 45, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_ForwardToUpward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 50, 51, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_Upward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 60, 63, 0.05f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running_Aiming_UpwardToForward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 70, 71, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_ForwardToDownward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 80, 81, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_Downward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 90, 90, 0.05f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Forward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 100, 103, 0.05f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_ForwardToUpward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 110, 111, 0.05f, false));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Upward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 120, 123, 0.05f, true));
	//topWeaponRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_UpwardToForward",
	//	FrameAnimation_Desc("Tarma_Top_Weapon.png", 130, 131, 0.05f, true));

	//topWeaponRenderer_->SetPivot(PivotMode::Center);
	//topWeaponRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
	//topWeaponRenderer_->Off();






	wholePistolRenderer_ = CreateComponent<GameEngineTextureRenderer>("WholePistolRenderer");
	wholePistolRenderer_->GetTransform().SetLocalScale(300, 300, 1);

	if (0 == GameEngineTexture::Find("Tarma_Whole_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Whole_Pistol.png", 10, 10);
	}
	wholePistolRenderer_->SetTexture("Tarma_Whole_Pistol.png");

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Running, JumpingToStanding",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 0, 3, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Running, JumpingToStanding",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = PlayerLegState::Standing;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("StandingToDucking",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 10, 12, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("StandingToDucking",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = PlayerLegState::Ducking;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming1_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 20, 23, 0.2f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Ducksteping_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 30, 36, 0.05f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Firing_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 40, 45, 0.05f, true));
	wholePistolRenderer_->AnimationBindEnd("Ducking_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 50, 53, 0.05f, true));
	wholePistolRenderer_->AnimationBindEnd("Ducking_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming2_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 60, 63, 0.05f, true));

	wholePistolRenderer_->SetPivot(PivotMode::Center);
	wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
	wholePistolRenderer_->Off();
}

void TestPlayer::CreatePlayerStates()
{
	playerStateManager_.CreateState(	
		"Dead",
		[this](float _deltaTime, const StateInfo& _info)->void {
			
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Aiming_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");

		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Aiming_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Aiming_Upward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Aiming_ForwardToUpward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_ForwardToUpward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Aiming_UpwardToForward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_UpwardToForward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Firing_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_Firing_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_FiringToAiming_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Standing_FiringToAiming_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);	
	
	playerStateManager_.CreateState(	
		"Pistol_Running_Aiming_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Running_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Running_Aiming_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Aiming_Upward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Running_Aiming_ForwardToUpward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_ForwardToUpward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Running_Aiming_UpwardToForward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_UpwardToForward");
		}
	);
 
	playerStateManager_.CreateState(
		"Pistol_Running_Firing_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);
		
	playerStateManager_.CreateState(	
		"Pistol_Running_Firing_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Running_FiringToAiming_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Running_FiringToAiming_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);


	playerStateManager_.CreateState(	
		"Pistol_Ducking_Aiming",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Ducking_Firing",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Ducking_DuckStepping",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Ducksteping_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_Ducking_FiringToAiming",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_FiringToAiming_Forward");
		}
	);

	//case PlayerState::Pistol_Ducking_ThrowingGrenade:



	playerStateManager_.CreateState(	
		"Pistol_VerticalJumping_Aiming_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("VerticalJumping_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(
		"Pistol_VerticalJumping_Aiming_ForwardToDownward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_ForwardToDownward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_VerticalJumping_Aiming_Downward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_Downward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_VerticalJumping_Firing_Forward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_VerticalJumping_Firing_Upward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_VerticalJumping_Firing_Downward",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Firing_Downward");
		}
	);	
	
	

	//case PlayerState::Pistol_ForwardJumping_Aiming_Forward:
	//	break;
	//case PlayerState::Pistol_ForwardJumping_Aiming_Downward:
	//	break;
	//case PlayerState::Pistol_ForwardJumping_Firing_Forward:
	//	break;
	//case PlayerState::Pistol_ForwardJumping_Firing_Upward:
	//	break;
	//case PlayerState::Pistol_ForwardJumping_Firing_Downward:
	//	break;



	playerStateManager_.CreateState(	
		"Pistol_StandingToDucking",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("StandingToDucking");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_RunningToStanding",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Running, JumpingToStanding");
		}
	);

	playerStateManager_.CreateState(	
		"Pistol_JumpingToStanding",
		[this](float _deltaTime, const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Running, JumpingToStanding");
		}
	);





	constexpr auto playerStateEntries = magic_enum::enum_entries<PlayerState>();
	const size_t playerStatesSize = playerStateEntries.size();

	for (size_t i = 0; i < playerStatesSize; i++)
	{
		allPlayerStates_.insert(
			std::make_pair(
				magic_enum::enum_integer<PlayerState>(playerStateEntries[i].first),
				std::make_pair(playerStateEntries[i].first, playerStateEntries[i].second.data())
			)
		);
	}

	
}
