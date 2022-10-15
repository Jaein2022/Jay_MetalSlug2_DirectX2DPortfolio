#include "PreCompile.h"
#include "GameEngineBlur.h"
#include "GameEngineRenderTarget.h"

GameEngineBlur::GameEngineBlur() : copiedRenderTarget_(nullptr), renderingPipeLine_(nullptr)
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

	//effectRenderSet_.SetPipeLine("Blur");
	//effectRenderSet_.shaderResourceHelper_.SetConstantBuffer_New(
	//	"WindowScale",
	//	GameEngineWindow::GetInst()->GetScale()
	//);

	renderingPipeLine_ = GameEngineRenderingPipeLine::Find("Blur");
	if (nullptr == renderingPipeLine_)
	{
		MsgBoxAssert("Blur: 그런 이름의 렌더링 파이프라인이 존재하지 않습니다.");
		return;
	}

	shaderResourceHelper_.ResourceCheck(this->renderingPipeLine_);

	shaderResourceHelper_.SetConstantBuffer_New(
		"WindowScale",
		GameEngineWindow::GetInst()->GetScale()
	);
}

void GameEngineBlur::Effect(GameEngineRenderTarget* _renderTarget)
{
	copiedRenderTarget_->Copy(_renderTarget);

	this->shaderResourceHelper_.SetTexture("Tex", copiedRenderTarget_->GetRenderTargetTexture(0));

	_renderTarget->Clear();
	_renderTarget->Setting();
	_renderTarget->Effect(this->renderingPipeLine_, &this->shaderResourceHelper_);
}
