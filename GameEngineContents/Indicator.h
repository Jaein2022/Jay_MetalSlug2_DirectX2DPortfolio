#pragma once
#include "GlobalHeader.h"

class GameEngineTexture;
class Indicator : public GameEngineDefaultRenderer
{
	//�� Ŭ������ ���� ����: Ư�� ������ �ҷ��� ������� ȭ�鿡 ǥ��.
 
	//����, UIī�޶� �ƴ� ��3�� ī�޶� ����غ� ��.
	//�Ѱ��ϸ� ���� ������ ����������, ���� ���̴��� ���� ȭ��ǥ�� ���� ���� ������ ������ ������ ����� �� ��.

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

