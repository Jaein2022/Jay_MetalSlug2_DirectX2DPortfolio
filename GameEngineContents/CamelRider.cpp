#include "PreCompile.h"
#include "CamelRider.h"

#include "PixelIndicator.h"

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








	camelRenderer_ = CreateComponent<GameEngineTextureRenderer>("CamelRenderer");
	camelRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	camelRenderer_->SetPivot(PivotMode::Center);
	camelRenderer_->GetTransform().SetLocalPosition(camelRendererLocalPos_);

	if (0 == GameEngineTexture::Find("Rebel_Camel.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_Camel.png", 10, 8);
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
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("UpToDown",
		FrameAnimation_Desc("Rebel_Camel.png", 30, 32, 0.1f, true)
	);	
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_Camel.png", 40, 45, 0.3f, true)
	);	
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("DownToUp",
		FrameAnimation_Desc("Rebel_Camel.png", 50, 52, 0.1f, true)
	);	
	
	
	camelRenderer_->CreateFrameAnimation_CutTexture("Dead",
		FrameAnimation_Desc("Rebel_Camel.png", 60, 71, 0.1f, true)
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
		FrameAnimation_Desc("Rebel_CamelRider.png", 30, 33, 0.1f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("UpToDown",
		FrameAnimation_Desc("Rebel_CamelRider.png", 40, 42, 0.5f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Idling",
		FrameAnimation_Desc("Rebel_CamelRider.png", 50, 53, 0.15f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("Down_Firing",
		FrameAnimation_Desc("Rebel_CamelRider.png", 60, 69, 0.1f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp",
		FrameAnimation_Desc("Rebel_CamelRider.png", 70, 72, 0.5f, true)
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
		FrameAnimation_Desc("Rebel_CamelRider.png", 110, 112, 0.5f, true)
	);	
	
	riderRenderer_->CreateFrameAnimation_CutTexture("DownToUp_SwordBroken",
		FrameAnimation_Desc("Rebel_CamelRider.png", 120, 122, 0.5f, true)
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
			(false == isSwordBroken_) ? camelRenderer_->ChangeFrameAnimation("UpToDown")
				: camelRenderer_->ChangeFrameAnimation("UpToDown_Broken");
			riderRenderer_->ChangeFrameAnimation("UpToDown");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

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
			//isSwordBroken_ = true;

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
			(false == isSwordBroken_) ? camelRenderer_->ChangeFrameAnimation("DownToUp_SwordBroken")
				: camelRenderer_->ChangeFrameAnimation("DownToUp");
			riderRenderer_->ChangeFrameAnimation("DownToUp");
			riderArmRenderer_->Off();

			riderRenderer_->GetTransform().SetLocalPosition(riderRendererLocalPos_Up_);
			riderArmRenderer_->GetTransform().SetLocalPosition(riderArmRendererLocalPos_Up_);

			riderCollisionBody_->GetTransform().SetLocalScale(riderCollisionBodyScale_Up_);
			riderCollisionBody_->GetTransform().SetLocalPosition(riderCollisionBodyPosition_Up_);

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

	GetDirection(_deltaTime);
	UpdateCamelRiderState(_deltaTime);

	MoveCamel(_deltaTime);
}

void CamelRider::End()
{
}

void CamelRider::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= camelRiderWorldPosPointer_->GetColorValue_UINT()))
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

				//if (currentArabianState_ == ArabianState::JumpingBackward)
				//{
				//	currentArabianState_ = ArabianState::Shuffling;
				//}
				//else
				//{
				//	currentArabianState_ = ArabianState::FallingToIdling;
				//}
			}
		}
		else if (groundColor_.color_ <= camelRiderWorldPosPointer_->GetColorValue_UINT()
			&& groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				//if (currentArabianState_ == ArabianState::JumpingBackward)
				//{
				//	currentArabianState_ = ArabianState::Shuffling;
				//}
				//else
				//{
				//	currentArabianState_ = ArabianState::FallingToIdling;
				//}
			}
		}
		else if (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;

				//if (currentArabianState_ == ArabianState::JumpingBackward)
				//{
				//	currentArabianState_ = ArabianState::Shuffling;
				//}
				//else
				//{
				//	currentArabianState_ = ArabianState::FallingToIdling;
				//}
			}
		}
		else
		{
			if (false == isAirborne_)
			{
				isAirborne_ = true;
				//currentArabianState_ = ArabianState::Falling;
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

void CamelRider::Run()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_;
}

void CamelRider::Fire()
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
				riderArmRenderer_->ChangeFrameAnimation("Up_TakingDamage");
			}
			else
			{
				riderArmRenderer_->ChangeFrameAnimation("Down_TakingDamage");
			}
		}
		else
		{
			if (5 >= static_cast<int>(currentCamelRiderState_))
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


