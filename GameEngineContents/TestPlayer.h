#pragma once

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

	void Move(float _deltaTime);	//좌우 트랜스폼 변화 담당 함수.
	void Jump(float _deltaTime);	//상하 트랜스폼 변화 담당 함수.




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

	char movingDirection_;	//-1: 좌측으로 이동, 0: 정지, 1: 우측으로 이동.
	bool isJumping_;		//false: 착지 상태. true: 공중에 떠 있는 상태.
	bool isUpKeyPressed_;
	bool isDownKeyPressed_;
	bool isJumpKeyDown_;
	bool isAttackKeyDowned_;
	bool isSpecialKeyDowned_;

	float playerSpeed_;

	int bulletCount_;
	int grenadeCount_;

};

