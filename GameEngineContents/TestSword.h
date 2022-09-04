#pragma once

class TestSword : public GameEngineActor
{
	//�� Ŭ������ ���� ����:
public:
	TestSword();
	~TestSword();

protected:
	TestSword(const TestSword& _other) = delete;
	TestSword(TestSword&& _other) noexcept = delete;

private:
	TestSword& operator=(const TestSword& _other) = delete;
	TestSword& operator=(const TestSword&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:

};

