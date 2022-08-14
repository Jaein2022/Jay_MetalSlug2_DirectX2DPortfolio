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
	void UpdateInputInfo(float _deltaTime);	//키입력 업데이트.
	//void UpdateContactObject();	//플레이어가 접촉한 오브젝트(아이템, NPC, 적 무기 등) 정보 업데이트.
	void UpdatePlayerState();	//플레이어 전체 상태 업데이트.

	//void Move(float _deltaTime);	//좌우 트랜스폼 변화 담당 함수.
	//void Jump(float _deltaTime);	//상하 트랜스폼 변화 담당 함수.
	//void Fire();					//플레이어 조준각도에 맞는 원거리무기 투사체 생성 발사 함수.
	//void ThrowGrenade();			//수류탄 오브젝트 생성 발사 함수.	
	//void MeleeAttack();			//근접공격 함수.



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

