#pragma once
#include "GlobalHeader.h"

class Sword;
class Arabian;
class PistolBullet;
class Indicator;
class IndicatorBase;
class Soldier;
class TestBackground;
class TestLevel: public GameEngineLevel
{
	//이 클래스의 존재 이유: 여러가지 테스트.
public:
	TestLevel();
	~TestLevel();

protected:
	TestLevel(const TestLevel& _other) = delete;
	TestLevel(TestLevel&& _other) noexcept = delete;

private:
	TestLevel& operator=(const TestLevel& _other) = delete;
	TestLevel& operator=(const TestLevel&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	void UpdateCameraActorMovement(float _deltaTime);

private:
	Soldier* testPlayer_;
	TestBackground* testBackground_;
	Arabian* testArabian_;

	Indicator* currentFocusPointer_;
	IndicatorBase* destFocus_;
};

