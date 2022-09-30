#include "PreCompile.h"
#include "Soldier.h"
#include "Indicator.h"

void Soldier::CreateSoldierAnimations()
{
	legRenderer_ = CreateComponent<GameEngineTextureRenderer>("RegRenderer");
	legRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	legRenderer_->SetPivot(PivotMode::Center);

	legRenderer_->GetTransform().SetLocalPosition(
		soldierRendererLocalPosX_,
		soldierRendererLocalPosY_,
		soldierRendererLocalPosZ_ + 5
	);

	if (0 == GameEngineTexture::Find("Tarma_Leg.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Leg.png", 10, 6);
	}
	legRenderer_->SetTexture("Tarma_Leg.png");

	legRenderer_->CreateFrameAnimation_CutTexture("Standing",
		FrameAnimation_Desc("Tarma_Leg.png", 0, 0, 1.f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Tarma_Leg.png", 10, 21, 0.05f, true));
	legRenderer_->AnimationBindTime("Running",
		std::bind(&Soldier::Run, this)
	);

	legRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 30, 40, 0.05f, false));
	legRenderer_->AnimationBindEnd("VerticalJumping",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = SoldierLegState::Falling;
		}
	);
	legRenderer_->CreateFrameAnimation_CutTexture("Falling",
		FrameAnimation_Desc("Tarma_Leg.png", 40, 40, 0.5f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 50, 55, 0.05f, false));

	legRenderer_->ChangeFrameAnimation("Standing");
	legRenderer_->Off();








	topPistolRenderer_ = CreateComponent<GameEngineTextureRenderer>("TopPistolRenderer");
	topPistolRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	topPistolRenderer_->SetPivot(PivotMode::Center);
	topPistolRenderer_->GetTransform().SetLocalPosition(
		soldierRendererLocalPosX_,
		soldierRendererLocalPosY_,
		soldierRendererLocalPosZ_
	);

	if (0 == GameEngineTexture::Find("Tarma_Top_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Top_Pistol.png", 10, 17);
	}
	topPistolRenderer_->SetTexture("Tarma_Top_Pistol.png");

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 0, 3, 0.2f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Running_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 10, 21, 0.05f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 30, 35, 0.05f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Falling_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 35, 35, 0.5f, true));
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
	topPistolRenderer_->AnimationBindFrame("Standing, Running, Jumping_Firing_Forward", 
		[this](const FrameAnimation_Desc& _desc)->void {
			if (1 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::FiringToAiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 114, 119, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_Firing_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 120, 123, 0.03f, true));
	topPistolRenderer_->AnimationBindFrame("Standing, Running, Jumping_Firing_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (1 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_Firing_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::FiringToAiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_FiringToAiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 124, 129, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_FiringToAiming_Upward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
			if (SoldierLegState::VerticalJumping == leg_ || SoldierLegState::ForwardJumping == leg_ || SoldierLegState::Falling == leg_)
			{
				direction_ = AimingDirection::Forward;
			}
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Firing_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 130, 133, 0.03f, true));
	topPistolRenderer_->AnimationBindFrame("Jumping_Firing_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (1 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	topPistolRenderer_->AnimationBindEnd("Jumping_Firing_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::FiringToAiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_FiringToAiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 134, 135, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Jumping_FiringToAiming_Downward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 140, 143, 0.03f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_ThrowingGrenade",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::ThrowingGrenadeToAiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_ThrowingGrenadeToAiming",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 144, 145, 0.05f, false));
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_ThrowingGrenadeToAiming",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 150, 155, 0.05f, false));
	topPistolRenderer_->AnimationBindFrame("Standing, Running, Jumping_MeleeAttack1", 
		[this](const FrameAnimation_Desc& _desc)->void {
			if (2 == _desc.curFrame_)
			{
				MeleeAttack();
			}
		}
	);
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_MeleeAttack1",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing, Running, Jumping_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 160, 165, 0.05f, false));
	topPistolRenderer_->AnimationBindFrame("Standing, Running, Jumping_MeleeAttack2",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (2 == _desc.curFrame_)
			{
				MeleeAttack();
			}
		}
	);
	topPistolRenderer_->AnimationBindEnd("Standing, Running, Jumping_MeleeAttack2",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);


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
	wholePistolRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	wholePistolRenderer_->SetPivot(PivotMode::Center);
	wholePistolRenderer_->GetTransform().SetLocalPosition(
		soldierRendererLocalPosX_,
		soldierRendererLocalPosY_,
		soldierRendererLocalPosZ_
	);

	if (0 == GameEngineTexture::Find("Tarma_Whole_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Whole_Pistol.png", 10, 17);
	}
	wholePistolRenderer_->SetTexture("Tarma_Whole_Pistol.png");

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Running, JumpingToStanding",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 0, 3, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Running, JumpingToStanding",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = SoldierLegState::Standing;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("StandingToDucking",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 10, 12, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("StandingToDucking",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = SoldierLegState::Ducking;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming1_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 20, 23, 0.2f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Ducksteping_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 30, 36, 0.05f, true));
	wholePistolRenderer_->AnimationBindTime("Ducking_Ducksteping_Forward",
		std::bind(&Soldier::DuckStep, this)
	);

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Firing_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 40, 43, 0.03f, true));
	wholePistolRenderer_->AnimationBindFrame("Ducking_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (1 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	wholePistolRenderer_->AnimationBindEnd("Ducking_Firing_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::FiringToAiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 44, 49, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_FiringToAiming_Forward",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming2_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 50, 53, 0.2f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 60, 63, 0.03f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_ThrowingGrenade",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::ThrowingGrenadeToAiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_ThrowingGrenadeToAiming",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 64, 65, 0.05f, false));
	wholePistolRenderer_->AnimationBindEnd("Ducking_ThrowingGrenadeToAiming",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming3_Forward",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 70, 73, 0.2f, true));

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 80, 85, 0.05f, true));
	wholePistolRenderer_->AnimationBindFrame("Ducking_MeleeAttack1",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (2 == _desc.curFrame_)
			{
				MeleeAttack();
			}
		}
	);
	wholePistolRenderer_->AnimationBindEnd("Ducking_MeleeAttack1",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 90, 95, 0.05f, true));
	wholePistolRenderer_->AnimationBindFrame("Ducking_MeleeAttack2",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (2 == _desc.curFrame_)
			{
				MeleeAttack();
			}
		}
	);
	wholePistolRenderer_->AnimationBindEnd("Ducking_MeleeAttack2",
		[this](const FrameAnimation_Desc& _desc)->void {
			top_ = SoldierTopState::Aiming;
		}
	);


	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Fallen_ByMeleeAttack_Ground",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 100, 118, 0.05f, false));

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Fallen_ByFlyingSword_Ground",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 120, 138, 0.05f, false));

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Fallen_BySolidAttack_Midair",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 140, 149, 0.05f, false));
	wholePistolRenderer_->AnimationBindStart("Fallen_BySolidAttack_Midair",
		[this](const FrameAnimation_Desc& _desc)->void {
			fallingSpeed_ = -1.f;
		}
	);
	wholePistolRenderer_->AnimationBindTime("Fallen_BySolidAttack_Midair",
		[this](const FrameAnimation_Desc& _desc, float _deltaTime)->void {
			if (true == isAirborne_)
			{
				movementFor1Second_ += float4::Right * -this->GetTransform().GetWorldScale().x * runningSpeed_;
			}
		}
	);


	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Fallen_BySolidBullet_Ground",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 150, 169, 0.05f, false));


	wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
	//wholePistolRenderer_->GetPixelData().plusColor_ = float4(0.35f, 0.35f, 0.35f, 0.f);
	wholePistolRenderer_->Off();




	redeployingRenderer_ = CreateComponent<GameEngineTextureRenderer>("RedeployingRenderer");
	if (0 == GameEngineTexture::Find("Tarma_Redeploying.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Redeploying.png", 7, 1);
	}
	redeployingRenderer_->SetTexture("Tarma_Redeploying.png");
	redeployingRenderer_->SetPivot(PivotMode::Bot);
	redeployingRenderer_->GetTransform().SetLocalScale(128, 960, 1);
	redeployingRenderer_->GetTransform().SetLocalPosition(float4::Zero);
	redeployingRenderer_->CreateFrameAnimation_CutTexture("Redeploying",
		FrameAnimation_Desc("Tarma_Redeploying.png", 0, 6, 0.05f, false));
	redeployingRenderer_->AnimationBindEnd("Redeploying",
		[this](const FrameAnimation_Desc& _desc)->void {
			leg_ = SoldierLegState::Ducking;
			top_ = SoldierTopState::Aiming;
			direction_ = AimingDirection::Forward;
		}
	);
	redeployingRenderer_->ChangeFrameAnimation("Redeploying");
	redeployingRenderer_->Off();


	allTextureRenderers_ = this->GetConvertedChildren<GameEngineTextureRenderer>();
}

void Soldier::CreateSoldierStates()
{
	soldierStateManager_.CreateState(		//1010
		"Fallen_ByMeleeAttack",
		[this](float _deltaTime, const StateInfo& _info)->void {
			if (2.f <= _info.stateTime_)
			{
				causeOfDeath_ = 0;
				weapon_ = SoldierWeaponType::Pistol;
				leg_ = SoldierLegState::Redeploying;
				top_ = SoldierTopState::Aiming;
				direction_ = AimingDirection::Forward;
				wholePistolRenderer_->CurAnimationReset();
			}
			else if (1.f <= _info.stateTime_ && false == isAirborne_)
			{
				Flicker(_deltaTime, true, float4(0, 0, 0, -1));
			}
		},
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			if (true == isAirborne_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Fallen_BySolidAttack_Midair");
			}
			else
			{
				wholePistolRenderer_->ChangeFrameAnimation("Fallen_ByMeleeAttack_Ground");
			}

			soldierLifeCollisionBody_->Off();
			soldierCloseCombatCollisionBody_->Off();
		}
	);

	soldierStateManager_.CreateState(		//1011
		"Fallen_ByFlyingSword",
		[this](float _deltaTime, const StateInfo& _info)->void {
			if (2.f <= _info.stateTime_)
			{
				causeOfDeath_ = 0;
				weapon_ = SoldierWeaponType::Pistol;
				leg_ = SoldierLegState::Redeploying;
				top_ = SoldierTopState::Aiming;
				direction_ = AimingDirection::Forward;
				wholePistolRenderer_->CurAnimationReset();
			}
			else if (1.f <= _info.stateTime_ && false == isAirborne_)
			{
				Flicker(_deltaTime, true, float4(0, 0, 0, -1));
			}
		},
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			if (true == isAirborne_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Fallen_BySolidAttack_Midair");
			}
			else
			{
				wholePistolRenderer_->ChangeFrameAnimation("Fallen_ByFlyingSword_Ground");
			}


			soldierLifeCollisionBody_->Off();
			soldierCloseCombatCollisionBody_->Off();
		}
	);





	soldierStateManager_.CreateState(		//1111
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
			
			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);


			//legRenderer_->Off();
			//topPistolRenderer_->Off();
			//wholePistolRenderer_->On();
			//
			//wholePistolRenderer_->ChangeFrameAnimation("Fallen_BySolidAttack_Midair");
			//
			//playerLifeCollisionBody_->GetTransform().SetLocalScale(playerLifeCollisionBodyScale_Standing_);
			//playerLifeCollisionBody_->GetTransform().SetLocalPosition(playerLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1112
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1114
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1115
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1121
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1122
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1131
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1132
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);	


	soldierStateManager_.CreateState(		//1141
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);	

	soldierStateManager_.CreateState(		//1151
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);	

	soldierStateManager_.CreateState(		//1171
		"Pistol_Standing_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Standing");

			if (true == this->isMeleeAttack1_)
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack1");
			}
			else
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);	





	soldierStateManager_.CreateState(		//1211
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1212
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1214
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1215
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);
 
	soldierStateManager_.CreateState(		//1221
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);
		
	soldierStateManager_.CreateState(		//1222	
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1231
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1232
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1241
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1251
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1271
		"Pistol_Running_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Running");

			if (true == this->isMeleeAttack1_)
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack1");
			}
			else
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);







	soldierStateManager_.CreateState(		//1311
		"Pistol_Ducking_Aiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();


			if ("Pistol_Ducking_ThrowingGrenadeToAiming" == _info.prevState_ 
				|| "Pistol_Redeploying" == _info.prevState_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming3_Forward");
			}
			else if ("Pistol_Ducking_FiringToAiming" == _info.prevState_ 
				|| "Pistol_Ducking_MeleeAttack" == _info.prevState_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming2_Forward");
			}
			else
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming1_Forward");
			}

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1321
		"Pistol_Ducking_Firing",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Firing_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1331
		"Pistol_Ducking_FiringToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_FiringToAiming_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1341
		"Pistol_Ducking_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_ThrowingGrenade");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1351
		"Pistol_Ducking_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_ThrowingGrenadeToAiming");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1361
		"Pistol_Ducking_DuckStepping",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Ducking_Ducksteping_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);

	soldierStateManager_.CreateState(		//1371
		"Pistol_Ducking_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			if (true == this->isMeleeAttack1_)
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_MeleeAttack1");
			}
			else
			{
				wholePistolRenderer_->ChangeFrameAnimation("Ducking_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;


			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);











	soldierStateManager_.CreateState(		//1411
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1413
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1416
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1417
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1421
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1422
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1423
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	

	soldierStateManager_.CreateState(		//1431
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	

	soldierStateManager_.CreateState(		//1432
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	

	soldierStateManager_.CreateState(		//1433
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	

	soldierStateManager_.CreateState(		//1441
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	

	soldierStateManager_.CreateState(		//1451
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);	
	
	soldierStateManager_.CreateState(		//1471
		"Pistol_VerticalJumping_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("VerticalJumping");

			if (true == this->isMeleeAttack1_)
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack1");
			}
			else
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);







	soldierStateManager_.CreateState(		//1511
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1513
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1516
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1517
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1521
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1522
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1523
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1531
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1532
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1533
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1541
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1551
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

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);

	soldierStateManager_.CreateState(		//1571
		"Pistol_ForwardJumping_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("ForwardJumping");

			if (true == this->isMeleeAttack1_)
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack1");
			}
			else
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);

			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -initialJumpSpeed_;		//점프 시작.
			}
		}
	);








	soldierStateManager_.CreateState(		//1611
		"Pistol_Falling_Aiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Falling_Aiming_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1613
		"Pistol_Falling_Aiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_Downward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1616
		"Pistol_Falling_Aiming_ForwardToDownward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_ForwardToDownward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1617
		"Pistol_Falling_Aiming_DownwardToForward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Aiming_DownwardToForward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1617
		"Pistol_Falling_Firing_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1622
		"Pistol_Falling_Firing_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_Firing_Upward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1623
		"Pistol_Falling_Firing_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_Firing_Downward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1631
		"Pistol_Falling_FiringToAiming_Forward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Forward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1632
		"Pistol_Falling_FiringToAiming_Upward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_FiringToAiming_Upward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1633
		"Pistol_Falling_FiringToAiming_Downward",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Jumping_FiringToAiming_Downward");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1641
		"Pistol_Falling_ThrowingGrenade",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenade");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1651
		"Pistol_Falling_ThrowingGrenadeToAiming",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");
			topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_ThrowingGrenadeToAiming");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1671
		"Pistol_Falling_MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->On();
			topPistolRenderer_->On();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			legRenderer_->ChangeFrameAnimation("Falling");

			if (true == this->isMeleeAttack1_)
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack1");
			}
			else
			{
				topPistolRenderer_->ChangeFrameAnimation("Standing, Running, Jumping_MeleeAttack2");
			}
			isMeleeAttack1_ = !isMeleeAttack1_;

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);








	soldierStateManager_.CreateState(		//1711
		"Pistol_StandingToDucking",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("StandingToDucking");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		}
	);

	soldierStateManager_.CreateState(		//1811
		"Pistol_RunningOrJumpingToStanding",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->On();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();

			wholePistolRenderer_->ChangeFrameAnimation("Running, JumpingToStanding");

			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Standing_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Standing_);
		},
		[this](const StateInfo& _info)->void {
			wholePistolRenderer_->CurAnimationReset();
		}
	);

	soldierStateManager_.CreateState(		//1911
		"Pistol_Redeploying",
		nullptr,
		[this](const StateInfo& _info)->void {
			legRenderer_->Off();
			topPistolRenderer_->Off();
			wholePistolRenderer_->Off();
			//topWeaponRenderer_->Off();
			//wholeWeaponRenderer_->Off();
			redeployingRenderer_->On();

			isDamageProof_ = true;
			remainedDamageProofDuration_ = damageProofTimeLimit_;
		},
		[this](const StateInfo& _info)->void {
			redeployingRenderer_->CurAnimationReset();
			redeployingRenderer_->Off();

			soldierLifeCollisionBody_->On();
			soldierCloseCombatCollisionBody_->On();
			soldierLifeCollisionBody_->GetTransform().SetLocalScale(soldierLifeCollisionBodyScale_Ducking_);
			soldierLifeCollisionBody_->GetTransform().SetLocalPosition(soldierLifeCollisionBodyPosition_Ducking_);
		}
	);




	std::vector<std::pair<SoldierState, std::string_view>> playerStateEntries(
		magic_enum::enum_entries<SoldierState>().begin(), magic_enum::enum_entries<SoldierState>().end()
	);
	size_t playerStateCount = playerStateEntries.size();
	allSoldierStates_.reserve(playerStateCount);

	for (size_t i = 0; i < playerStateCount; i++)
	{
		if (allSoldierStates_.end() != allSoldierStates_.find(static_cast<int>(playerStateEntries[i].first)))
		{
			MsgBoxAssertString(std::string(playerStateEntries[i].second) + std::string(": 이미 존재하는 플레이어 스테이트입니다."));
			return;
		}
		
		allSoldierStates_.insert(
			std::make_pair(
				static_cast<int>(playerStateEntries[i].first),
				std::make_pair(playerStateEntries[i].first, playerStateEntries[i].second.data())
			)
		);
	}

	soldierStateManager_.ChangeState(allSoldierStates_[1111].second);
}
