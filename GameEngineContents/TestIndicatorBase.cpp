#include "PreCompile.h"
#include "TestIndicatorBase.h"
#include "TestIndicator.h"

TestIndicatorBase::TestIndicatorBase(): indicator_(nullptr)
{
}

TestIndicatorBase::~TestIndicatorBase()
{
}

void TestIndicatorBase::Start()
{
	this->GetTransform().SetLocalScale(float4::One);
	this->GetTransform().SetWorldScale(float4::One);
	indicator_ = TestIndicator::CreateIndicator<TestIndicator>(
		this->GetNameConstRef() + "_Indicator",
		this,
		float4::Black,
		float4::Zero,
		float4(5, 5, 1)
	);
}

void TestIndicatorBase::Update(float _deltaTime)
{
}

void TestIndicatorBase::End()
{
}

void TestIndicatorBase::SetPointerColor(const float4& _color)
{
	indicator_->color_ = _color;
}

void TestIndicatorBase::SetPointerScale(const float4& _scale)
{
	indicator_->GetTransform().SetLocalScale(_scale);
}
