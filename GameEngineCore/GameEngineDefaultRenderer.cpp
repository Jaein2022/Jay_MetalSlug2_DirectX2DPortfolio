#include "PreCompile.h"
#include "GameEngineDefaultRenderer.h"
#include "GameEngineRenderingPipeLine.h"

GameEngineDefaultRenderer::GameEngineDefaultRenderer() : renderingPipeLine_(nullptr)
{
}

GameEngineDefaultRenderer::~GameEngineDefaultRenderer()
{
}

void GameEngineDefaultRenderer::Render(float _deltaTime)
{
	if (nullptr == renderingPipeLine_)
	{
		MsgBoxAssert("렌더링 파이프라인이 없습니다. 렌더링을 할 수 없습니다.");
		return;
	}

	shaderResources_.AllResourcesSetting();
	renderingPipeLine_->Rendering();
}

void GameEngineDefaultRenderer::SetPipeLine(const std::string& _name)
{
	renderingPipeLine_ = GameEngineRenderingPipeLine::Find(_name);

	if (nullptr == renderingPipeLine_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 렌더링 파이프라인이 없습니다.");
		return;
	}

	shaderResources_.ResourceCheck(renderingPipeLine_);

	if (true == shaderResources_.IsConstantBuffer("TRANSFORMDATA"))
	{
		shaderResources_.SetConstantBuffer_Link(
			"TRANSFORMDATA",
			&GetTransformData(),
			sizeof(GetTransformData())
		);
	}

	if (true == shaderResources_.IsConstantBuffer("RENDEROPTION"))
	{
		shaderResources_.SetConstantBuffer_Link(
			"RENDEROPTION",
			&renderOption_,
			sizeof(renderOption_)
		);
	}
}

void GameEngineDefaultRenderer::Start()
{
	GameEngineRenderer::Start();
}

void GameEngineDefaultRenderer::Update(float _deltaTime)
{
}

void GameEngineDefaultRenderer::End()
{
}
