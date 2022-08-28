#pragma once

class GameEngineTexture;
class TestIndicator : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유: 특정 지점을 소량의 비용으로 표시.
 
	//한가하면 전용 렌더링 파이프라인, 전용 셰이더를 만들어서 간단한 지시자 모양들을 만들어 볼 것.
	//나중에 메인, UI카메라가 아닌 제3의 카메라에 등록해볼 것.

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

