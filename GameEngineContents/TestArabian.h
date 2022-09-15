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
	Dead
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

public:
	inline void TakeDamage(int _damage)
	{
		hp_ -= _damage;
	}

private:

	//발이 땅에 닿으면 추락을 멈추고, 발이 땅에 닿으면 추락 판정하는 함수. 
	void CheckGround();

	//추락 이동값 적용.
	void Fall(float _deltaTime);

	//플레이어 위치에 따라 스테이트 변화.
	void ReactToPlayerPosition();

	//아라비안 스테이트 변환 및 업데이트.
	void UpdateArabianState(float _deltaTime);

	//모든 계산이 적용된 이동값을 실제로 적용하고 이동값을 리셋하는 함수.
	void MoveArabian(float _deltaTime);

	void Shuffle();	//셔플중 이동.
	void Run();		//달리기.
	void SetSlopeCheckerDirection(char _localDirection);	//경사체커들 로컬방향 전환.
	float GetSlope(char _localDirection);
	void ThrowSword();		
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);	//점프데스 애니메이션 중 움직임.

private:

	ArabianState currentArabianState_;

	bool isFalling_;		//false: 착지 상태. true: 공중에 떠 있는 상태.
	//bool isEngaging_;

	const int arabianRendererLocalPosX_;
	const int arabianRendererLocalPosY_;
	const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianLifeCollisionBody_;
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

	char shuffleDirection_;		//셔플 중일때 이동방향.
	//-1: 액터 월드방향의 역방향. 0: 에러. 1: 액터 월드방향의 정방향.

	float4 movementFor1Second_;	//1초 동안의 이동량. 델타타임과 플레이 속도는 MoveArabian()함수에서 한번만 계산한다.


	TestIndicator* releasePoint_;
	float releaseAngle_;
	float releaseVelocity_;

	const float recognitionDistance_;	//인식거리.
	const float engagementDistance_;	//교전거리.
	const float chargeDistance_;		//돌진거리.

	int hp_;

};

