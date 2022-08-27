#include "PreCompile.h"
#include "GameEngineRenderSet.h"
#include "GameEngineRenderingPipeLine.h"

GameEngineRenderSet::GameEngineRenderSet() : renderingPipeLine_(nullptr)
{
}

GameEngineRenderSet::~GameEngineRenderSet()
{
}

void GameEngineRenderSet::SetPipeLine(const std::string& _renderingPipeLineName)
{
	this->renderingPipeLine_ = GameEngineRenderingPipeLine::Find(_renderingPipeLineName);
	if (nullptr == this->renderingPipeLine_)
	{
		MsgBoxAssertString(_renderingPipeLineName + ": �׷� �̸��� ������ ������������ �������� �ʽ��ϴ�.");
		return;
	}

	this->shaderResourceHelper_.ResourceCheck(this->renderingPipeLine_);
}

GameEngineRenderingPipeLine* GameEngineRenderSet::GetPipeLine()
{
	if (false == this->renderingPipeLine_->IsOriginal())
	{
		return this->renderingPipeLine_;
	}

	GameEngineRenderingPipeLine* copiedPipeLine = GameEngineRenderingPipeLine::Create();
	copiedPipeLine->Copy(this->renderingPipeLine_);
	this->renderingPipeLine_ = copiedPipeLine;
	return this->renderingPipeLine_;
}

void GameEngineRenderSet::Render()
{
	if (nullptr == this->renderingPipeLine_)
	{
		MsgBoxAssert("������ ������������ �����ϴ�.");
		return;
	}

	this->shaderResourceHelper_.AllResourcesSetting();
	this->renderingPipeLine_->Rendering();
}
