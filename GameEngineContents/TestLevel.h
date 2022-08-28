#pragma once

class TestIndicator;
class TestIndicatorBase;
class TestBackground;
class TestPlayer;
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

public:



public:
	static const float gravity_;
	static const float playSpeed_;


private:
	void UpdateCameraActorMovement(float _deltaTime);

private:
	TestPlayer* testPlayer_;
	TestBackground* testBackground_;

	TestIndicator* currentFocusPointer_;
	TestIndicatorBase* destFocus_;
};

