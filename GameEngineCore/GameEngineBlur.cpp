#include "PreCompile.h"
#include "GameEngineBlur.h"
#include "GameEngineRenderTarget.h"

GameEngineBlur::GameEngineBlur() : copiedRenderTarget_(nullptr)
{
}

GameEngineBlur::~GameEngineBlur()
{
	//if (nullptr != copiedRenderTarget_)
	//{
	//	delete copiedRenderTarget_;
	//	copiedRenderTarget_ = nullptr;
	//}
}

void GameEngineBlur::EffectInit()
{
	copiedRenderTarget_ = GameEngineRenderTarget::Create();
	copiedRenderTarget_->CreateRenderTargetTexture(
		GameEngineWindow::GetScale(),
		DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
		float4::Zero
	);

	effectRenderSet_.SetPipeLine("Blur");
	effectRenderSet_.shaderResourceHelper_.SetConstantBuffer_New(
		"WindowScale",
		GameEngineWindow::GetInst()->GetScale()
	);
}

void GameEngineBlur::Effect(GameEngineRenderTarget* _renderTarget)
{
	copiedRenderTarget_->Copy(_renderTarget);

	effectRenderSet_.shaderResourceHelper_.SetTexture("Tex", copiedRenderTarget_->GetRenderTargetTexture(0));

	_renderTarget->Clear();
	_renderTarget->Setting();
	_renderTarget->Effect(this->effectRenderSet_);
}
