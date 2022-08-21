#pragma once
#include "TestPlayer_Header.h"

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

	void FallAndLand(float _deltaTime);




private:
	PlayerState currentState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isJumping_;		//false: ���� ����. true: ���߿� �� �ִ� ����.


	const int playerRenderPivotX_;
	const int playerRenderPivotY_;
	const int playerRenderPivotZ_;


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
	bool isTestKeyDown_;

	GameEngineCollision* renderPivotPointer_;		//�����Ǻ� ǥ�ñ�.


	GameEngineCollision* upperMidfootPointer_;		//������ ���������� ǥ��.
	GameEngineCollision* playerWorldPosPointer_;	//������ ���������� ǥ��.
	GameEngineCollision* lowerMidfootPointer_;		//������ ���������� ǥ��.

	GameEngineCollision* frontPointer_;			//���� �ݸ���. ������� �̵����� ����.

	PixelColor magenta_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

