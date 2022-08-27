#pragma once
#include "TestPlayer_Header.h"

class TestPointer;
class TestBackground;
class TestPlayer : public GameEngineActor
{
	//�� Ŭ������ ���� ����: 
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

	void UpdateInputInfo();	//Ű�Է� ������Ʈ.
	void ConvertInputToPlayerStates();

	void UpdatePlayerState(float _deltaTime);	//�÷��̾� ��ü ���� ������Ʈ.

	void Run(float _deltaTime);
	float CheckSlope();
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

	bool isJumping_;		//false: ���� ����. true: ���߿� �� �ִ� ����.


	const int playerRendererLocalPosX_;
	const int playerRendererLocalPosY_;
	const int playerRendererLocalPosZ_;


	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;


	char horizontalInputValue_;	//-1: ���� �Է�, 0: �߸�, 1: ���� �Է�.
	char verticalInputValue_;	//-1: �ϴ� �Է�, 0, �߸�, 1: ��� �Է�.
	bool isJumpKeyDown_;
	bool isAttackKeyDown_;
	bool isSpecialKeyDown_;

	TestPointer* renderPivotPointer_;		//�����Ǻ� ǥ��.


	TestPointer* upperLandingChecker_;		
	TestPointer* playerWorldPosPointer_;	//������ ���������� ǥ��.
	TestPointer* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_Int_;		//���üĿ���� �������� int X��.
	const float slopeCheckerLocalPosX_Float_;	//���üĿ���� �������� float X��.
	TestPointer* slopeChecker_;		
	TestPointer* ascendingSlopeChecker_;			
	TestPointer* flatSlopeChecker_;
	TestPointer* descendingSlopeChecker_;


	const PixelColor magenta_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

