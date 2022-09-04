#pragma once

class TestSword : public GameEngineActor
{
	//이 클래스의 존재 이유:
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

