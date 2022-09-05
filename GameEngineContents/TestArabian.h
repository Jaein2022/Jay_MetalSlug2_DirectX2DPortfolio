#pragma once

enum class ArabianState
{
	Idling,
	Shuffling,
	PreparingToAttack,
	Running,
	Jumping, 
	Falling,
	FallingToIdling,
	ThrowingSword,
	MeleeAttack,
	Death
};


class TestIndicator;
class TestPixelIndicator;
class TestArabian: public GameEngineActor
{
	//이 클래스의 존재 이유: 적과의 교전, 적/플레이어 사망 기능 구현.
public:
	TestArabian();
	~TestArabian();

protected:
	TestArabian(const TestArabian& _other) = delete;
	TestArabian(TestArabian&& _other) noexcept = delete;

private:
	TestArabian& operator=(const TestArabian& _other) = delete;
	TestArabian& operator=(const TestArabian&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	void ReactToPlayerPosition();	
	void Shuffle(float _deltaTime);
	void UpdateArabianState(float _deltaTime);
	void Run(float _deltaTime);
	float GetSlope();
	void Fall(float _deltaTime);
	void CheckGround();
	void ThrowSword();



private:

	ArabianState currentArabianState_;

	bool isFalling_;		//false: 착지 상태. true: 공중에 떠 있는 상태.
	//bool isEngaging_;

	const int arabianRendererLocalPosX_;
	const int arabianRendererLocalPosY_;
	const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianCollision_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.

	TestIndicator* renderPivotPointer_;		//액터의 렌더피봇 표시.


	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* arabianWorldPosPointer_;	//액터의 월드포지션 표시.
	TestPixelIndicator* lowerLandingChecker_;

	const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	TestPixelIndicator* slopeChecker_;
	TestPixelIndicator* ascendingSlopeChecker_;
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;

	GameEngineStateManager arabianStateManager_;

	std::map<const ArabianState, const char*> allArabianStates_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float shufflingSpeed_;

	bool isLeft_;

	char localDirection_;		//-1: 액터 월드방향의 역방향. 0: 에러. 1: 액터 월드방향의 정방향.


	TestIndicator* releasePoint_;
	float releaseAngle_;
	float releaseVelocity_;

	const float recognitionDistance_;	//인식거리.
	const float engagementDistance_;	//교전거리.
	const float chargeDistance_;		//돌진거리.



};

