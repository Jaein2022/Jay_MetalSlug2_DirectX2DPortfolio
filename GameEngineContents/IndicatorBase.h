#pragma once
#include "GlobalHeader.h"

class Indicator;
class IndicatorBase: public GameEngineActor
{
	//�� Ŭ������ ���� ����: ���� ���� ���� ���� �ε������� ����� ����ؼ�.

public:
	IndicatorBase();
	~IndicatorBase();

protected:
	IndicatorBase(const IndicatorBase& _other) = delete;
	IndicatorBase(IndicatorBase&& _other) noexcept = delete;

private:
	IndicatorBase& operator=(const IndicatorBase& _other) = delete;
	IndicatorBase& operator=(const IndicatorBase&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void SetPointerColor(const float4& _color);
	void SetPointerScale(const float4& _scale);


private:
	Indicator* indicator_;
};

