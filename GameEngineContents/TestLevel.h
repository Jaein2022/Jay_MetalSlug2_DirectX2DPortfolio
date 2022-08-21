#pragma once

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



public:
	static const float gravity_;
	static const float playSpeed_;

private:
	
	
	TestPlayer* testPlayer_;
	TestBackground* testBackground_;

};

