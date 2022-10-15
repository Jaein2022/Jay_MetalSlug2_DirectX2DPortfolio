#pragma once
#include "Rebel.h"



enum class ArabianState
{
	Shuffling,
	PreparingToAttack,
	JumpingBackward,
	Running,
	ThrowingSword,
	MeleeAttack,
	Jumping,
	Turning,
	Falling,
	FallingToWaiting,
	Waiting,
	Dead
};

class Indicator;
class PixelIndicator;
class Soldier;
class Arabian : public Rebel
{
	//이 클래스의 존재 이유: 적과의 교전, 적/플레이어 사망 기능 구현.

	friend class Mission1;
public:
	Arabian();
	~Arabian();

protected:
	Arabian(const Arabian& _other) = delete;
	Arabian(Arabian&& _other) noexcept = delete;

private:
	Arabian& operator=(const Arabian& _other) = delete;
	Arabian& operator=(const Arabian&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	void TakeDamage(
		int _damage,
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) override;


private:
	void CreateArabianAnimations();
	void CreateArabianStates();

	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();

	//추락 이동값 적용.
	void Fall(float _deltaTime);

	//플레이어와의 거리, 방향 판단.
	void UpdateDistanceAndDirection(float _deltaTime);

	void ReactInShuffling();

	//아라비안 스테이트 변환 및 업데이트.
	void UpdateArabianState(float _deltaTime);

	//모든 계산이 적용된 이동값을 실제로 적용하고 이동값을 리셋하는 함수.
	void MoveArabian(float _deltaTime);

	//반란군 아라비안의 다음 스테이트를 선택하는 함수. 
	//_minStateIndex와 _maxStateIndex는 정수를 넣어야 하지만 가변인자로 들어가는 제외대상은 아라비안스테이트로 넣어야 한다.
	//_maxStateIndex는 1보다 큰 정수를 넣어야 하지만 _minStateIndex는 0이나 음수를 넣을 수 있고,
	// 음수값이 커질수록 0번인 ArabianState::Shuffling이 나올 확률이 올라가서 그만큼 공격 빈도와 난이도가 줄어든다.
	void SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...);

	//셔플중 이동.
	void Shuffle();	

	//달리기.
	void Run();		

	//점프.
	void Jump(const FrameAnimation_Desc& _desc);

	//경사체커들 로컬방향 전환.
	void SetSlopeCheckerDirection(char _localDirection);	

	//경사값 구하기.
	float GetSlope(char _localDirection);

	//검 투척.
	void ThrowSword();		

	//점프데스 애니메이션 중 움직임.
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);	

	void MeleeAttack();

	//백점프 애니메이션 중 움직임.
	void JumpBackWard();

	void Flicker(		//깜빡임.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);


private:
	inline void SetRecognitionDistance(float _distance)
	{
		recognitionDistance_ = _distance;
	}

private:

	ArabianState currentArabianState_;

	bool isAirborne_;		//true: 공중에 떠 있는 상태. false: 착지 상태.
	bool isEngaging_;		//true: 교전 중. false: 플레이어 인식 못함.

	const float4 arabianRendererLocalPos_;

	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianLifeCollisionBody_;
	GameEngineCollision* arabianCloseCombatCollisionBody_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.


	PixelIndicator* upperLandingChecker_;
	PixelIndicator* midLandingChecker_;		//액터의 월드포지션도 표시.
	PixelIndicator* lowerLandingChecker_;

	const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	PixelIndicator* slopeChecker_;
	PixelIndicator* ascendingSlopeChecker_;
	PixelIndicator* flatSlopeChecker_;
	PixelIndicator* descendingSlopeChecker_;

	PixelColor currentSteppingPixelColor_;


	GameEngineStateManager arabianStateManager_;

	std::unordered_map<ArabianState, const char*> allArabianStates_;
	//삽입, 순회, 삭제는 한번만 하고, 탐색은 런타임 내내 할 예정이므로 탐색 효율이 좋다고 하는 비정렬 맵 사용.
	//사실 비정렬 맵을 써보고 싶어서 선택.

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;

	float shufflingSpeed_;

	char shuffleDirection_;		//셔플 중일때 이동방향.
	//-1: 액터 월드방향의 역방향. 0: 방향전환 필요없음. 1: 액터 월드방향의 정방향.

	float4 movementFor1Second_;	//1초 동안의 이동량. 델타타임과 플레이 속도는 MoveArabian()함수에서 한번만 계산한다.

	Soldier* enemySoldier_;

	float currentTurningDelay_;
	const float turningDelay_;
	bool isArabiansDirectionWrong_;	//true: 아라비안이 방향전환을 해야 한다.

	Indicator* releasePoint_;		//검 투척 지점. 플레이어의 머즐과 같은 역할.
	float releaseAngle_;			//검 투척 각도.
	float releaseVelocity_;			//검 투척 속도.

	float horizontalDistance_;			//플레이어와의 수평 거리. 높이는 반영하지 않음.
	float recognitionDistance_;	//인식거리.
	const float engagementDistance_;	//교전거리.
	const float chargeDistance_;		//돌진거리.

	int hp_;

	const float flickeringPeriod_;//깜빡임 주기.
	float remainingPeriod_;
	bool flickeringSwitch_;

};

