#pragma once
#include "TestPlayer_Header.h"

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
	void Fall(float _deltaTime);
	void CheckFalling();




private:
	PlayerState currentState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isJumping_;		//false: 착지 상태. true: 공중에 떠 있는 상태.


	const int playerRenderPivotX_;
	const int playerRenderPivotY_;
	const int playerRenderPivotZ_;


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

	TestIndicator* renderPivotPointer_;		//렌더피봇 표시.


	TestIndicator* upperLandingChecker_;		
	TestIndicator* playerWorldPosPointer_;	//액터의 월드포지션 표시.
	TestIndicator* lowerLandingChecker_;		

	TestIndicator* slopeChecker_;			//전방 콜리전. 경사지형 이동각도 판정.

	PixelColor magenta_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

