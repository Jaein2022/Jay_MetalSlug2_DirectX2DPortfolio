#include "PreCompile.h"
#include "Indicator.h"

#ifdef _DEBUG
bool Indicator::isRendering_ = true;
#else
bool Indicator::isRendering_ = false;
#endif

Indicator::Indicator()
{
}

Indicator::~Indicator()
{
}

void Indicator::Start()
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

void Indicator::Update(float _deltaTime)
{
}

void Indicator::End()
{
}

void Indicator::Render(float _deltaTime)
{
	if (true == isRendering_)
	{
		GameEngineDefaultRenderer::Render(_deltaTime);
	}
}
