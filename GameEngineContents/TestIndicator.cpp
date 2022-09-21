#include "PreCompile.h"
#include "TestIndicator.h"

#ifdef _DEBUG
bool TestIndicator::isRendering_ = true;
#else
bool TestIndicator::isRendering_ = false;
#endif

TestIndicator::TestIndicator()
{
}

TestIndicator::~TestIndicator()
{
}

void TestIndicator::Start()
{
	SetPipeLine("Color");

	if (true == shaderResourceHelper_.IsConstantBuffer("ResultColor"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link("ResultColor", this->color_);
		GameEngineRenderer::PushRendererToMainCamera();
	}
	else
	{
		MsgBoxAssert("ResultColor: �׷� �̸��� ��� ���۰� �����ϴ�.");
		return;
	}
}

void TestIndicator::Update(float _deltaTime)
{
}

void TestIndicator::End()
{
}

void TestIndicator::Render(float _deltaTime)
{
	if (true == isRendering_)
	{
		GameEngineDefaultRenderer::Render(_deltaTime);
	}
}
