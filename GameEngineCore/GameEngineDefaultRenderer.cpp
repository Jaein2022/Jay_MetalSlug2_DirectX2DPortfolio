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

	this->shaderResourceHelper_.AllResourcesSetting();
	this->renderingPipeLine_->Rendering();
	this->shaderResourceHelper_.AllResourcesReset();
}

void GameEngineDefaultRenderer::SetPipeLine(const std::string& _renderingPipeLineName)
{
	renderingPipeLine_ = GameEngineRenderingPipeLine::Find(_renderingPipeLineName);

	if (nullptr == renderingPipeLine_)
	{
		MsgBoxAssertString(_renderingPipeLineName + ": �׷� �̸��� ������ ������������ �����ϴ�.");
		return;
	}

	this->shaderResourceHelper_.ResourceCheck(renderingPipeLine_);

	if (true == shaderResourceHelper_.IsConstantBuffer("TRANSFORMDATA"))
	{
		shaderResourceHelper_.SetConstantBuffer_Link(
			"TRANSFORMDATA",
			&GetTransformData(),
			sizeof(GetTransformData())
		);
	}

	if (true == shaderResourceHelper_.IsConstantBuffer("RENDEROPTION"))
	{
		shaderResourceHelper_.SetConstantBuffer_Link(
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

	renderingPipeLine_ = ClonePipeLine(renderingPipeLine_);
	return renderingPipeLine_;
}

GameEngineRenderingPipeLine* GameEngineDefaultRenderer::GetClonePipeLine()
{
	return this->renderingPipeLine_;
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
