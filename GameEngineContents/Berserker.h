#pragma once
#include "Rebel.h"

enum class BerserkerState
{
	Waiting_SittingDown,
	Waiting_Standing,
	Idling,
	Running,
	Turning,
	MeleeAttack,
	Dead
};

class PixelIndicator;
class Soldier;
class Berserker : public Rebel
{
	//이 클래스의 존재 이유: 광전사.

	friend class DevelopmentTool;

public:
	Berserker();
	~Berserker();

protected:
	Berserker(const Berserker& _other) = delete;
	Berserker(Berserker&& _other) noexcept = delete;

private:
	Berserker& operator=(const Berserker& _other) = delete;
	Berserker& operator=(const Berserker&& _other) = delete;


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
	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();

	//추락 이동값 적용.
	void Fall(float _deltaTime);

	//플레이어와의 거리, 방향 판단.
	void UpdateDistanceAndDirection(float _deltaTime);

	//아라비안 스테이트 변환 및 업데이트.
	void UpdateBerserkerState(float _deltaTime);

	//모든 계산이 적용된 이동값을 실제로 적용하고 이동값을 리셋하는 함수.
	void MoveBerserker(float _deltaTime);


	//달리기.
	void Run();

	//경사값 구하기.
	float GetSlope(char _localDirection);

	//점프데스 애니메이션 중 움직임.
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);

	void MeleeAttack();


	void Flicker(		//깜빡임.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);

private:
	inline void SetBerserkerState(BerserkerState _newState)
	{
		currentBerserkerState_ = _newState;
	}

	void ResetRebelState() override
	{
		currentBerserkerState_ = BerserkerState::Idling;
	}


private:
	BerserkerState currentBerserkerState_;

	bool isAirborne_;		//true: 공중에 떠 있는 상태. false: 착지 상태.
	bool isEngaging_;		//true: 교전 중. false: 플레이어 인식 못함.

	const float4 berserkerRendererLocalPos_;

	GameEngineTextureRenderer* berserkerWholeRenderer_;
	GameEngineTextureRenderer* berserkerLegRenderer_;
	GameEngineTextureRenderer* berserkerTopRenderer_;

	GameEngineCollision* berserkerLifeCollisionBody_;
	GameEngineCollision* berserkerCloseCombatCollisionBody_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.

	PixelIndicator* upperLandingChecker_;
	PixelIndicator* midLandingChecker_;		//액터의 월드포지션도 표시.
	PixelIndicator* lowerLandingChecker_;


	GameEngineStateManager berserkerStateManager_;

	std::unordered_map<BerserkerState, const char*> allBerserkerStates_;
	//삽입, 순회, 삭제는 한번만 하고, 탐색은 런타임 내내 할 예정이므로 탐색 효율이 좋다고 하는 비정렬 맵 사용.
	//사실 비정렬 맵을 써보고 싶어서 선택.

	float fallingSpeed_;

	const float runningSpeed_;

	float4 movementFor1Second_;	//1초 동안의 이동량. 델타타임과 플레이 속도는 MoveBerserker()함수에서 한번만 계산한다.

	Soldier* enemySoldier_;

	float currentTurningDelay_;
	const float turningDelay_;
	bool isBerserkerDirectionWrong_;	//true: 광전사가 방향전환을 해야 한다.

	float horizontalDistance_;			//플레이어와의 수평 거리. 높이는 반영하지 않음.

	int hp_;

	const float flickeringPeriod_;//깜빡임 주기.
	float remainingPeriod_;
	bool flickeringSwitch_;

};

