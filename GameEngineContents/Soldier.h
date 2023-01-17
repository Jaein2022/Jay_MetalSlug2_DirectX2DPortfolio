#pragma once
#include "GlobalHeader.h"
#include "Soldier_Header.h"

class PixelIndicator;
class Indicator;
class Soldier : public GameEngineActor
{
	//이 클래스의 존재 이유: 플레이어.

	friend class Mission1;

public:
	Soldier();
	~Soldier();

protected:
	Soldier(const Soldier& _other) = delete;
	Soldier(Soldier&& _other) noexcept = delete;

private:
	Soldier& operator=(const Soldier& _other) = delete;
	Soldier& operator=(const Soldier&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	//void TakeWeapon(int _weaponType);
	//void ReleasePrisoner(int _prisonerType);
	void TakeDamage(int _rebelWeaponType);	//데미지 받는 함수. 플레이어는 무적 상태가 아니면 한방에 죽으므로 뭐로 맞았는지 정보를 받는다.
	//스스로는 호출 금지.


private:
	void CreateSoldierAnimations();
	void CreateSoldierStates();

	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();			

	//추락 이동값 적용.
	void Fall(float _deltaTime);	

	//키입력 정보 업데이트.
	void UpdateInputInfo();	

	//키입력 정보를 각 부위별 스테이트로 변환.
	void ConvertInputToSoldierStates();	

	//총구위치 변화.
	void ControlMuzzle();		

	//플레이어 전체 상태 업데이트.
	void UpdateSoldierState(float _deltaTime);	

	//모든 계산이 적용된 이동값을 최종적으로 적용하고 이동값을 리셋하는 함수.
	void MoveSoldier(float _deltaTime);

	void Run();			//달리기.
	void DuckStep();	//오리걸음.
	float GetSlope();	//Run(), DuckStep()함수로 지상 이동할때 경사각 구하는 함수.
	void Fire();		//총알 생성 및 발사시 필요한 정보 입력.
	void MeleeAttack();	//근접공격.
	void Flicker(		//깜빡임.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);

private:
	inline void SetSoldierWorldPosLimit(float _limit)
	{
		soldierWorldPosLimit_X_ = _limit;
	}


private:

	SoldierState currentSoldierState_;

	SoldierWeaponType weapon_;
	SoldierLegState leg_;
	SoldierTopState top_;
	AimingDirection direction_;

	GameEngineStateManager soldierStateManager_;

	std::unordered_map<int, const std::pair<const SoldierState, const char*>> allSoldierStates_;	//모든 솔저(플레이어) 스테이트.
	//삽입, 순회, 삭제는 한번만 하고, 탐색은 런타임 내내 할 예정이므로 탐색 효율이 좋다고 알려진 비정렬 맵 사용.
	//사실 비정렬 맵을 써보고 싶어서 선택.

	//비정렬 맵(Unordered Map): 들어오는 키값을 해시함수를 거쳐서 나온 숫자로 바꿔서, 
	// 그 값을 인덱스로 하는 배열 내 원소로 데이터를 저장하는 방식의 컨테이너. 
	//장점으로는 검색 속도가 '대체로' 다른 컨테이너들보다 빠르다고 한다(항상 그렇진 않음).
	//단점으로는 이름대로 키값 정렬이 되지 않고, 순회 효율이 떨어지며, 해시 충돌이 많이 발생할 수록 검색 효율이 떨어져서,
	// 최악의 경우 선형 시간복잡도를 보이게 된다고 한다.

	//해시 충돌: 다른 입력값을 해시 함수에 넣었는데 같은 결과값이 나와서 두 입력값을 구분할 수 없게 되는 현상.
	// 해시 충돌 확률이 낮을 수록 더 좋은 해시 함수라고 한다.

	bool isAirborne_;		//false: 착지 상태. true: 공중에 떠 있는 상태.

	const float4 soldierRendererLocalPos_;

	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;
	GameEngineTextureRenderer* redeployingRenderer_;
	std::list<GameEngineTextureRenderer*> allTextureRenderers_;


	char horizontalInputValue_;	//-1: 좌측 입력, 0: 중립, 1: 우측 입력.
	char verticalInputValue_;	//-1: 하단 입력, 0, 중립, 1: 상단 입력.
	bool isJumpKeyDown_;
	bool isAttackKeyDown_;
	bool isSpecialKeyDown_;


	PixelIndicator* upperLandingChecker_;		
	PixelIndicator* midLandingChecker_;		//액터의 월드포지션도 표시.
	PixelIndicator* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	PixelIndicator* slopeChecker_;		
	PixelIndicator* ascendingSlopeChecker_;			
	PixelIndicator* flatSlopeChecker_;
	PixelIndicator* descendingSlopeChecker_;


	Indicator* muzzle_;		//총구.
	const float4 pistolForwardMuzzlePosition_;	//전방 보고있을때 총구위치 
	const float4 pistolUpwardMuzzlePosition_;	//위를 보고있을때 총구위치 
	const float4 pistolDownwardMuzzlePosition_;	//아래를 보고있을때 총구위치 
	const float4 pistolDuckingMuzzlePosition_;	//쪼그려 앉았을때 총구위치 

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float4 movementFor1Second_;		//1초 동안의 이동량. 델타타임과 플레이 속도는 MoveSoldier()함수에서 한번만 계산한다.

	float aimingAngle_;

	GameEngineCollision* soldierLifeCollisionBody_;
	GameEngineCollision* soldierCloseCombatCollisionBody_;
	
	GameEngineCollision* frontCheckCollisionBody_;	//밟을 수 있는 충돌체 감지용 충돌체.
	GameEngineCollision* lowerLandingCheckCollisionBody_;	//밟을 수 있는 충돌체 감지용 충돌체.
	GameEngineCollision* upperLandingCheckCollisionBody_;	//밟을 수 있는 충돌체 감지용 충돌체.


	const float4 soldierLifeCollisionBodyScale_Standing_;		//솔저 라이프 충돌체 서있을때 로컬 크기.
	const float4 soldierLifeCollisionBodyPosition_Standing_;	//솔저 라이프 충돌체 서있을때 로컬 위치.
	const float4 soldierLifeCollisionBodyScale_Ducking_;		//솔저 라이프 충돌체 앉아 있을때 로컬 크기.
	const float4 soldierLifeCollisionBodyPosition_Ducking_;		//솔저 라이프 충돌체 앉아 있을때 로컬 위치.

	const int meleeAttackDamage_;	//근접공격 데미지.
	bool isMeleeAttack1_;	//true: 근접공격 애니메이션1. false: 근접공격 애니메이션2.

	int causeOfDeath_;		//솔저 사망원인. 0: 솔저가 아직 살아있음. 
	bool isDamageProof_;	//true: 무적상태. 
	float remainingDamageProofTime_;	//남은 무적 지속시간.
	const float damageProofTimeLimit_;	//무적 시간제한.
	const float flickeringPeriod_;		//깜빡임 주기.
	float remainingPeriod_;
	bool flickeringSwitch_;

	GameEngineTextureRenderer* player1SignRenderer_;

	float soldierWorldPosLimit_X_;		//미션 차원에서 설정하는 가로 이동 제한
};

