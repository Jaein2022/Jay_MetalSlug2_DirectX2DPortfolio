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
		FrameAnimation_Desc("Tarma_Leg.png", 0, 0, 1.f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Tarma_Leg.png", 6, 17, 0.05f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 18, 23, 0.05f, false));
	legRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 24, 29, 0.05f, false));

	legRenderer_->SetPivot(PivotMode::Custom);
	legRenderer_->GetTransform().SetLocalPosition(0, 0, 10);
	legRenderer_->ChangeFrameAnimation("Standing");
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
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 80, 81, 0.05f, true));
	topPistolRenderer_->AnimationBindEnd("Jumping_Aiming_ForwardToDownward",
		[this](const FrameAnimation_Desc& _desc)->void {
			direction_ = AimingDirection::Downward;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 90, 90, 1.f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_DownwardToForward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 100, 101, 0.05f, true));
	topPistolRenderer_->AnimationBindEnd("Jumping_Aiming_DownwardToForward",
		[this](const FrameAnimation_Desc& _desc)->void {
			direction_ = AimingDirection::Forward;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 110, 113, 0.03f, true));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 114, 119, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 120, 123, 0.03f, true));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 124, 129, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
			if (PlayerLegState::VerticalJumping == leg_ || PlayerLegState::ForwardJumping == leg_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Firing_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 140, 143, 0.03f, true));
	topPistolRenderer_->AnimationBindEnd("Jumping_Firing_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_FiringToAiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 144, 145, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Jumping_FiringToAiming_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 150, 152, 0.02f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_ThrowingGrenade",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::ThrowingGrenadeToAiming;
		}
	);
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenadeToAiming",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 153, 155, 0.2f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_ThrowingGrenadeToAiming",
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



	//topWeaponRenderer_ = CreateComponent<GameEngineTextureRenderer>("TopWeaponRenderer");
	//topWeaponRenderer_->GetTransform().SetLocalScale(300, 300, 1);

	//if (0 == GameEngineTexture::Find("Tarma_Top_Weapon.png")->GetCutCount())
	//{
	//	GameEngineTexture::Cut("Tarma_Top_Weapon.png", 10, 23);
	//}
	//topWeaponRenderer_->SetTexture("Tarma_Top_Weapon.png");

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
		GameEngineTexture::Cut("Tarma_Whole_Pistol.png", 10, 11);
	}
	wholePistolRenderer_->SetTexture("Tarma_Whole_Pistol.png");

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Running, JumpingToStanding",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 0, 3, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Running, JumpingToStanding",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = PlayerLegState::Standing;
			wholePistolRenderer_->CurAnimationReset();
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
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 40, 43, 0.03f, true));
	wholePistolRenderer_->AnimationBindEnd("Ducking_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::FiringToAiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 44, 49, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming2_Forward");
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming2_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 50, 53, 0.2f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 60, 62, 0.03f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_ThrowingGrenade",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::ThrowingGrenadeToAiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_ThrowingGrenadeToAiming",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 63, 65, 0.2f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_ThrowingGrenadeToAiming",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = PlayerTopState::Aiming;
			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming3_Forward");
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming3_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 70, 73, 0.2f, true));


	wholePistolRenderer_->SetPivot(PivotMode::Center);
	wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
	wholePistolRenderer_->Off();
}

void TestPlayer::CreatePlayerStates()
{
	playerStateManager_.CreateState(		//1000
		"Dead",
		nullptr,
		[this](const StateInfo& _info)->void {
			
		}
	);

	playerStateManager_.CreateState(		//1111
		"Pistol_Standing_Aiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1112
		"Pistol_Standing_Aiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Aiming_Upward");
		}
	);

	playerStateManager_.CreateState(		//1114
		"Pistol_Standing_Aiming_ForwardToUpward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_ForwardToUpward");
		}
	);

	playerStateManager_.CreateState(		//1115
		"Pistol_Standing_Aiming_UpwardToForward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_UpwardToForward");
		}
	);

	playerStateManager_.CreateState(		//1121
		"Pistol_Standing_Firing_Forward",
		nullptr, 
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(		//1122
		"Pistol_Standing_Firing_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(		//1131
		"Pistol_Standing_FiringToAiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1132
		"Pistol_Standing_FiringToAiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);	


	playerStateManager_.CreateState(		//1141
		"Pistol_Standing_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenade");
		}
	);	

	playerStateManager_.CreateState(		//1151
		"Pistol_Standing_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenadeToAiming");
		}
	);	











	//Pistol_Standing_MeleeAttack = 1171,











	playerStateManager_.CreateState(		//1211
		"Pistol_Running_Aiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Running_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1212
		"Pistol_Running_Aiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Aiming_Upward");
		}
	);

	playerStateManager_.CreateState(		//1214
		"Pistol_Running_Aiming_ForwardToUpward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_ForwardToUpward");
		}
	);

	playerStateManager_.CreateState(		//1215
		"Pistol_Running_Aiming_UpwardToForward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running_Aiming_UpwardToForward");
		}
	);
 
	playerStateManager_.CreateState(		//1221
		"Pistol_Running_Firing_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);
		
	playerStateManager_.CreateState(		//1222	
		"Pistol_Running_Firing_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(		//1231
		"Pistol_Running_FiringToAiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1232
		"Pistol_Running_FiringToAiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);

	playerStateManager_.CreateState(		//1241
		"Pistol_Running_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenade");
		}
	);

	playerStateManager_.CreateState(		//1251
		"Pistol_Running_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenadeToAiming");
		}
	);










	//Pistol_Running_MeleeAttack = 1271,













	playerStateManager_.CreateState(		//1311
		"Pistol_Ducking_Aiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();


			if ("Pistol_Ducking_ThrowingGrenadeToAiming" == _info.prevState_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming3_Forward");
			}
			else if ("Pistol_Ducking_FiringToAiming" == _info.prevState_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming2_Forward");
			}
			else
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
			}
		}
	);

	playerStateManager_.CreateState(		//1321
		"Pistol_Ducking_Firing",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(		//1331
		"Pistol_Ducking_FiringToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1341
		"Pistol_Ducking_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_ThrowingGrenade");
		}
	);

	playerStateManager_.CreateState(		//1351
		"Pistol_Ducking_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_ThrowingGrenadeToAiming");
		}
	);

	playerStateManager_.CreateState(		//1361
		"Pistol_Ducking_DuckStepping",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Ducksteping_Forward");
		}
	);










	//Pistol_Ducking_MeleeAttack = 1371,










	playerStateManager_.CreateState(		//1411
		"Pistol_VerticalJumping_Aiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("VerticalJumping_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1413
		"Pistol_VerticalJumping_Aiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_Downward");
		}
	);

	playerStateManager_.CreateState(		//1416
		"Pistol_VerticalJumping_Aiming_ForwardToDownward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_ForwardToDownward");
		}
	);

	playerStateManager_.CreateState(		//1417
		"Pistol_VerticalJumping_Aiming_DownwardToForward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();
	
			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_DownwardToForward");
		}
	);

	playerStateManager_.CreateState(		//1421
		"Pistol_VerticalJumping_Firing_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(		//1422
		"Pistol_VerticalJumping_Firing_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(		//1423
		"Pistol_VerticalJumping_Firing_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Firing_Downward");
		}
	);	

	playerStateManager_.CreateState(		//1431
		"Pistol_VerticalJumping_FiringToAiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);	

	playerStateManager_.CreateState(		//1432
		"Pistol_VerticalJumping_FiringToAiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);	

	playerStateManager_.CreateState(		//1433
		"Pistol_VerticalJumping_FiringToAiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_FiringToAiming_Downward");
		}
	);	

	playerStateManager_.CreateState(		//1441
		"Pistol_VerticalJumping_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenade");
		}
	);	

	playerStateManager_.CreateState(		//1451
		"Pistol_VerticalJumping_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenadeToAiming");
		}
	);	
	






	//	Pistol_VerticalJumping_MeleeAttack = 1471,








	playerStateManager_.CreateState(		//1511
		"Pistol_ForwardJumping_Aiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("VerticalJumping_Aiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1513
		"Pistol_ForwardJumping_Aiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_Downward");
		}
	);

	playerStateManager_.CreateState(		//1516
		"Pistol_ForwardJumping_Aiming_ForwardToDownward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_ForwardToDownward");
		}
	);

	playerStateManager_.CreateState(		//1517
		"Pistol_ForwardJumping_Aiming_DownwardToForward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_DownwardToForward");
		}
	);

	playerStateManager_.CreateState(		//1521
		"Pistol_ForwardJumping_Firing_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");
		}
	);

	playerStateManager_.CreateState(		//1522
		"Pistol_ForwardJumping_Firing_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");
		}
	);

	playerStateManager_.CreateState(		//1523
		"Pistol_ForwardJumping_Firing_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Firing_Downward");
		}
	);

	playerStateManager_.CreateState(		//1531
		"Pistol_ForwardJumping_FiringToAiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");
		}
	);

	playerStateManager_.CreateState(		//1532
		"Pistol_ForwardJumping_FiringToAiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");
		}
	);

	playerStateManager_.CreateState(		//1533
		"Pistol_ForwardJumping_FiringToAiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_FiringToAiming_Downward");
		}
	);

	playerStateManager_.CreateState(		//1541
		"Pistol_ForwardJumping_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenade");
		}
	);

	playerStateManager_.CreateState(		//1551
		"Pistol_ForwardJumping_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenadeToAiming");
		}
	);










	//	Pistol_ForwardJumping_MeleeAttack = 1571,











	playerStateManager_.CreateState(		//1611
		"Pistol_StandingToDucking",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("StandingToDucking");
		}
	);

	playerStateManager_.CreateState(		//1711
		"Pistol_RunningToStanding",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Running, JumpingToStanding");
		}
	);

	playerStateManager_.CreateState(		//1811
		"Pistol_JumpingToStanding",
		nullptr,
		[this](const StateInfo& _info)->void {
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

	playerStateManager_.ChangeState(allPlayerStates_.find(1111)->second.second);
}
