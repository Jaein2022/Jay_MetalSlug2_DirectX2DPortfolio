#pragma once

enum class ActorGroup
{
	//테스트레벨을 벗어나서 정식 미션 만들때 공용이넘으로 분리할 것.
	Player,
	PistolBullet,
	Rebel,
	Sword,
	Background,
	UI,
};

class TestSword;
class TestArabian;
class TestPistolBullet;
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
	TestPistolBullet* GetPistolBullet();
	TestSword* GetSword();
	const float4& GetPlayerWorldPosition();

public:

	//테스트레벨을 벗어나서 정식 미션 만들때 공용이넘으로 분리할 것.
	static const float gravity_;
	static const float playSpeed_;
	static const PixelColor groundColor_;	//cyan(0, 255, 255, 255) = 4294967040(UINT)
	


private:
	void UpdateCameraActorMovement(float _deltaTime);

private:
	TestPlayer* testPlayer_;
	TestBackground* testBackground_;
	TestArabian* testArabian_;

	TestIndicator* currentFocusPointer_;
	TestIndicatorBase* destFocus_;
};

