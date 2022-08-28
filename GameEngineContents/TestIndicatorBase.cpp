#include "PreCompile.h"
#include "TestIndicatorBase.h"
#include "TestIndicator.h"

TestIndicatorBase::TestIndicatorBase(): pointer_(nullptr)
{
}

TestIndicatorBase::~TestIndicatorBase()
{
}

void TestIndicatorBase::Start()
{
	this->GetTransform().SetLocalScale(float4::One);
	this->GetTransform().SetWorldScale(float4::One);
	pointer_ = TestIndicator::CreateIndicator<TestIndicator>(
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
	pointer_->color_ = _color;
}

void TestIndicatorBase::SetPointerScale(const float4& _scale)
{
	pointer_->GetTransform().SetLocalScale(_scale);
}
