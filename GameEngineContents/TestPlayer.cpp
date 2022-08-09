#include "PreCompile.h"
#include "TestPlayer.h"

TestPlayer::TestPlayer()
	: legRenderer_(nullptr),
	topPistolRenderer_(nullptr),
	wholePistolRenderer_(nullptr),
	topWeaponRenderer_(nullptr),
	wholeWeaponRenderer_(nullptr),
	playerStateManager_(nullptr),
	weapon_(PlayerWeaponType::Pistol),
	velocity_(100.f),
	bulletCount_(-1),
	grenadeCount_(0)
{
}

TestPlayer::~TestPlayer()
{
}

void TestPlayer::Start()
{

	this->GetTransform().SetWorldPosition(0, 0, 0);


	if (false == GameEngineInput::GetInst()->IsKey("MoveLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("MoveLeft", 'J');
		GameEngineInput::GetInst()->CreateKey("MoveRight", 'L');
		GameEngineInput::GetInst()->CreateKey("LookUp", 'I');
		GameEngineInput::GetInst()->CreateKey("LookDown", 'K');

		GameEngineInput::GetInst()->CreateKey("Attack", 'X');
		GameEngineInput::GetInst()->CreateKey("Jump", 'Z');
		GameEngineInput::GetInst()->CreateKey("Special", 'S');
		GameEngineInput::GetInst()->CreateKey("Test", 'A');
	}

	legRenderer_ = CreateComponent<GameEngineTextureRenderer>("RegRenderer");
	legRenderer_->GetTransform().SetLocalScale(300, 300, 1);
	topPistolRenderer_ = CreateComponent<GameEngineTextureRenderer>("TopPistolRenderer");
	topPistolRenderer_->GetTransform().SetLocalScale(300, 300, 1);
	wholePistolRenderer_ = CreateComponent<GameEngineTextureRenderer>("WholePistolRenderer");
	wholePistolRenderer_->GetTransform().SetLocalScale(300, 300, 1);


	if (0 == GameEngineTexture::Find("Tarma_Leg.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Leg.png", 6, 5);
	}
	legRenderer_->SetTexture("Tarma_Leg.png");
	legRenderer_->SetPivot(PivotMode::Center);

	legRenderer_->CreateFrameAnimation_CutTexture("Standing",
		FrameAnimation_Desc("Tarma_Leg.png", 0, 0, 0.5f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Tarma_Leg.png", 6, 17, 0.05f, true));
	legRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 18, 23, 0.05f, false));
	legRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping",
		FrameAnimation_Desc("Tarma_Leg.png", 24, 29, 0.05f, false));
	legRenderer_->ChangeFrameAnimation("Standing");

	
	if (0 == GameEngineTexture::Find("Tarma_Top_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Top_Pistol.png", 6, 17);
	}
	topPistolRenderer_->SetTexture("Tarma_Top_Pistol.png");	
	topPistolRenderer_->SetPivot(PivotMode::Center);

	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 0, 3, 0.01f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Running_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 6, 17, 0.05f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("VerticalJumping_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 18, 23, 0.05f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("ForwardJumping_Aiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 24, 29, 0.05f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_Aiming_ForwardToUpward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 30, 31, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_Aiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 36, 39, 0.05f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_ForwardToDownward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 42, 43, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Aiming_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 48, 48, 0.05f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_Firing_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 54, 59, 0.01f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_FiringToAiming_Forward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 60, 63, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_Firing_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 66, 71, 0.01f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running_FiringToAiming_Upward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 72, 75, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Jumping_Firing_Downward",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 78, 83, 0.01f, true));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running,Jumping_ThrowingGrenade",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 84, 89, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running,Jumping_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 90, 95, 0.01f, false));
	topPistolRenderer_->CreateFrameAnimation_CutTexture("Standing,Running,Jumping_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Top_Pistol.png", 96, 101, 0.01f, false));
	topPistolRenderer_->ChangeFrameAnimation("Standing_Aiming_Forward");

	
	if (0 == GameEngineTexture::Find("Tarma_Whole_Pistol.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_Whole_Pistol.png", 7, 9);
	}
	wholePistolRenderer_->SetTexture("Tarma_Whole_Pistol.png");
	wholePistolRenderer_->SetPivot(PivotMode::Center);

	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Running,JumpingToStanding",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 0, 3, 0.05f, false));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("StandingToDucking",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 7, 9, 0.01f, false));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Aiming",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 14, 17, 0.05f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Advancing",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 21, 27, 0.05f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_Firing",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 28, 33, 0.01f, true));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_FiringToAiming",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 35, 38, 0.01f, false));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_ThowingGrenade",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 42, 47, 0.01f, false));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_MeleeAttack1",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 49, 52, 0.01f, false));
	wholePistolRenderer_->CreateFrameAnimation_CutTexture("Ducking_MeleeAttack2",
		FrameAnimation_Desc("Tarma_Whole_Pistol.png", 56, 61, 0.01f, false));
	wholePistolRenderer_->ChangeFrameAnimation("Ducking_Aiming");
	
	
	
	
	
	playerStateManager_ = CreateComponent<GameEngineStateManager>("PlayerStateManager");
	//playerStateManager_->CreateState();
	









}

void TestPlayer::Update(float _deltaTime)
{



}

void TestPlayer::End()
{
}


