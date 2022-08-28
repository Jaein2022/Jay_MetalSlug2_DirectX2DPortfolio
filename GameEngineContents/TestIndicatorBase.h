#pragma once

class TestIndicator;
class TestIndicatorBase: public GameEngineActor
{
	//이 클래스의 존재 이유: 액터 없는 레벨 직속 인디케이터 만들어 써야해서.

public:
	TestIndicatorBase();
	~TestIndicatorBase();

protected:
	TestIndicatorBase(const TestIndicatorBase& _other) = delete;
	TestIndicatorBase(TestIndicatorBase&& _other) noexcept = delete;

private:
	TestIndicatorBase& operator=(const TestIndicatorBase& _other) = delete;
	TestIndicatorBase& operator=(const TestIndicatorBase&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void SetPointerColor(const float4& _color);
	void SetPointerScale(const float4& _scale);


private:
	TestIndicator* pointer_;
};

