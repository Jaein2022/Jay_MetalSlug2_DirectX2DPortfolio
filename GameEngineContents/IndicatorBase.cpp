#include "PreCompile.h"
#include "IndicatorBase.h"
#include "Indicator.h"

IndicatorBase::IndicatorBase(): indicator_(nullptr)
{
}

IndicatorBase::~IndicatorBase()
{
}

void IndicatorBase::Start()
{
	this->GetTransform().SetLocalScale(float4::One);
	this->GetTransform().SetWorldScale(float4::One);
	indicator_ = Indicator::CreateIndicator<Indicator>(
		this->GetNameConstRef() + "_Indicator",
		this,
		float4::Black,
		float4::Zero,
		float4(5, 5, 1)
	);
}

void IndicatorBase::Update(float _deltaTime)
{
}

void IndicatorBase::End()
{
}

void IndicatorBase::SetPointerColor(const float4& _color)
{
	indicator_->color_ = _color;
}

void IndicatorBase::SetPointerScale(const float4& _scale)
{
	indicator_->GetTransform().SetLocalScale(_scale);
}
