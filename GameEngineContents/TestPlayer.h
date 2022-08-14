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
	void UpdateInputInfo(float _deltaTime);	//Ű�Է� ������Ʈ.
	//void UpdateContactObject();	//�÷��̾ ������ ������Ʈ(������, NPC, �� ���� ��) ���� ������Ʈ.
	void UpdatePlayerState();	//�÷��̾� ��ü ���� ������Ʈ.

	//void Move(float _deltaTime);	//�¿� Ʈ������ ��ȭ ��� �Լ�.
	//void Jump(float _deltaTime);	//���� Ʈ������ ��ȭ ��� �Լ�.
	//void Fire();					//�÷��̾� ���ذ����� �´� ���Ÿ����� ����ü ���� �߻� �Լ�.
	//void ThrowGrenade();			//����ź ������Ʈ ���� �߻� �Լ�.	
	//void MeleeAttack();			//�������� �Լ�.



private:
	GameEngineStateManager playerStateManager_;

	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;

	PlayerState currentState_;

	PlayerWeaponType weapon_;
	PlayerLegStatus leg_;
	PlayerTopStatus top_;
	AimingDirection direction_;

	bool isLanded_;
	bool isRight_;
	float runningSpeed_;
	float ducksteppingSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

