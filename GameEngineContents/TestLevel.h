#pragma once

enum class ActorGroup
{
	//�׽�Ʈ������ ����� ���� �̼� ���鶧 �����̳����� �и��� ��.
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
	//�� Ŭ������ ���� ����: �������� �׽�Ʈ.
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

	//�׽�Ʈ������ ����� ���� �̼� ���鶧 �����̳����� �и��� ��.
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

