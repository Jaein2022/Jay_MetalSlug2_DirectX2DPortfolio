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
		MsgBoxAssert("������ ������������ �����ϴ�. �������� �� �� �����ϴ�.");
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
		MsgBoxAssertString(_name + ": �׷� �̸��� ������ ������������ �����ϴ�.");
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

GameEngineRenderingPipeLine* GameEngineDefaultRenderer::GetPipeLine()
{
	if (false == renderingPipeLine_->IsOriginal())
	{
		return renderingPipeLine_;
	}

	renderingPipeLine_ = GetClonePipeLine(renderingPipeLine_);
	return renderingPipeLine_;
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
