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

GameEngineRenderTarget* GameEngineRenderTarget::Create()
{
	return CreateUnnamedRes();
}

void GameEngineRenderTarget::CreateRenderTargetTexture(
	ID3D11Texture2D* _texture,
	const float4& _color
)
{
	GameEngineTexture* newTexture = GameEngineTexture::Create(_texture);
	//_texture�� ������ newTexture�� �����Ѵ�.

	CreateRenderTargetTexture(newTexture, _color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(
	const float4& _size,
	const float4& _color
)
{
	CreateRenderTargetTexture(_size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, _color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(
	const float4& _size,
	DXGI_FORMAT _format,
	const float4& _color
)
{
	D3D11_TEXTURE2D_DESC newTextureDesc = { 0 };
	newTextureDesc.Width = _size.UIX();
	newTextureDesc.Height = _size.UIY();
	newTextureDesc.MipLevels = 1;
	newTextureDesc.ArraySize = 1;
	newTextureDesc.Format = _format;
	newTextureDesc.SampleDesc.Count = 1;
	newTextureDesc.SampleDesc.Quality = 0;
	newTextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	newTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	CreateRenderTargetTexture(newTextureDesc, _color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(
	D3D11_TEXTURE2D_DESC _desc,
	const float4& _color
)
{
	GameEngineTexture* newTexture = GameEngineTexture::Create(_desc);
	CreateRenderTargetTexture(newTexture, _color);
}

void GameEngineRenderTarget::CreateRenderTargetTexture(
	GameEngineTexture* _texture,
	const float4& _color
)
{
	this->renderTargets_.push_back(_texture);
	//renderTargets_�� newTexture�� �����Ѵ�.

	this->renderTargetViews_.push_back(_texture->CreateRenderTargetView());
	//newTexture���� ������ ����Ÿ�ٺ並 �����Ѵ�.

	this->shaderResourceViews_.push_back(_texture->CreateShaderResourceView());
	//

	this->clearColors_.push_back(_color);
	//_color�� �����Ѵ�.
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
