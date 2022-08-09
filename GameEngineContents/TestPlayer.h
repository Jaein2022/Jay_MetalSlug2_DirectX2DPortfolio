#pragma once
#include "Enums.h"

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
	void Attack();
	void Fire();
	void MeleeAttack();

	void Move();
	void Jump();
	void Run();


private:
	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;

	GameEngineStateManager* playerStateManager_;

	PlayerWeaponType weapon_;

	float velocity_;
	int bulletCount_;
	int grenadeCount_;


};

