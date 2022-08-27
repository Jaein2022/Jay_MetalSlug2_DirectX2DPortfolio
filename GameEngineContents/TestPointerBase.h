#pragma once

class TestPointer;
class TestPointerBase: public GameEngineActor
{
	//�� Ŭ������ ���� ����: ���� ���� ���� ���� ������ ����� ����ؼ�.

public:
	TestPointerBase();
	~TestPointerBase();

protected:
	TestPointerBase(const TestPointerBase& _other) = delete;
	TestPointerBase(TestPointerBase&& _other) noexcept = delete;

private:
	TestPointerBase& operator=(const TestPointerBase& _other) = delete;
	TestPointerBase& operator=(const TestPointerBase&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void SetPointerColor(const float4& _color);
	void SetPointerScale(const float4& _scale);


private:
	TestPointer* pointer_;
};

