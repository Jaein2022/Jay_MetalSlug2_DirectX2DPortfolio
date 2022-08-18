#pragma once

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

	void Move(float _deltaTime);	//�¿� Ʈ������ ��ȭ ��� �Լ�.
	void Jump(float _deltaTime);	//���� Ʈ������ ��ȭ ��� �Լ�.




private:
	GameEngineStateManager playerStateManager_;

	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;

	PlayerState currentState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	char movingDirection_;	//-1: �������� �̵�, 0: ����, 1: �������� �̵�.
	bool isJumping_;		//false: ���� ����. true: ���߿� �� �ִ� ����.
	bool isUpKeyPressed_;
	bool isDownKeyPressed_;
	bool isJumpKeyDown_;
	bool isAttackKeyDowned_;
	bool isSpecialKeyDowned_;

	float playerSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

