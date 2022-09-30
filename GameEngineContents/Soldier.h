#pragma once
#include "GlobalHeader.h"
#include "Soldier_Header.h"

class PixelIndicator;
class Indicator;
class Soldier : public GameEngineActor
{
	//이 클래스의 존재 이유: 
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

	SoldierState currentSoldierState_;

	SoldierWeaponType weapon_;
	SoldierLegState leg_;
	SoldierTopState top_;
	AimingDirection direction_;

	GameEngineStateManager soldierStateManager_;

	std::unordered_map<int, const std::pair<const SoldierState, const char*>> allSoldierStates_;	//모든 솔저(플레이어) 스테이트.
	//삽입, 순회, 삭제는 한번만 하고, 탐색은 런타임 내내 할 예정이므로 탐색 효율이 좋다고 알려진 비정렬 맵 사용.
	//사실 비정렬 맵을 써보고 싶어서 선택.

	bool isAirborne_;		//false: 착지 상태. true: 공중에 떠 있는 상태.


	const int soldierRendererLocalPosX_;
	const int soldierRendererLocalPosY_;
	const int soldierRendererLocalPosZ_;


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
	PixelIndicator* soldierWorldPosPointer_;	//액터의 월드포지션 표시.
	PixelIndicator* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	PixelIndicator* slopeChecker_;		
	PixelIndicator* ascendingSlopeChecker_;			
	PixelIndicator* flatSlopeChecker_;
	PixelIndicator* descendingSlopeChecker_;

	PixelColor currentGroundColor_;


	Indicator* muzzleIndicator_;		//총구위치 표시기.
	const float4 pistolForwardMuzzlePosition_;	//전방 보고있을때 총구위치 
	const float4 pistolUpwardMuzzlePosition_;	//위를 보고있을때총구위치 
	const float4 pistolDownwardMuzzlePosition_;	//아래를 보고있을때총구위치 
	const float4 pistolDuckingMuzzlePosition_;	//쪼그려 앉았을때 총구위치 

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float4 movementFor1Second_;		//1초 동안의 이동량. 델타타임과 플레이 속도는 MoveSoldier()함수에서 한번만 계산한다.

	float aimingAngle_;

	GameEngineCollision* soldierLifeCollisionBody_;
	GameEngineCollision* soldierCloseCombatCollisionBody_;

	const float4 soldierLifeCollisionBodyScale_Standing_;		//솔저 라이프 충돌체 서있을때 로컬 크기.
	const float4 soldierLifeCollisionBodyPosition_Standing_;	//솔저 라이프 충돌체 서있을때 로컬 위치.
	const float4 soldierLifeCollisionBodyScale_Ducking_;		//솔저 라이프 충돌체 앉아 있을때 로컬 크기.
	const float4 soldierLifeCollisionBodyPosition_Ducking_;		//솔저 라이프 충돌체 앉아 있을때 로컬 위치.

	const int meleeAttackDamage_;	//근접공격 데미지.
	bool isMeleeAttack1_;	//true: 근접공격 애니메이션1. false: 근접공격 애니메이션2.

	int causeOfDeath_;		//솔저 사망원인. 0: 솔저가 아직 살아있음. 
	bool isDamageProof_;	//true: 무적상태. 
	float remainedDamageProofDuration_;	//남은 무적 지속시간.
	const float damageProofTimeLimit_;	//무적 시간제한.
	const float flickeringPeriod_;//깜빡임 주기.

};

