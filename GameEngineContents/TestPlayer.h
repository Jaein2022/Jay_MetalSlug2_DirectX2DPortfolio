#pragma once
#include "TestPlayer_Header.h"

class TestPixelIndicator;
class TestIndicator;
class TestBackground;
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


public:


private:
	void CreatePlayerAnimations();
	void CreatePlayerStates();

	void UpdateInputInfo();	//키입력 업데이트.
	void ConvertInputToPlayerStates();

	void UpdatePlayerState(float _deltaTime);	//플레이어 전체 상태 업데이트.

	void Run(float _deltaTime);
	void DuckStep(float _deltaTime);
	float CheckSlope();
	void Fall(float _deltaTime);
	void CheckFalling();
	void ControlMuzzle();
	void Fire();


private:
	PlayerState currentState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isJumping_;		//false: 착지 상태. true: 공중에 떠 있는 상태.


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


	TestIndicator* muzzleIndicator_;		//총구 표시기.
	const float4 pistolForwardMuzzlePosition_;
	const float4 pistolUpwardMuzzlePosition_;
	const float4 pistolDownwardMuzzlePosition_;
	const float4 pistolDuckingMuzzlePosition_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;
	float duckStepSpeed_;

	float aimingAngle_;

};

