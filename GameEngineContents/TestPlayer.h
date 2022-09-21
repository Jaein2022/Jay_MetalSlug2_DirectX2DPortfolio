#pragma once
#include "TestPlayer_Header.h"

class TestPixelIndicator;
class TestIndicator;
class TestPlayer : public GameEngineActor
{
	//이 클래스의 존재 이유: 
public:
	TestPlayer();
	~TestPlayer();

protected:
	TestPlayer(const TestPlayer& _other) = delete;
	TestPlayer(TestPlayer&& _other) noexcept = delete;

private:
	TestPlayer& operator=(const TestPlayer& _other) = delete;
	TestPlayer& operator=(const TestPlayer&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	//void TakeWeapon(int _weaponType);
	//void ReleasePrisoner(int _prisonerType);
	void TakeDamage(int _rebelWeaponType);	//데미지 받는 함수. 플레이어는 무적 상태가 아니면 한방에 죽으므로 뭐로 맞았는지 정보를 받는다.



private:
	void CreatePlayerAnimations();
	void CreatePlayerStates();

	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();			

	//추락 이동값 적용.
	void Fall(float _deltaTime);	

	//키입력 정보 업데이트.
	void UpdateInputInfo();	

	//키입력 정보를 각 부위별 스테이트로 변환.
	void ConvertInputToPlayerStates();	

	//총구위치 변화.
	void ControlMuzzle();		

	//플레이어 전체 상태 업데이트.
	void UpdatePlayerState(float _deltaTime);	

	//모든 계산이 적용된 이동값을 실제로 적용하고 이동값을 리셋하는 함수.
	void MovePlayer(float _deltaTime);

	void Run();			//달리기.
	void DuckStep();	//오리걸음.
	float GetSlope();	//Run(), DuckStep()함수로 지상 이동할때 경사각 구하는 함수.
	void Fire();		//총알 생성 및 발사시 필요한 정보 입력.
	void MeleeAttack();	//근접공격.
	void Flicker(		//깜빡임.
		float _deltaTime,
		const float4& _plusColor
	);

private:

	PlayerState currentPlayerState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isAirborne_;		//false: 착지 상태. true: 공중에 떠 있는 상태.


	const int playerRendererLocalPosX_;
	const int playerRendererLocalPosY_;
	const int playerRendererLocalPosZ_;


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

	//TestIndicator* renderPivotPointer_;		//플레이어의 렌더피봇 표시.


	TestPixelIndicator* upperLandingChecker_;		
	TestPixelIndicator* playerWorldPosPointer_;	//액터의 월드포지션 표시.
	TestPixelIndicator* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	TestPixelIndicator* slopeChecker_;		
	TestPixelIndicator* ascendingSlopeChecker_;			
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;


	TestIndicator* muzzleIndicator_;		//총구위치 표시기.
	const float4 pistolForwardMuzzlePosition_;	//전방 보고있을때 총구위치 
	const float4 pistolUpwardMuzzlePosition_;	//위를 보고있을때총구위치 
	const float4 pistolDownwardMuzzlePosition_;	//아래를 보고있을때총구위치 
	const float4 pistolDuckingMuzzlePosition_;	//쪼그려 앉았을때 총구위치 

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float4 movementFor1Second_;		//1초 동안의 이동량. 델타타임과 플레이 속도는 MovePlayer()함수에서 한번만 계산한다.

	float aimingAngle_;

	GameEngineCollision* playerLifeCollisionBody_;
	GameEngineCollision* playerCloseCombatCollisionBody_;

	const float4 playerLifeCollisionBodyScale_Standing_;	//플레이어 라이프 충돌체 서있을때 로컬 크기.
	const float4 playerLifeCollisionBodyPosition_Standing_;	//플레이어 라이프 충돌체 서있을때 로컬 위치.
	const float4 playerLifeCollisionBodyScale_Ducking_;		//플레이어 라이프 충돌체 앉아 있을때 로컬 크기.
	const float4 playerLifeCollisionBodyPosition_Ducking_;	//플레이어 라이프 충돌체 앉아 있을때 로컬 위치.

	const int meleeAttackDamage_;	//근접공격 데미지.
	bool isMeleeAttack1_;	//true: 근접공격 애니메이션1. false: 근접공격 애니메이션2.

	int causeOfDeath_;		//플레이어 사망원인. 0: 플레이어가 아직 살아있음. 
	bool isDamageProof_;	//true: 무적상태. 
	const float flickeringPeriod_;//깜빡임 주기.

};

