#include "PreCompile.h"
#include "TestPointerBase.h"
#include "TestPointer.h"

TestPointerBase::TestPointerBase(): pointer_(nullptr)
{
}

TestPointerBase::~TestPointerBase()
{
}

void TestPointerBase::Start()
{
	this->GetTransform().SetLocalScale(float4::One);
	this->GetTransform().SetWorldScale(float4::One);
	pointer_ = TestPointer::CreatePointer(
		this->GetNameConstRef() + "Pointer",
		this,
		float4::Black,
		float4::Zero,
		float4(5, 5, 1)
	);
}

void TestPointerBase::Update(float _deltaTime)
{
}

void TestPointerBase::End()
{
}

void TestPointerBase::SetPointerColor(const float4& _color)
{
	pointer_->color_ = _color;
}

void TestPointerBase::SetPointerScale(const float4& _scale)
{
	pointer_->GetTransform().SetLocalScale(_scale);
}
