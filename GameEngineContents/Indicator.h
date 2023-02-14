#pragma once
#include "GlobalHeader.h"

class GameEngineTexture;
class Indicator : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유: 특정 지점을 소량의 비용으로 화면에 표시.
 
	//메인, UI카메라가 아닌 제3의 카메라에 등록해볼 것.
	//한가하면 전용 렌더링 파이프라인, 전용 셰이더를 만들어서 화살표나 쐐기 같은 간단한 지시자 모양들을 만들어 볼 것.

	friend class IndicatorBase;

protected:
	static bool isRendering_;

protected:
	Indicator();
	virtual ~Indicator();
	Indicator(const Indicator& _other) = delete;
	Indicator(Indicator&& _other) noexcept = delete;
	Indicator& operator=(const Indicator& _other) = delete;
	Indicator& operator=(const Indicator&& _other) = delete;

public:
	template <typename IndicatorType>
	static IndicatorType* CreateIndicator(
		const std::string& _name,
		GameEngineUpdateObject* _parentActor,
		const float4& _color,
		const float4& _localPosition,
		const float4& _localScale
	)
	{
		Indicator* newIndicator = new IndicatorType();
		newIndicator->SetName(_name);
		newIndicator->color_ = _color;
		newIndicator->SetParent(_parentActor);
		newIndicator->GetTransform().SetLocalScale(_localScale);
		newIndicator->GetTransform().SetLocalPosition(_localPosition);
		newIndicator->Start();
		return dynamic_cast<IndicatorType*>(newIndicator);
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

