#pragma once

class TestIndicator;
class TestIndicatorBase: public GameEngineActor
{
	//�� Ŭ������ ���� ����: ���� ���� ���� ���� �ε������� ����� ����ؼ�.

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

