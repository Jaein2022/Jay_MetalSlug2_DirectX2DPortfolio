#include "PreCompile.h"
#include "CamelRider.h"
#include "PixelIndicator.h"
#include "Soldier.h"
#include "RebelBullet.h"

CamelRider::CamelRider()
	: currentCamelRiderState_(CamelRiderState::Up_Idling),
	riderRenderPivot_(nullptr),
	camelRenderer_(nullptr),
	riderRenderer_(nullptr),
	riderArmRenderer_(nullptr),
	camelRendererLocalPos_(0, 105, 0),
	riderRenderPivotPos_Up_(-60, 197, -5),
	riderRenderPivotPos_Down_(-60, 95, -5),
	riderRendererLocalPos_Up_(0, 0, 6),
	riderRendererLocalPos_Down_(0, 0, 3),
	riderArmRendererLocalPos_Up_(53, 60, 7),
	riderArmRendererLocalPos_Down_(60, 30, 4),
	upperLandingChecker_(nullptr),
	midLandingChecker_(nullptr),
	lowerLandingChecker_(nullptr),
	fallingSpeed_(0.f),
	runningSpeed_(4.0f),
	movementFor1Second_(float4::Zero),
	isAirborne_(false),
	isEngaging_(false),
	isSwordBroken_(false),
	riderCollisionBody_(nullptr),
	swordCollisionBody_(nullptr),
	riderCollisionBodyScale_Up_(90, 90, 10),
	riderCollisionBodyPosition_Up_(5, 48, 15),
	riderCollisionBodyScale_Down_(110, 100, 10),
	riderCollisionBodyPosition_Down_(-5, 25, 15),
	swordCollisionBodyScale_Up_(40, 135, 10),
	swordCollisionBodyPosition_Up_(90, 58, 15),
	swordCollisionBodyScale_Down_(60, 130, 10),
	swordCollisionBodyPosition_Down_(100, 20, 15),
	hp_(1),
	swordDurability_(8),
	enemySoldier_(nullptr),
	isRiderDirectionWrong_(false),
	muzzle_(nullptr),
	muzzleLocalPosition_Up_(125, 43, 10),
	muzzleLocalPosition_Down_(135, 30, 10)
{
}

CamelRider::~CamelRider()
{
}

void CamelRider::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);

	midLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"CamelRiderWorldPosPointer",
		this,
		float4::Red,
		float4(0, 0, -5),
		float4(5, 5, 1)
	);

	lowerLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"LowerLandingChecker",
		this,
		float4::Black,
		float4(0, -5, -5),
		float4(5, 5, 1)
	);

	riderRenderPivot_ = Indicator::CreateIndicator<Indicator>(
		"RiderRenderPivot",
		this,
		float4::Cyan,
		float4(riderRenderPivotPos_Up_),
		float4(1, 1, 1)
	);

	muzzle_ = Indicator::CreateIndicator<Indicator>(
		"Muzzle",
		riderRenderPivot_,
		float4::Red,
		float4(muzzleLocalPosition_Up_),
		float4(5, 5, 1)
	);

	riderCollisionBody_ = CreateComponent<GameEngineCollision>("RiderCollisionBody");
	riderCollisionBody_->ChangeOrder(ObjectOrder::Rebel);
	riderCollisionBody_->SetCollisionMode(CollisionMode::Single);
	riderCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
	riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);



	swordCollisionBody_ = CreateComponent<GameEngineCollision>("SwordCollisionBody");
	swordCollisionBody_->ChangeOrder(ObjectOrder::RebelMachine);
	swordCollisionBody_->SetCollisionMode(CollisionMode::Single);
	swordCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
	swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);


	enemySoldier_ = this->GetLevel()->GetConvertedGroup<Soldier>(ObjectOrder::Soldier).front();
	//낙타기수 생성은 반드시 솔저 생성 이후로 할 것.
	//솔저(플레이어)가 한명 이상으로 늘어나면 문제 발생할 수 있음.





	camelRenderer_ = CreateComponent<GameEngineTextureRenderer>("CamelRenderer");
	camelRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	camelRenderer_->SetPivot(PivotMode::Center);
	camelRenderer_->GetTransform().SetLocalPosition(camelRendererLocalPos_);

	if (0 == GameEngineTexture::Find("Rebel_Camel.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_Camel.png", 10, 10);
	}
	camelRenderer_->SetTexture("Rebel_Camel.png");
	camelRenderer_->CreateFrameAnimation_CutTexture("Up_Idling",
		FrameAnimation_Desc("Rebel_Camel.png", 0, 5, 0.3f, true)
	);

	camelRenderer_->CreateFrameAnimation_CutTexture("Up_Running",
		FrameAnimation_Desc("Rebel_Camel.png", 10, 21, 0.05f, true)
	);	
	camelRenderer_->AnimationBindTime("Up_Running",
		std::bind(&CamelRider::Run, this)
	);

	camelRenderer_->CreateFrameAnimation_CutTexture("Up_RunningBackward",
		FrameAnimation_Desc("Rebel_Camel.png", 30, 41, 0.05f, true)
	);
	camelRenderer_->AnimationBindTime("Up_RunningBackward",
		std::bind(&CamelRider::RunBackward, this)
	);
	
	camelRenderer_->CreateFrameAnimation_CutTexture("UpToDown",
		FrameAnimation_Desc("Rebel_Camel.png", 50, 52, 0.075f, false)
	);	
	camelRenderer_->AnimationBindEnd("UpToDown",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Down_Idling;
		}
	);
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_Camel.png", 60, 65, 0.3f, true)
	);	
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("DownToUp",
		FrameAnimation_Desc("Rebel_Camel.png", 70, 72, 0.075f, false)
	);	
	camelRenderer_->AnimationBindEnd("DownToUp",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
		}
	);
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("Dead",
		FrameAnimation_Desc("Rebel_Camel.png", 80, 91, 0.05f, true)
	);


	camelRenderer_->ChangeFrameAnimation("Up_Idling");





	riderRenderer_ = CreateComponent<GameEngineTextureRenderer>("RiderRenderer");
	riderRenderer_->SetPivot(PivotMode::Center);
	riderRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);


	if (0 == GameEngineTexture::Find("Rebel_CamelRider.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_CamelRider.png", 10, 15);
	}
	riderRenderer_->SetTexture("Rebel_CamelRider.png");
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Entrance",
		FrameAnimation_Desc("Rebel_CamelRider.png", 0, 9, 0.1f, true)
	);	
	riderRenderer_->AnimationBindEnd("Up_Entrance",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Idling",
		FrameAnimation_Desc("Rebel_CamelRider.png", 10, 13, 0.15f, true)
	);	
	riderRenderer_->AnimationBindEnd("Up_Idling",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			if (true == isEngaging_)
			{
				React();
			}
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Firing",
		FrameAnimation_Desc("Rebel_CamelRider.png", 20, 29, 0.1f, true)
	);	
	riderRenderer_->AnimationBindFrame("Up_Firing",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			if (6 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	riderRenderer_->AnimationBindEnd("Up_Firing",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);

	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Turning",
		FrameAnimation_Desc("Rebel_CamelRider.png", 30, 33, 0.1f, false)
	);	
	riderRenderer_->AnimationBindEnd("Up_Turning",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("UpToDown",
		FrameAnimation_Desc("Rebel_CamelRider.png", 40, 42, 0.075f, false)
	);	
	riderRenderer_->AnimationBindEnd("UpToDown",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_CamelRider.png", 50, 53, 0.15f, true)
	);	
	riderRenderer_->AnimationBindEnd("Down_Idling",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Firing",
		FrameAnimation_Desc("Rebel_CamelRider.png", 60, 69, 0.1f, true)
	);	
	riderRenderer_->AnimationBindFrame("Down_Firing",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			if (6 == _desc.curFrame_)
			{
				Fire();
			}
		}
	);
	riderRenderer_->AnimationBindEnd("Down_Firing",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);

	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp",
		FrameAnimation_Desc("Rebel_CamelRider.png", 70, 72, 0.075f, false)
	);	
	riderRenderer_->AnimationBindEnd("DownToUp",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_SwordBreaking",
		FrameAnimation_Desc("Rebel_CamelRider.png", 80, 85, 0.1f, false)
	);
	riderRenderer_->AnimationBindEnd("Up_SwordBreaking",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
			isSwordBroken_ = true;
		}
	);

	riderRenderer_->CreateFrameAnimation_CutTexture("Down_SwordBreaking",
		FrameAnimation_Desc("Rebel_CamelRider.png", 90, 95, 0.1f, false)
	);	
	riderRenderer_->AnimationBindEnd("Down_SwordBreaking",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Down_Idling;
			isSwordBroken_ = true;
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Turning_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 100, 103, 0.1f, true)
	);	
	riderRenderer_->AnimationBindEnd("Up_Turning_SwordBroken",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("UpToDown_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 110, 112, 0.075f, false)
	);	
	riderRenderer_->AnimationBindEnd("UpToDown_SwordBroken",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 120, 122, 0.075f, false)
	);	
	riderRenderer_->AnimationBindEnd("DownToUp_SwordBroken",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			React();
		}
	);
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Dead",
		FrameAnimation_Desc("Rebel_CamelRider.png", 130, 142, 0.1f, false)
	);


	riderRenderer_->ChangeFrameAnimation("Up_Idling");





	riderArmRenderer_ = CreateComponent<GameEngineTextureRenderer>("RiderArmRenderer");
	riderArmRenderer_->SetPivot(PivotMode::Center);
	riderArmRenderer_->GetTransform().SetLocalScale(150, 150, 1);
	riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

	if (0 == GameEngineTexture::Find("Rebel_CamelRiderArm.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_CamelRiderArm.png", 5, 6);
	}
	riderArmRenderer_->SetTexture("Rebel_CamelRiderArm.png");
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Up_Idling",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 0, 3, 0.15f, true)
	);	
	
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Up_TakingDamage",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 5, 7, 0.05f, false)
	);	
	riderArmRenderer_->AnimationBindEnd("Up_TakingDamage",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			riderArmRenderer_->ChangeFrameAnimation("Up_Idling");
		}
	);
	
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 10, 13, 0.15f, true)
	);	
	
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Down_TakingDamage",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 15, 17, 0.05f, false)
	);
	riderArmRenderer_->AnimationBindEnd("Down_TakingDamage",
		[this](const FrameAnimation_Desc& _desc)->void
		{
			riderArmRenderer_->ChangeFrameAnimation("Down_Idling");
		}
	);
	
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Up_Idling_Broken",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 20, 23, 0.15f, true)
	);	
	
	riderArmRenderer_->CreateFrameAnimation_CutTexture("Down_Idling_Broken",
		FrameAnimation_Desc("Rebel_CamelRiderArm.png", 25, 28, 0.15f, true)
	);

	riderArmRenderer_->ChangeFrameAnimation("Up_Idling");




	riderRenderer_->SetParent(riderRenderPivot_);
	riderArmRenderer_->SetParent(riderRenderPivot_);
	riderCollisionBody_->SetParent(riderRenderPivot_);
	swordCollisionBody_->SetParent(riderRenderPivot_);



	camelRiderStateManager_.CreateState(
		"Up_Entrance",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_Running");
			riderRenderer_->ChangeFrameAnimation("Up_Entrance");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->Off();				 
			swordCollisionBody_->Off();
		},
		[this](const StateInfo& _info)->void
		{
			riderCollisionBody_->On();
			swordCollisionBody_->On();

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);

			isEngaging_ = true;
		}
	);


	camelRiderStateManager_.CreateState(
		"Up_Idling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_Idling");
			riderRenderer_->ChangeFrameAnimation("Up_Idling");
			riderArmRenderer_->On();

			if (false == riderArmRenderer_->IsCurAnimation("Up_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Up_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Up_Idling_Broken");
			}

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);

			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);


	camelRiderStateManager_.CreateState(
		"Up_Running",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_Running");
			riderRenderer_->ChangeFrameAnimation("Up_Idling");
			riderArmRenderer_->On();
			if (false == riderArmRenderer_->IsCurAnimation("Up_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Up_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Up_Idling_Broken");
			}

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Up_);
			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);

	camelRiderStateManager_.CreateState(
		"Up_RunningBackward",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_RunningBackward");
			riderRenderer_->ChangeFrameAnimation("Up_Idling");
			riderArmRenderer_->On();
			if (false == riderArmRenderer_->IsCurAnimation("Up_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Up_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Up_Idling_Broken");
			}

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Up_);
			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);

	camelRiderStateManager_.CreateState(
		"Up_Turning",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			camelRenderer_->ChangeFrameAnimation("Up_Idling");
			(false == isSwordBroken_) ? riderRenderer_->ChangeFrameAnimation("Up_Turning")
				: riderRenderer_->ChangeFrameAnimation("Up_Turning_SwordBroken");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->Off();
		},
		[this](const StateInfo& _info)->void
		{
			isRiderDirectionWrong_ = false;
			if (0.f < riderRenderPivot_->GetTransform().GetLocalScale().x)
			{
				riderRenderPivot_->GetTransform().PixLocalNegativeX();
			}
			else
			{
				riderRenderPivot_->GetTransform().PixLocalPositiveX();
			}
		}
	);
		
		
	camelRiderStateManager_.CreateState(
		"Up_Firing",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_Idling");
			riderRenderer_->ChangeFrameAnimation("Up_Firing");
			riderArmRenderer_->On();
			if (false == riderArmRenderer_->IsCurAnimation("Up_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Up_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Up_Idling_Broken");
			}

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Up_);
			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);

	camelRiderStateManager_.CreateState(
		"Up_SwordBreaking",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Up_);

			camelRenderer_->ChangeFrameAnimation("Up_Idling");
			riderRenderer_->ChangeFrameAnimation("Up_SwordBreaking");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Up_);
			swordCollisionBody_->Off();
		}
	);


	camelRiderStateManager_.CreateState(
		"UpToDown",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Down_);

			camelRenderer_->ChangeFrameAnimation("UpToDown");
			(false == isSwordBroken_) ? riderRenderer_->ChangeFrameAnimation("UpToDown")
				: riderRenderer_->ChangeFrameAnimation("UpToDown_SwordBroken");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Down_);
			swordCollisionBody_->Off();
		}
	);


	camelRiderStateManager_.CreateState(
		"Down_Idling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Down_);

			camelRenderer_->ChangeFrameAnimation("Down_Idling");
			riderRenderer_->ChangeFrameAnimation("Down_Idling");
			riderArmRenderer_->On();
			if (false == riderArmRenderer_->IsCurAnimation("Down_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Down_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Down_Idling_Broken");
			}
		

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Down_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Down_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Down_);
			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);

	camelRiderStateManager_.CreateState(
		"Down_Firing",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Down_);

			camelRenderer_->ChangeFrameAnimation("Down_Idling");
			riderRenderer_->ChangeFrameAnimation("Down_Firing");
			riderArmRenderer_->On();
			if (false == riderArmRenderer_->IsCurAnimation("Down_TakingDamage"))
			{
				(false == isSwordBroken_) ? riderArmRenderer_->ChangeFrameAnimation("Down_Idling")
					: riderArmRenderer_->ChangeFrameAnimation("Down_Idling_Broken");
			}
		

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);

			swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Down_);
			swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Down_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Down_);
			if (false == isSwordBroken_)
			{
				swordCollisionBody_->On();
			}
		}
	);

	camelRiderStateManager_.CreateState(
		"Down_SwordBreaking",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Down_);

			camelRenderer_->ChangeFrameAnimation("Down_Idling");
			riderRenderer_->ChangeFrameAnimation("Down_SwordBreaking");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Down_);
			swordCollisionBody_->Off();
		}
	);

	camelRiderStateManager_.CreateState(
		"DownToUp",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			riderRenderPivot_->GetTransform().SetLocalPosition(riderRenderPivotPos_Down_);

			camelRenderer_->ChangeFrameAnimation("DownToUp");
			(false == isSwordBroken_) ? riderRenderer_->ChangeFrameAnimation("DownToUp")
				: riderRenderer_->ChangeFrameAnimation("DownToUp_SwordBroken");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);
			muzzle_->GetTransform().SetLocalPosition(muzzleLocalPosition_Down_);
			swordCollisionBody_->Off();
		}
	);

	camelRiderStateManager_.CreateState(
		"Dead",
		std::bind(&CamelRider::RunInDead, this),
		[this](const StateInfo& _info)->void
		{
			if (0 == GameEngineRandom::mainRandom_.GetRandomInt(0, 1))
			{
				GameEngineSound::SoundPlayOneshot("Rebel_Death1.mp3");
			}
			else
			{
				GameEngineSound::SoundPlayOneshot("Rebel_Death2.mp3");
			}

			camelRenderer_->ChangeFrameAnimation("Dead");
			riderRenderer_->ChangeFrameAnimation("Dead");
			riderArmRenderer_->Off();

			riderCollisionBody_->Off();
			swordCollisionBody_->Off();

			pointOfDeath_ = riderRenderPivot_->GetTransform().GetWorldPosition();

			riderRenderPivot_->GetTransform().PixLocalPositiveX();
		}
	);


	std::vector<std::pair<CamelRiderState, std::string_view>> camelRiderStateEntries(
		magic_enum::enum_entries<CamelRiderState>().begin(), magic_enum::enum_entries<CamelRiderState>().end()
	);

	size_t camelRiderStateCount = camelRiderStateEntries.size();
	allCamelRiderStates_.reserve(camelRiderStateCount);

	for (size_t i = 0; i < camelRiderStateCount; i++)
	{
		if (allCamelRiderStates_.end() != allCamelRiderStates_.find(camelRiderStateEntries[i].first))
		{
			MsgBoxAssertString(std::string(camelRiderStateEntries[i].second) + std::string(": 이미 존재하는 스테이트입니다."));
			return;
		}

		allCamelRiderStates_.insert(
			std::make_pair(
				camelRiderStateEntries[i].first, camelRiderStateEntries[i].second.data()
			)
		);
	}



	camelRiderStateManager_.ChangeState("Up_Idling");
}

void CamelRider::Update(float _deltaTime)
{
	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	UpdateDirection(_deltaTime);

	UpdateCamelRiderState(_deltaTime);

	MoveCamel(_deltaTime);

}

void CamelRider::End()
{
}

void CamelRider::TakeDamage(
	int _damage,
	GameEngineCollision* _soldierWeaponCollision,
	GameEngineCollision* _rebelCollision
)
{
	if (_rebelCollision == swordCollisionBody_
		&& static_cast<int>(ObjectOrder::Soldier) != _soldierWeaponCollision->GetActor()->GetOrder())
	{
		swordDurability_ -= _damage;

		if (0 < swordDurability_)
		{
			if (5 >= static_cast<int>(currentCamelRiderState_))
			{
				riderArmRenderer_->ChangeFrameAnimation("Up_TakingDamage", true);
			}
			else
			{
				riderArmRenderer_->ChangeFrameAnimation("Down_TakingDamage", true);
			}
		}
		else
		{
			if (CamelRiderState::UpToDown > currentCamelRiderState_)
			{
				currentCamelRiderState_ = CamelRiderState::Up_SwordBreaking;
			}
			else
			{
				currentCamelRiderState_ = CamelRiderState::Down_SwordBreaking;
			}
		}
	}
	else
	{
		hp_ -= _damage;
	}
}

void CamelRider::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if (true == upperLandingChecker_->IsOnGroundPixel()
			&& true == lowerLandingChecker_->IsOnGroundPixel()
			&& true == midLandingChecker_->IsOnGroundPixel())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (true == lowerLandingChecker_->IsOnGroundPixel()
			&& true == midLandingChecker_->IsOnGroundPixel())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (true == lowerLandingChecker_->IsOnGroundPixel())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else
		{
			if (false == isAirborne_)
			{
				isAirborne_ = true;
			}
		}
	}
}

void CamelRider::Fall(float _deltaTime)
{
	fallingSpeed_ += gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void CamelRider::UpdateDirection(float _deltaTime)
{
	float soldierWorldPosX = enemySoldier_->GetTransform().GetWorldPosition().x;

	float thisWorldPosX = this->GetTransform().GetWorldPosition().x;


	float temp1 = riderRenderPivot_->GetTransform().GetLocalScale().x;
	float temp2 = riderRenderPivot_->GetTransform().GetWorldScale().x;

	if (((soldierWorldPosX < thisWorldPosX && 0 > riderRenderPivot_->GetTransform().GetLocalScale().x)
		|| (soldierWorldPosX > thisWorldPosX && 0 < riderRenderPivot_->GetTransform().GetLocalScale().x))
		&& false == isRiderDirectionWrong_)
	{
		isRiderDirectionWrong_ = true;
	}
}

void CamelRider::React()
{
	if (true == isRiderDirectionWrong_)
	{
		if (static_cast<int>(CamelRiderState::UpToDown) <= static_cast<int>(currentCamelRiderState_))
		{
			currentCamelRiderState_ = CamelRiderState::DownToUp;
		}
		else if (CamelRiderState::DownToUp == currentCamelRiderState_)
		{
			currentCamelRiderState_ = CamelRiderState::Up_Idling;
		}
		else 
		{
			currentCamelRiderState_ = CamelRiderState::Up_Turning;
		}
		return;
	}


	if (static_cast<int>(CamelRiderState::Down_Idling) <= static_cast<int>(currentCamelRiderState_))
	{
		SelectNextState(8, 11, 1, CamelRiderState::Down_SwordBreaking);
	}
	else
	{
		if (this->GetTransform().GetWorldPosition().x 
			< this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x 
				- (GameEngineWindow::GetInst()->GetScale().HX() * 0.5f))
		{
			//낙타기수가 윈도우 좌측 끝을 넘어가지 못하게 한다.
			SelectNextState(1, 7, 3, CamelRiderState::Up_SwordBreaking, CamelRiderState::Up_Running);
		}
		else if (this->GetTransform().GetWorldPosition().x
			> (this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x 
				+ (GameEngineWindow::GetInst()->GetScale().HX() * 0.5f )))
		{
			//낙타기수가 윈도우 우측 끝을 넘어가면 돌아오게 한다.
			currentCamelRiderState_ = CamelRiderState::Up_Running;
		}
		else
		{
			SelectNextState(1, 7, 2, CamelRiderState::Up_SwordBreaking, CamelRiderState::Up_Turning);
		}
	}
}

void CamelRider::UpdateCamelRiderState(float _deltaTime)
{
	if (0 >= hp_)
	{
		currentCamelRiderState_ = CamelRiderState::Dead;
	}

	if (allCamelRiderStates_[currentCamelRiderState_] != camelRiderStateManager_.GetCurrentStateName())
	{
		camelRiderStateManager_.ChangeState(allCamelRiderStates_[currentCamelRiderState_]);
	}

	camelRiderStateManager_.Update(_deltaTime);
}

void CamelRider::MoveCamel(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void CamelRider::SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...)
{
	if (_maxStateIndex < _minStateIndex)
	{
		MsgBoxAssert("최대값이 최소값보다 작습니다. 값을 다시 지정하세요.");
		return;
	}

	if (_maxStateIndex >= static_cast<int>(CamelRiderState::Dead) || 0 > _maxStateIndex)
	{
		MsgBoxAssert("최대값이 스테이트 범위를 벗어났습니다. 값을 다시 지정하세요.");
		return;
	}

	int nextStateIndex
		= GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);

	if (0 < _exclusionCount)	//제외할 낙타기수 스테이트가 1개 이상일때만 진입.
	{
		va_list exclusionList;		//제외할 낙타기수 스테이트 리스트.
		va_start(exclusionList, _exclusionCount);	//exclusionList 초기화.
		CamelRiderState exclusion;		//제외할 낙타기수 스테이트.

		for (int i = 0; i < _exclusionCount; i++)
		{
			exclusion = va_arg(exclusionList, CamelRiderState);

			if (static_cast<CamelRiderState>(nextStateIndex) == exclusion)
			{
				//랜덤으로 뽑은 숫자가 다음 스테이트에서 제외할 낙타기수 스테이트 번호와 같다면 다시 뽑는다.
				nextStateIndex = GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);
				va_end(exclusionList);
				va_start(exclusionList, _exclusionCount);
				i = -1;
				continue;
			}
		}
		//랜덤으로 뽑은 숫자가 제외할 스테이트와 하나도 겹치지 않는다면 통과.
		va_end(exclusionList);
	}


	if (0 >= nextStateIndex)
	{
		nextStateIndex = 0;
	}

	currentCamelRiderState_ = static_cast<CamelRiderState>(nextStateIndex);
}

void CamelRider::Run()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_;
}

void CamelRider::RunBackward()
{
	movementFor1Second_ += float4::Left * GetTransform().GetWorldScale().x * runningSpeed_;
}

void CamelRider::RunInDead()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_;
	riderRenderPivot_->GetTransform().SetWorldPosition(pointOfDeath_);

	if (this->GetTransform().GetWorldPosition().x + 200.f
		< this->GetLevel()->GetMainCameraActorTransform().GetWorldPosition().x - GameEngineWindow::GetScale().HX())
	{
		this->Off();
	}
}

void CamelRider::Fire()
{
	RebelBullet* newRebelBullet = this->GetLevel()->CreateActor<RebelBullet>(
		ObjectOrder::RebelAttack_SolidBullet,
		"RebelBullet"
	);

	newRebelBullet->GetTransform().SetWorldPosition(this->muzzle_->GetTransform().GetWorldPosition());

	float riderRenderPivotWorldDirection = this->riderRenderPivot_->GetTransform().GetWorldScale().x;

	if (0.f > riderRenderPivotWorldDirection)
	{
		newRebelBullet->GetTransform().PixLocalPositiveX();
	}
	else
	{
		newRebelBullet->GetTransform().PixLocalNegativeX();
	}

	newRebelBullet->SetFiringDirection(riderRenderPivotWorldDirection);
}