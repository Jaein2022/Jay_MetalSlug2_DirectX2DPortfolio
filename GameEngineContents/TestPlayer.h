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
	void TakeDamage(int _rebelWeaponType);
public:


private:
	void CreatePlayerAnimations();
	void CreatePlayerStates();

	void UpdateInputInfo();	//키입력 업데이트.
	void ConvertInputToPlayerStates();	//키입력 정보를 각 부위별 스테이트로 변환.

	void UpdatePlayerState(float _deltaTime);	//플레이어 전체 상태 업데이트.

	void Run(float _deltaTime);			//달리기.
	void DuckStep(float _deltaTime);	//오리걸음.
	float GetSlope();			//Run(), DuckStep()함수로 지상 이동할때 경사각 구하는 함수.
	void Fall(float _deltaTime);	//추락.
	void CheckGround();			//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void ControlMuzzle();		//총구위치 변화.
	void Fire();				//총알 생성 및 발사시 필요한 정보 입력.


private:

	PlayerState currentPlayerState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isFalling_;		//false: 착지 상태. true: 공중에 떠 있는 상태.


	const int playerRendererLocalPosX_;
	const int playerRendererLocalPosY_;
	const int playerRendererLocalPosZ_;


	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;


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
	const float4 pistolForwardMuzzlePosition_;
	const float4 pistolUpwardMuzzlePosition_;
	const float4 pistolDownwardMuzzlePosition_;
	const float4 pistolDuckingMuzzlePosition_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float aimingAngle_;


	GameEngineCollision* playerCollision_;

	int causeOfDeath_;		//플레이어 사망원인. 0: 플레이어가 아직 살아있음. 



};

