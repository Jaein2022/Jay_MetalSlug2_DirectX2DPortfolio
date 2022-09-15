#include "PreCompile.h"
#include "TestArabian.h"
#include "TestIndicator.h"
#include "TestPixelIndicator.h"
#include "TestLevel.h"
#include "TestSword.h"

TestArabian::TestArabian()
	: currentArabianState_(ArabianState::Idling),
	isFalling_(false),
	arabianRendererLocalPosX_(0),
	arabianRendererLocalPosY_(75),
	arabianRendererLocalPosZ_(0),
	arabianRenderer_(nullptr),
	arabianLifeCollisionBody_(nullptr),
	renderPivotPointer_(nullptr),
	upperLandingChecker_(nullptr),
	arabianWorldPosPointer_(nullptr),
	lowerLandingChecker_(nullptr),
	slopeCheckerLocalPosX_(10),				//0 금지!	
	slopeChecker_(nullptr),
	ascendingSlopeChecker_(nullptr),
	flatSlopeChecker_(nullptr),
	descendingSlopeChecker_(nullptr),
	initialJumpSpeed_(5.f),
	fallingSpeed_(0.f),
	runningSpeed_(3.f),
	shufflingSpeed_(0.5f),
	shuffleDirection_(1),
	movementFor1Second_(float4::Zero),
	releasePoint_(nullptr),
	releaseAngle_(60.f),
	releaseVelocity_(5.f),
	recognitionDistance_(800.f),
	engagementDistance_(500.f),
	chargeDistance_(150.f),
	hp_(10)
{
}

TestArabian::~TestArabian()
{
}

void TestArabian::Start()
{
	this->GetTransform().SetLocalScale(1, 1, 1);
	this->GetTransform().SetWorldScale(1, 1, 1);

	if (0 == slopeCheckerLocalPosX_)
	{
		MsgBoxAssert("slopeCheckerLocalPosX_에 0 넣지 말 것!");
		return;
	}

	arabianLifeCollisionBody_ = CreateComponent<GameEngineCollision>("ArabianCollision");
	arabianLifeCollisionBody_->ChangeOrder(this->GetOrder());
	arabianLifeCollisionBody_->SetDebugSetting(CollisionType::CT_AABB, float4(0.f, 1.f, 0.f, 0.5f));
	arabianLifeCollisionBody_->GetTransform().SetLocalScale(80, 150, 10);
	arabianLifeCollisionBody_->GetTransform().SetLocalPosition(0, 75, 10);



	arabianRenderer_ = CreateComponent<GameEngineTextureRenderer>("ArabianRenderer");
	arabianRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	arabianRenderer_->SetPivot(PivotMode::Center);
	arabianRenderer_->GetTransform().SetLocalPosition(
		arabianRendererLocalPosX_,
		arabianRendererLocalPosY_,
		arabianRendererLocalPosZ_
	);

	if (0 == GameEngineTexture::Find("Rebel_Arabian.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_Arabian.png", 10, 17);
	}
	arabianRenderer_->SetTexture("Rebel_Arabian.png");
	arabianRenderer_->CreateFrameAnimation_CutTexture("Idling",
		FrameAnimation_Desc("Rebel_Arabian.png", 0, 8, 0.15f, true)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("PreparingToAttack",
		FrameAnimation_Desc("Rebel_Arabian.png", 10, 15, 0.1f, false)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("Shuffling",
		FrameAnimation_Desc("Rebel_Arabian.png", 20, 25, 0.1f, true)
	);
	arabianRenderer_->AnimationBindEnd("Shuffling",
		[this](const FrameAnimation_Desc& _desc)->void {
			shuffleDirection_ = -shuffleDirection_;
		}
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Rebel_Arabian.png", 30, 41, 0.05f, true)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("Jumping",
		FrameAnimation_Desc("Rebel_Arabian.png", 50, 58, 0.1f, false)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("Falling",
		FrameAnimation_Desc("Rebel_Arabian.png", 58, 58, 1.f, true)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("FallingToIdling",
		FrameAnimation_Desc("Rebel_Arabian.png", 59, 63, 0.1f, false)
	);
	arabianRenderer_->AnimationBindEnd("FallingToIdling",
		[this](const FrameAnimation_Desc& _desc)->void {
			currentArabianState_ = ArabianState::Idling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("BackwardJump",
		FrameAnimation_Desc("Rebel_Arabian.png", 70, 73, 0.1f, false)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("ThrowingSword&Reloading",
		FrameAnimation_Desc("Rebel_Arabian.png", 80, 97, 0.1f, true)
	);
	arabianRenderer_->AnimationBindFrame("ThrowingSword&Reloading",
		[this](const FrameAnimation_Desc& _desc)->void {
			if (7 == _desc.curFrame_)
			{
				ThrowSword();
			}
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("MeleeAttack",
		FrameAnimation_Desc("Rebel_Arabian.png", 100, 107, 0.05f, true)
	);
	arabianRenderer_->AnimationBindFrame("MeleeAttack",
		[this](const FrameAnimation_Desc& _desc)->void {

		}
	);
	arabianRenderer_->AnimationBindEnd("MeleeAttack",
		[this](const FrameAnimation_Desc& _desc)->void {
			//currentArabianState_ = ArabianState::Shuffling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("JumpDeath",
		FrameAnimation_Desc("Rebel_Arabian.png", 110, 120, 0.075f, true)
	);
	arabianRenderer_->AnimationBindFrame("JumpDeath",
		std::bind(&TestArabian::MoveInJumpDeath, this, std::placeholders::_1)
	);
	arabianRenderer_->AnimationBindEnd("JumpDeath",
		[this](const FrameAnimation_Desc& _desc)->void {
			this->Death();
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Death1",
		FrameAnimation_Desc("Rebel_Arabian.png", 130, 141, 0.075f, true)
	);
	arabianRenderer_->AnimationBindEnd("Death1",
		[this](const FrameAnimation_Desc& _desc)->void {
			//this->Death();
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Death2",
		FrameAnimation_Desc("Rebel_Arabian.png", 150, 169, 0.075f, true)
	);
	arabianRenderer_->AnimationBindEnd("Death2",
		[this](const FrameAnimation_Desc& _desc)->void {
			//this->Death();
		}
	);


	arabianRenderer_->ChangeFrameAnimation("Idling");



	renderPivotPointer_ = TestIndicator::CreateIndicator<TestIndicator>(
		"RenderPivotPointer",
		this,
		float4::Cyan,
		float4(arabianRendererLocalPosX_, arabianRendererLocalPosY_, -5),
		float4(5, 5, 1)
		);

	upperLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"UpperLandingCheck",
		this,
		float4::Black,
		float4(0, 5, -5),
		float4(5, 5, 1)
		);

	arabianWorldPosPointer_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"PlayerWorldPosPointer",
		this,
		float4::Red,
		float4(0, 0, -5),
		float4(5, 5, 1)
		);

	lowerLandingChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"LowerLandingChecker",
		this,
		float4::Black,
		float4(0, -5, -5),
		float4(5, 5, 1)
		);

	slopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"SlopeChecker",
		this,
		float4::Blue,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
		);

	ascendingSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"AscendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 10, -5),
		float4(5, 5, 1)
		);

	flatSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"FlatSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, 0, -5),
		float4(5, 5, 1)
		);

	descendingSlopeChecker_ = TestIndicator::CreateIndicator<TestPixelIndicator>(
		"DescendingSlopeChecker",
		this,
		float4::Black,
		float4(slopeCheckerLocalPosX_, -10, -5),
		float4(5, 5, 1)
		);

	releasePoint_ = TestIndicator::CreateIndicator<TestIndicator>(
		"ReleasePoint",
		this,
		float4::Red,
		float4(100, 115, 10),
		float4(5, 5, 1)
	);


	arabianStateManager_.CreateState(
		"Idling",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("Idling");
			//arabianRenderer_->ChangeFrameAnimation("JumpDeath");
		}
	);
	arabianStateManager_.CreateState(
		"Shuffling",
		std::bind(&TestArabian::Shuffle, this),
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("Shuffling");
		},
		[this](const StateInfo& _info)->void {
			shuffleDirection_ = 1;
		}
	);	
	arabianStateManager_.CreateState(
		"PreparingToAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("PreparingToAttack");
		}
	);	
	arabianStateManager_.CreateState(
		"Running",
		std::bind(&TestArabian::Run, this),
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("Running");
		}
	);	
	arabianStateManager_.CreateState(
		"Jumping",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("Jumping");
		}
	);	
	arabianStateManager_.CreateState(
		"Falling",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("Falling");
		}
	);	
	arabianStateManager_.CreateState(
		"FallingToIdling",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("FallingToIdling");
		}
	);	
	arabianStateManager_.CreateState(
		"ThrowingSword",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("ThrowingSword&Reloading");
		}
	);	
	arabianStateManager_.CreateState(
		"MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("MeleeAttack");
		}
	);	
	arabianStateManager_.CreateState(
		"Dead",
		nullptr,
		[this](const StateInfo& _info)->void {
			arabianRenderer_->ChangeFrameAnimation("JumpDeath");
			arabianLifeCollisionBody_->Off();
		}
	);

	constexpr auto arabianStateEntries = magic_enum::enum_entries<ArabianState>();
	constexpr size_t arabianStateSize = arabianStateEntries.size();

	for (size_t i = 0; i < arabianStateSize; i++)
	{
		std::pair<std::map<const ArabianState, const char*>::iterator, bool> insertResult = allArabianStates_.insert(
			std::make_pair(
				arabianStateEntries[i].first, arabianStateEntries[i].second.data()
			)
		);

		if (false == insertResult.second)
		{
			MsgBoxAssertString(std::string(insertResult.first->second) + std::string(": 이미 존재하는 스테이트입니다."));
			return;
		}
	}

	arabianStateManager_.ChangeState(allArabianStates_[currentArabianState_]);
}

void TestArabian::Update(float _deltaTime)
{
	CheckGround();
	if (true == isFalling_)
	{
		Fall(_deltaTime);
	}
	ReactToPlayerPosition();

	UpdateArabianState(_deltaTime);
	MoveArabian(_deltaTime);
}

void TestArabian::End()
{
}

void TestArabian::CheckGround()
{
	if (0 <= fallingSpeed_)
	{
		if ((TestLevel::groundColor_.color_ <= upperLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
			&& (TestLevel::groundColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()))
		{
			//PixelColor magenta = PixelColor(255, 0, 255, 255);
			//magenta.color_;		//4294902015
			//PixelColor yellow = PixelColor(255, 255, 0, 255);
			//yellow.color_;		//4278255615
			//PixelColor cyan = PixelColor(0, 255, 255, 255);
			//cyan.color_;			//4294967040


			if (true == isFalling_)
			{
				this->GetTransform().SetWorldMove(float4::Up * 5.f);
				isFalling_ = false;
				fallingSpeed_ = 0.f;

				currentArabianState_ = ArabianState::FallingToIdling;
			}
		}
		else if (TestLevel::groundColor_.color_ <= arabianWorldPosPointer_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isFalling_)
			{
				isFalling_ = false;
				fallingSpeed_ = 0.f;

				currentArabianState_ = ArabianState::FallingToIdling;
			}
		}
		else if (TestLevel::groundColor_.color_ <= lowerLandingChecker_->GetColorValue_UINT())
		{
			if (true == isFalling_)
			{
				this->GetTransform().SetWorldMove(float4::Down * 5.f);
				isFalling_ = false;
				fallingSpeed_ = 0.f;

				currentArabianState_ = ArabianState::FallingToIdling;
			}
		}
		else
		{
			if (false == isFalling_)
			{
				isFalling_ = true;
				currentArabianState_ = ArabianState::Falling;
			}
		}
	}
}

void TestArabian::Fall(float _deltaTime)
{
	fallingSpeed_ += TestLevel::gravity_ * _deltaTime;

	movementFor1Second_ += float4::Down * fallingSpeed_;
}

void TestArabian::ReactToPlayerPosition()
{
	//float4 currectDistance 
	//	= this->GetTransform().GetWorldPosition() - this->GetLevel<TestLevel>()->GetPlayerWorldPosition();


	float playerWorldPosX = GetLevel<TestLevel>()->GetPlayerWorldPosition().x;
	//float playerWorldPosX = this->GetLevel()->GetGroup(ActorGroup::Player).front()->GetTransform().GetWorldPosition().x;
	float thisWorldPosX = this->GetTransform().GetWorldPosition().x; 
	float horizontalDistance = abs(thisWorldPosX - playerWorldPosX);

	if (playerWorldPosX < thisWorldPosX)
	{
		this->GetTransform().PixLocalNegativeX();
	}
	else
	{
		this->GetTransform().PixLocalPositiveX();
	}

	if (false == isFalling_)
	{
		if (chargeDistance_ > horizontalDistance)
		{
			currentArabianState_ = ArabianState::MeleeAttack;
		}
		else if (engagementDistance_ > horizontalDistance)
		{
			currentArabianState_ = ArabianState::ThrowingSword;
		}
		else if (recognitionDistance_ > horizontalDistance)
		{
			currentArabianState_ = ArabianState::Running;
		}
	}

	


}

void TestArabian::UpdateArabianState(float _deltaTime)
{
	if (0 >= hp_)
	{
		currentArabianState_ = ArabianState::Dead;
	}

	if (allArabianStates_[currentArabianState_] != arabianStateManager_.GetCurrentStateName())
	{
		arabianStateManager_.ChangeState(allArabianStates_[currentArabianState_]);
	}

	arabianStateManager_.Update(_deltaTime);
}

void TestArabian::MoveArabian(float _deltaTime)
{
	this->GetTransform().SetWorldMove(movementFor1Second_ * _deltaTime * TestLevel::playSpeed_);
	movementFor1Second_ = float4::Zero;
}

void TestArabian::Shuffle()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * shuffleDirection_ * shufflingSpeed_;

	movementFor1Second_ += float4::Up * GetSlope(shuffleDirection_) * shufflingSpeed_;
}

void TestArabian::Run()
{
	movementFor1Second_ += float4::Right * GetTransform().GetWorldScale().x * runningSpeed_ ;

	movementFor1Second_ += float4::Up * GetSlope(1) * runningSpeed_ ;
}

void TestArabian::SetSlopeCheckerDirection(char _localDirection)
{
	slopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		slopeChecker_->GetTransform().GetLocalPosition().IY(),
		slopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	ascendingSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		ascendingSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	flatSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		flatSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		flatSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
	descendingSlopeChecker_->GetTransform().SetLocalPosition(
		slopeCheckerLocalPosX_ * _localDirection,
		descendingSlopeChecker_->GetTransform().GetLocalPosition().IY(),
		descendingSlopeChecker_->GetTransform().GetLocalPosition().IZ()
	);
}

float TestArabian::GetSlope(char _localDirection)
{
	if (false == isFalling_)
	{
		SetSlopeCheckerDirection(_localDirection);

		int beginPosY = 0;
		int endPosY = 0;
		int slopeCheckPosY = 0;

		if (TestLevel::groundColor_.color_ <= ascendingSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_ * _localDirection,
				ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5,
				-5
			);

			if (TestLevel::groundColor_.color_ == slopeChecker_->GetColorValue_UINT())
			{
				return 0.f;
			}
			else
			{
				beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY() + 5;
				endPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			}
		}
		else if (TestLevel::groundColor_.color_ <= flatSlopeChecker_->GetColorValue_UINT()
			&& TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = ascendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else if (TestLevel::groundColor_.color_ <= descendingSlopeChecker_->GetColorValue_UINT())
		{
			beginPosY = flatSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
		}
		else
		{
			beginPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY();
			endPosY = descendingSlopeChecker_->GetTransform().GetLocalPosition().IY() - 5;
		}

		for (slopeCheckPosY = beginPosY; slopeCheckPosY >= endPosY; slopeCheckPosY--)
		{
			slopeChecker_->GetTransform().SetLocalPosition(
				slopeCheckerLocalPosX_ * _localDirection,
				slopeCheckPosY,
				-5
			);

			if (TestLevel::groundColor_.color_ <= slopeChecker_->GetColorValue_UINT())
			{
				break;
			}
		}

		return static_cast<float>(slopeCheckPosY) / static_cast<float>(slopeCheckerLocalPosX_);
		//액터가 수평방향으로 1픽셀 이동했을때 수직으로는 몇픽셀 이동해야하는지에 대한 값 반환.
	}
	else
	{
		MsgBoxAssert("점프중일때는 의미 없는 기능입니다.");
		return 0.f;
	}
}

void TestArabian::ThrowSword()
{
	TestSword* newSword = this->GetLevel<TestLevel>()->GetSword();
	newSword->GetTransform().SetWorldPosition(releasePoint_->GetTransform().GetWorldPosition());

	if (0 > this->GetTransform().GetWorldScale().x)
	{
		newSword->GetTransform().PixLocalPositiveX();
	}
	else
	{
		newSword->GetTransform().PixLocalNegativeX();
	}

	newSword->SetReleaseSpeed(
		abs((90.f - releaseAngle_) - (GetTransform().GetWorldScale().x * 90.f)),
		releaseVelocity_
	);
}

void TestArabian::MoveInJumpDeath(const FrameAnimation_Desc& _desc)
{
	if (8 <= _desc.curFrame_)
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 1.f / _desc.interval_;

		if (false == isFalling_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 1.f / _desc.interval_;
		}
	}
	else
	{
		movementFor1Second_ += float4::Right * -GetTransform().GetWorldScale().x * 5.f / _desc.interval_;

		if (false == isFalling_)
		{
			movementFor1Second_ += float4::Up * GetSlope(-1) * 5.f / _desc.interval_;
		}
	}
}

