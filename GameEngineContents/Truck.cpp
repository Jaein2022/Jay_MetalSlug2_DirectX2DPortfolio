#include "PreCompile.h"
#include "Truck.h"
#include "Indicator.h"
#include "PixelIndicator.h"

Truck::Truck()
	: currentTruckState_(TruckState::Deploying),
	upperLandingChecker_(nullptr),
	truckWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	movementFor1Second_(float4::Zero),
	isAirborne_(true),
	fallingSpeed_(0.f),
	truckRenderer_(nullptr),
	truckRendererLocalPos_(0, 160, 0),
	truckDurabilityCollisionBody_(nullptr),
	truckDurability_(15),
	truckSlopeCollisionBody_(nullptr),
	truckWreckageCollisionBody1_(nullptr),
	truckWreckageCollisionBody2_(nullptr),
	truckWreckageCollisionBody3_(nullptr),
	drivingSpeed_(2.5f)
{
}

Truck::~Truck()
{
}

void Truck::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	truckDurabilityCollisionBody_ = CreateComponent<GameEngineCollision>("TruckDurabilityCollisioinBody");
	truckDurabilityCollisionBody_->ChangeOrder(this->GetOrder());
	truckDurabilityCollisionBody_->SetCollisionMode(CollisionMode::Single);
	truckDurabilityCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	truckDurabilityCollisionBody_->GetTransform().SetLocalScale(460, 300, 10);
	truckDurabilityCollisionBody_->GetTransform().SetLocalPosition(0, 150, 10);




	truckWreckageCollisionBody1_ = CreateComponent<GameEngineCollision>("TruckWreckageCollisionBody1");
	truckWreckageCollisionBody1_->ChangeOrder(ObjectOrder::SteppableObject);
	truckWreckageCollisionBody1_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 1.f, 0.5f));
	truckWreckageCollisionBody1_->GetTransform().SetLocalScale(300, 110, 10);
	truckWreckageCollisionBody1_->GetTransform().SetLocalPosition(-75, 55, 10);

	truckWreckageCollisionBody2_ = CreateComponent<GameEngineCollision>("TruckWreckageCollisionBody2");
	truckWreckageCollisionBody2_->ChangeOrder(ObjectOrder::SteppableObject);
	truckWreckageCollisionBody2_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 1.f, 0.5f));
	truckWreckageCollisionBody2_->GetTransform().SetLocalScale(90, 180, 10);
	truckWreckageCollisionBody2_->GetTransform().SetLocalPosition(120, 90, 10);

	truckWreckageCollisionBody3_ = CreateComponent<GameEngineCollision>("TruckWreckageCollisionBody3");
	truckWreckageCollisionBody3_->ChangeOrder(ObjectOrder::SteppableObject);
	truckWreckageCollisionBody3_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 1.f, 0.5f));
	truckWreckageCollisionBody3_->GetTransform().SetLocalScale(65, 120, 10);
	truckWreckageCollisionBody3_->GetTransform().SetLocalPosition(200, 60, 10);



	upperLandingChecker_ = Indicator::CreateIndicator<PixelIndicator>(
		"UpperLandingChecker",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
	);


	truckWorldPosPointer_ = Indicator::CreateIndicator<PixelIndicator>(
		"TruckWorldPosPointer",
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



	if (0 == GameEngineTexture::Find("TroopTruck.png")->GetCutCount())
	{
		GameEngineTexture::Cut("TroopTruck.png", 10, 5);
	}

	truckRenderer_ = CreateComponent<GameEngineTextureRenderer>("TruckRenderer");
	truckRenderer_->SetPivot(PivotMode::Center);
	truckRenderer_->GetTransform().SetLocalScale(800, 800, 1);
	truckRenderer_->GetTransform().SetLocalPosition(truckRendererLocalPos_);
	truckRenderer_->SetTexture("TroopTruck.png");
	truckRenderer_->CreateFrameAnimation_CutTexture("Idling",
		FrameAnimation_Desc("TroopTruck.png", 0, 0, 0.5f, true)
	);

	truckRenderer_->CreateFrameAnimation_CutTexture("Driving",
		FrameAnimation_Desc("TroopTruck.png", 10, 13, 0.1f, true)
	);

	truckRenderer_->CreateFrameAnimation_CutTexture("Deploying",
		FrameAnimation_Desc("TroopTruck.png", 20, 31, 0.1f, false)
	);

	truckRenderer_->CreateFrameAnimation_CutTexture("Destroyed",
		FrameAnimation_Desc("TroopTruck.png", 40, 43, 0.1f, true)
	);

	truckRenderer_->ChangeFrameAnimation("Idling");



	truckStateManager_.CreateState(
		"Idling",
		nullptr
	);

	truckStateManager_.CreateState(
		"Driving",
		nullptr
	);

	truckStateManager_.CreateState(
		"Deploying",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			truckRenderer_->ChangeFrameAnimation("Deploying");
		}
	);

	truckStateManager_.CreateState(
		"Destroyed",
		nullptr,
		[this](const StateInfo& _info)->void
		{
			truckRenderer_->ChangeFrameAnimation("Destroyed");
			truckDurabilityCollisionBody_->Off();
		}
	);


	std::vector<std::pair<TruckState, std::string_view>> truckStateEntries(
		magic_enum::enum_entries<TruckState>().begin(), magic_enum::enum_entries<TruckState>().end()
	);

	size_t truckStateCount = truckStateEntries.size();
	allTruckStates_.reserve(truckStateCount);

	for (size_t i = 0; i < truckStateCount; i++)
	{
		if (allTruckStates_.end() != allTruckStates_.find(truckStateEntries[i].first))
		{
			MsgBoxAssertString(std::string(truckStateEntries[i].second) + std::string(": �̹� �����ϴ� ������Ʈ�Դϴ�."));
			return;
		}

		allTruckStates_.insert(
			std::make_pair(
				truckStateEntries[i].first, truckStateEntries[i].second.data()
			)
		);
	}

	truckStateManager_.ChangeState(allTruckStates_[currentTruckState_]);


}

void Truck::Update(float _deltaTime)
{
	CheckGround();
	if (true == isAirborne_)
	{
		Fall(_deltaTime);
	}

	UpdateTruckState(_deltaTime);

	MoveTruck(_deltaTime);


}

void Truck::End()
{
}

void Truck::TakeDamage(
	int _damage,
	GameEngineCollision* _soldierWeaponCollision,
	GameEngineCollision* _rebelCollision
)
{
	if (0 < truckDurability_)
	{
		truckDurability_ -= _damage;
	}





}

void Truck::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (groundColor_.color_ <= truckWorldPosPointer_->GetColorValue_UINT()))
		{
			if (true == isAirborne_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isAirborne_ = false;
				fallingSpeed_ = 0.f;
			}
		}
		else if (groundColor_.color_ <= truckWorldPosPointer_->GetColorValue_UINT()
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

void Truck::Fall(float _deltaTime)
{
	fallingSpeed_ += gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void Truck::Drive()
{
	movementFor1Second_ += float4::Left * drivingSpeed_;
}

void Truck::UpdateTruckState(float _deltaTime)
{
	if (0 >= truckDurability_)
	{
		currentTruckState_ = TruckState::Destroyed;
	}

	if (allTruckStates_[currentTruckState_] != truckStateManager_.GetCurrentStateName())
	{
		truckStateManager_.ChangeState(allTruckStates_[currentTruckState_]);
	}

	truckStateManager_.Update(_deltaTime);
}

void Truck::MoveTruck(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * playSpeed_);
	movementFor1Second_ = float4::Zero;
}