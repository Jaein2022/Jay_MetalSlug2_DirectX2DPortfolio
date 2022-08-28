#pragma once

class GameEngineTexture;
class TestIndicator : public GameEngineDefaultRenderer
{
	//�� Ŭ������ ���� ����: Ư�� ������ �ҷ��� ������� ǥ��.
 
	//�Ѱ��ϸ� ���� ������ ����������, ���� ���̴��� ���� ������ ������ ������ ����� �� ��.
	//���߿� ����, UIī�޶� �ƴ� ��3�� ī�޶� ����غ� ��.

	friend class TestIndicatorBase;

protected:
	static bool isRendering_;

protected:
	TestIndicator();
	virtual ~TestIndicator();
	TestIndicator(const TestIndicator& _other) = delete;
	TestIndicator(TestIndicator&& _other) noexcept = delete;
	TestIndicator& operator=(const TestIndicator& _other) = delete;
	TestIndicator& operator=(const TestIndicator&& _other) = delete;

public:
	template <typename PointerType>
	static PointerType* CreateIndicator(
		const std::string& _name,
		GameEngineActor* _parentActor,
		const float4& _color,
		const float4& _localPosition,
		const float4& _localScale
	)
	{
		TestIndicator* newIndicator = new PointerType();
		newIndicator->SetName(_name);
		newIndicator->color_ = _color;
		newIndicator->SetParent(_parentActor);
		newIndicator->GetTransform().SetLocalScale(_localScale);
		newIndicator->GetTransform().SetLocalPosition(_localPosition);
		newIndicator->Start();
		//newPointer->SetRenderingOrder(5);
		return dynamic_cast<PointerType*>(newIndicator);
	}

	static void RenderingOnOffSwitch()
	{
		isRendering_ = !isRendering_;
	}

protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void Render(float _deltaTime) override;

protected:
	float4 color_;


};

