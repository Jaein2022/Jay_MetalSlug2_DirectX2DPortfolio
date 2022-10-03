#include "PreCompile.h"
#include "CamelRider.h"
#include "PixelIndicator.h"
#include "Soldier.h"

CamelRider::CamelRider()
	: currentCamelRiderState_(CamelRiderState::Up_Idling),
	camelRenderer_(nullptr),
	riderRenderer_(nullptr),
	riderArmRenderer_(nullptr),
	camelRendererLocalPos_(0, 105, 0),
	riderRendererLocalPos_Up_(-60, 197, 1),
	riderRendererLocalPos_Down_(-60, 95, -2),
	riderArmRendererLocalPos_Up_(-7, 257, 2),
	riderArmRendererLocalPos_Down_(0, 125, -1),
	upperLandingChecker_(nullptr),
	camelRiderWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	fallingSpeed_(0.f),
	runningSpeed_(4.0f),
	movementFor1Second_(float4::Zero),
	isAirborne_(false),
	isEngaging_(false),
	isSwordBroken_(false),
	riderCollisionBody_(nullptr),
	swordCollisionBody_(nullptr),
	riderCollisionBodyScale_Up_(float4(90, 90, 10)),
	riderCollisionBodyPosition_Up_(float4(-55, 245, 10)),
	riderCollisionBodyScale_Down_(float4(110, 100, 10)),
	riderCollisionBodyPosition_Down_(float4(-65, 120, 10)),
	swordCollisionBodyScale_Up_(float4(40, 135, 10)),
	swordCollisionBodyPosition_Up_(float4(30, 255, 10)),
	swordCollisionBodyScale_Down_(float4(60, 130, 10)),
	swordCollisionBodyPosition_Down_(float4(40, 115, 10)),
	hp_(1),
	swordDuration_(3),
	enemySoldier_(nullptr),
	riderLocalDirection_(1)
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

	camelRiderWorldPosPointer_ = Indicator::CreateIndicator<PixelIndicator>(
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


	riderCollisionBody_ = CreateComponent<GameEngineCollision>("RiderCollisionBody");
	riderCollisionBody_->ChangeOrder(this->GetOrder());
	riderCollisionBody_->SetCollisionMode(CollisionMode::Single);
	riderCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
	riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);



	swordCollisionBody_ = CreateComponent<GameEngineCollision>("SwordCollisionBody");
	swordCollisionBody_->ChangeOrder(this->GetOrder());
	swordCollisionBody_->SetCollisionMode(CollisionMode::Single);
	swordCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	swordCollisionBody_->GetTransform().SetLocalScale(swordCollisionBodyScale_Up_);
	swordCollisionBody_->GetTransform().SetLocalPosition(swordCollisionBodyPosition_Up_);


	enemySoldier_ = this->GetLevel()->GetConvertedGroup<Soldier>(CollisionBodyOrder::Soldier).front();
	//��Ÿ��� ������ �ݵ�� ���� ���� ���ķ� �� ��.
	//����(�÷��̾�)�� �Ѹ� �̻����� �þ�� ���� �߻�.





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


	camelRenderer_->ChangeFrameAnimation("Down_Idling");





	riderRenderer_ = CreateComponent<GameEngineTextureRenderer>("RiderRenderer");
	riderRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	riderRenderer_->SetPivot(PivotMode::Center);
	riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);

	if (0 == GameEngineTexture::Find("Rebel_CamelRider.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_CamelRider.png", 10, 15);
	}
	riderRenderer_->SetTexture("Rebel_CamelRider.png");
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Entrance",
		FrameAnimation_Desc("Rebel_CamelRider.png", 0, 9, 0.1f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Idling",
		FrameAnimation_Desc("Rebel_CamelRider.png", 10, 13, 0.15f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Up_Firing",
		FrameAnimation_Desc("Rebel_CamelRider.png", 20, 29, 0.1f, true)
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
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_CamelRider.png", 50, 53, 0.15f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Firing",
		FrameAnimation_Desc("Rebel_CamelRider.png", 60, 69, 0.1f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp",
		FrameAnimation_Desc("Rebel_CamelRider.png", 70, 72, 0.075f, false)
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
	
	riderRenderer_->CreateFrameAnimation_CutTexture("UpToDown_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 110, 112, 0.075f, false)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 120, 122, 0.075f, false)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Dead",
		FrameAnimation_Desc("Rebel_CamelRider.png", 130, 142, 0.1f, false)
	);


	riderRenderer_->ChangeFrameAnimation("Up_Idling");





	riderArmRenderer_ = CreateComponent<GameEngineTextureRenderer>("RiderArmRenderer");
	riderArmRenderer_->GetTransform().SetLocalScale(150, 150, 1);
	riderArmRenderer_->SetPivot(PivotMode::Center);
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




	camelRiderStateManager_.CreateState(
		"Up_Entrance",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
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
		}
	);


	camelRiderStateManager_.CreateState(
		"Up_Idling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
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
			riderRenderer_->GetTransform().PixLocalNegativeX();
			riderArmRenderer_->GetTransform().PixLocalNegativeX();
			riderCollisionBody_->GetTransform().PixLocalNegativeX();
			swordCollisionBody_->GetTransform().PixLocalNegativeX();
		}
	);
		
		
		
		
		camelRiderStateManager_.CreateState(
		"Up_Firing",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
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
			camelRenderer_->ChangeFrameAnimation("Up_Idling");
			riderRenderer_->ChangeFrameAnimation("Up_SwordBreaking");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

			swordCollisionBody_->Off();
		}
	);


	camelRiderStateManager_.CreateState(
		"UpToDown",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			camelRenderer_->ChangeFrameAnimation("UpToDown");
			(false == isSwordBroken_) ? riderRenderer_->ChangeFrameAnimation("UpToDown")
				: riderRenderer_->ChangeFrameAnimation("UpToDown_SwordBroken");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);

			swordCollisionBody_->Off();
		}
	);


	camelRiderStateManager_.CreateState(
		"Down_Idling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
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
			camelRenderer_->ChangeFrameAnimation("Down_Idling");
			riderRenderer_->ChangeFrameAnimation("Down_SwordBreaking");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);
 
			swordCollisionBody_->Off();
		}
	);

	camelRiderStateManager_.CreateState(
		"DownToUp",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			camelRenderer_->ChangeFrameAnimation("DownToUp");
			(false == isSwordBroken_) ? riderRenderer_->ChangeFrameAnimation("DownToUp")
				: riderRenderer_->ChangeFrameAnimation("DownToUp_SwordBroken");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Down_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Down_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Down_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Down_);

			swordCollisionBody_->Off();
		}
	);

	camelRiderStateManager_.CreateState(
		"Dead",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			camelRenderer_->ChangeFrameAnimation("Dead");
			riderRenderer_->ChangeFrameAnimation("Dead");
			riderArmRenderer_->Off();

			riderCollisionBody_->Off();
			swordCollisionBody_->Off();

			this->Death(1.f);
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
			MsgBoxAssertString(std::string(camelRiderStateEntries[i].second) + std::string(": �̹� �����ϴ� ������Ʈ�Դϴ�."));
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
	//float4 temp1 = riderArmRenderer_->GetTransform().GetWorldPosition();
	//float4 temp2 = riderCollisionBody_->GetTransform().GetWorldPosition();
	//float4 temp3 = swordCollisionBody_->GetTransform().GetWorldPosition();

	//float4 temp4 = riderArmRenderer_->GetTransform().GetWorldScale();
	//float4 temp5 = riderCollisionBody_->GetTransform().GetWorldScale();
	//float4 temp6 = swordCollisionBody_->GetTransform().GetWorldScale();



	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	GetDirection(_deltaTime);

	if (true == GameEngineInput::GetInst()->IsDown("Test"))
	{
		if (CamelRiderState::Down_Idling == currentCamelRiderState_)
		{
			currentCamelRiderState_ = CamelRiderState::DownToUp;
		}
		else if (CamelRiderState::Up_Idling == currentCamelRiderState_)
		{
			currentCamelRiderState_ = CamelRiderState::UpToDown;
		}

		//currentCamelRiderState_ = CamelRiderState::Up_Turning;

	}

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
		&& static_cast<int>(CollisionBodyOrder::Soldier) != _soldierWeaponCollision->GetActor()->GetOrder())
	{
		swordDuration_ -= _damage;

		if (0 < swordDuration_)
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
		if ((groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= camelRiderWorldPosPointer_->GetColorValue_UINT()))
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (groundColor_.color_ <= camelRiderWorldPosPointer_->GetColorValue_UINT()
			&& groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
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

void CamelRider::GetDirection(float _deltaTime)
{
	float soldierWorldPosX = enemySoldier_->GetTransform().GetWorldPosition().x;

	float thisWorldPosX = this->GetTransform().GetWorldPosition().x;


	if (soldierWorldPosX < thisWorldPosX && 0 < riderRenderer_->GetTransform().GetWorldScale().x)
	{
		riderLocalDirection_ = 1;
	}
	else if (soldierWorldPosX > thisWorldPosX && 0 > riderRenderer_->GetTransform().GetWorldScale().x)
	{
		riderLocalDirection_ = -1;
	}
	else
	{
		riderLocalDirection_ = 0;
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
	if (_maxStateIndex <= _minStateIndex)
	{
		MsgBoxAssert("�ִ밪�� �ּҰ����� �۰ų� �����ϴ�. ���� �ٽ� �����ϼ���.");
		return;
	}

	if (_maxStateIndex >= static_cast<int>(CamelRiderState::Dead) || 1 > _maxStateIndex)
	{
		MsgBoxAssert("�ִ밪�� ������Ʈ ������ ������ϴ�. ���� �ٽ� �����ϼ���.");
		return;
	}

	int nextStateIndex
		= GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);

	if (0 < _exclusionCount)	//������ ��Ÿ���������Ʈ�� 1�� �̻��϶��� ����.
	{
		va_list exclusionList;		//������ ��Ÿ���������Ʈ ����Ʈ.
		va_start(exclusionList, _exclusionCount);	//exclusionList �ʱ�ȭ.
		CamelRiderState exclusion;		//������ ��Ÿ���������Ʈ.

		for (int i = 0; i < _exclusionCount; i++)
		{
			exclusion = va_arg(exclusionList, CamelRiderState);

			if (static_cast<CamelRiderState>(nextStateIndex) == exclusion)
			{
				//�������� ���� ���ڰ� ���� ������Ʈ���� ������ ��Ÿ���������Ʈ ��ȣ�� ���ٸ� �ٽ� �̴´�.
				nextStateIndex = GameEngineRandom::mainRandom_.GetRandomInt(_minStateIndex, _maxStateIndex);
				va_end(exclusionList);
				va_start(exclusionList, _exclusionCount);
				i = -1;
				continue;
			}

			if (exclusion > CamelRiderState::Up_Idling)
			{
				MsgBoxAssert("���ܰ��� ��Ÿ��� ������Ʈ ������ ������ϴ�. ���� �ٽ� �����ϼ���.");
				return;
			}
		}
		//�������� ���� ���ڰ� ������ ������Ʈ�� �ϳ��� ��ġ�� �ʴ´ٸ� ���.
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

void CamelRider::Fire()
{
}