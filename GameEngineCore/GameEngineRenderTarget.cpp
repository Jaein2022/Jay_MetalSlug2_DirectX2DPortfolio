#include "PreCompile.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"
#include "GameEngineDepthStencilTexture.h"

GameEngineRenderTarget::GameEngineRenderTarget() : depthStencilView_(nullptr), depthTexture_(nullptr)
{
}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
}

GameEngineRenderTarget* GameEngineRenderTarget::Create(const std::string& _name)
{
	return CreateNamedRes(_name);
}

void GameEngineRenderTarget::CreateRenderTarget(
	ID3D11Texture2D* _texture,
	const float4& _clearColor
)
{
	GameEngineTexture* newTexture = GameEngineTexture::Create(_texture);
	//_texture�� ������ newTexture�� �����Ѵ�.

	this->renderTargets_.push_back(newTexture);
	//renderTargets_�� newTexture�� �����Ѵ�.

	this->renderTargetViews_.push_back(newTexture->CreateRenderTargetView());
	//newTexture���� ������ ����Ÿ�ٺ並 �����Ѵ�.

	this->clearColors_.push_back(_clearColor);
	//_clearColor�� �����Ѵ�.

}

void GameEngineRenderTarget::CreateDepthTexture(int _index)
{
	depthTexture_ = GameEngineDepthStencilTexture::Create(renderTargets_[_index]->GetScale());
	depthStencilView_ = depthTexture_->CreateDepthStencilView();
}

void GameEngineRenderTarget::Clear()
{
	for (size_t i = 0; i < renderTargetViews_.size(); i++)
	{
		GameEngineDevice::GetContext()->ClearRenderTargetView(	//������ ����Ÿ�ٺ並 �� ������ ä��� �Լ�.
			renderTargetViews_[i],		//��� ����Ÿ�ٺ�.
			clearColors_[i].arr1D		//��� ����Ÿ�ٺ並 ĥ�� ��.
		);
	}

	if (nullptr != depthStencilView_)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(	//���� ���ٽ� �� �ʱ�ȭ�Լ�.
			depthStencilView_,						//�ʱ�ȭ ��� ���� ���ٽ� ��.
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,//���̹��� | ���ٽǹ��� �ʱ�ȭ
			1.f,									//���� �ʱⰪ.
			0										//���ٽ� �ʱⰪ.
		);
	}
}

void GameEngineRenderTarget::Setting()
{
	if (true == renderTargetViews_.empty())
	{
		MsgBoxAssert("������ ����Ÿ�ٺ䰡 �ϳ��� �����ϴ�.");
		return;
	}

	GameEngineDevice::GetContext()->OMSetRenderTargets(		//������ ����Ÿ�ٺ並 ������ ���������ο� �����ϴ� �Լ�.
		static_cast<UINT>(renderTargetViews_.size()),	//������	����Ÿ�ٺ� ��. 0~8�� ���� ����.
		&renderTargetViews_[0],			//����Ÿ�ٺ� �迭 �ּ�.
		depthStencilView_
	);

}
