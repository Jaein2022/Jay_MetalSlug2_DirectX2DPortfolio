#include "PreCompile.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"

ID3D11RenderTargetView* GameEngineRenderTarget::prevRenderTargetView_ = nullptr;
ID3D11DepthStencilView* GameEngineRenderTarget::prevDepthStencilView_ = nullptr;

GameEngineRenderTarget::GameEngineRenderTarget() 
	: depthStencilView_(nullptr),
	depthTexture_(nullptr),
	mergePipeLine_(GameEngineRenderingPipeLine::Find("TargetMerge"))
{
	mergeShaderResourceHelper_.ResourceCheck(mergePipeLine_);
}

GameEngineRenderTarget::~GameEngineRenderTarget()
{
	for (GameEnginePostEffect* effect : allEffects_)
	{
		delete effect;
		effect = nullptr;
	}

	allEffects_.clear();
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

GameEngineTexture* GameEngineRenderTarget::GetRenderTargetTexture(size_t _index)
{
	if (renderTargets_.size() <= _index)
	{
		MsgBoxAssert("����Ÿ���� ������ �ʰ��� �ε����� �Է��Ͽ����ϴ�.");
		return nullptr;
	}

	return renderTargets_[_index];
}

void GameEngineRenderTarget::CreateDepthTexture(int _index)
{
	D3D11_TEXTURE2D_DESC depthTextureDesc = { 0 };
	depthTextureDesc.Width = renderTargets_[_index]->GetScale().UIX();
	depthTextureDesc.Height = renderTargets_[_index]->GetScale().UIY();
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	depthTexture_ = GameEngineTexture::Create(depthTextureDesc);

	depthStencilView_ = depthTexture_->CreateDepthStencilView();
}

void GameEngineRenderTarget::SetDepthTexture(GameEngineTexture* _depthTexture)
{
	depthTexture_ = _depthTexture;
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

	GameEngineDevice::GetContext()->OMSetRenderTargets(		//������ ����Ÿ�ٺ並 ����̽� ���ؽ�Ʈ�� �����ϴ� �Լ�.
		static_cast<UINT>(renderTargetViews_.size()),	//������	����Ÿ�ٺ� ��. 0~8�� ���� ����.
		&renderTargetViews_[0],			//����Ÿ�ٺ� �迭 �ּ�.
		depthStencilView_
	);
}

void GameEngineRenderTarget::Copy(GameEngineRenderTarget* _otherRenderTarget, int _index /*= 0*/)
{
	this->Clear();
	mergeShaderResourceHelper_.SetTexture("Tex", _otherRenderTarget->GetRenderTargetTexture(_index));
	Effect(GameEngineRenderingPipeLine::Find("TargetMerge"), &mergeShaderResourceHelper_);
}

void GameEngineRenderTarget::Merge(GameEngineRenderTarget* _otherRenderTarget, int _index /*= 0*/)
{
	mergeShaderResourceHelper_.SetTexture("Tex", _otherRenderTarget->GetRenderTargetTexture(_index));
	Effect(GameEngineRenderingPipeLine::Find("TargetMerge"), &mergeShaderResourceHelper_);
}

void GameEngineRenderTarget::Effect(GameEngineRenderingPipeLine* _otherPipeLine, GameEngineShaderResourceHelper* _shaderResourceHelper)
{
	this->Setting();
	_shaderResourceHelper->AllResourcesSetting();
	_otherPipeLine->Rendering();
	_shaderResourceHelper->AllResourcesReset();
}

void GameEngineRenderTarget::EffectProcess()
{
	for (GameEnginePostEffect* effect : allEffects_)
	{
		if (true == effect->IsUpdate())
		{
			effect->Effect(this);
		}
	}
}

void GameEngineRenderTarget::GetPrevRenderTarget()
{
	GameEngineDevice::GetContext()->OMGetRenderTargets(
		1,
		&prevRenderTargetView_,
		&prevDepthStencilView_
	);
}

void GameEngineRenderTarget::SetPrevRenderTarget()
{
	GameEngineDevice::GetContext()->OMSetRenderTargets(
		1,
		&prevRenderTargetView_,
		prevDepthStencilView_
	);

	if (nullptr != prevRenderTargetView_)
	{
		prevRenderTargetView_->Release();
	}

	if (nullptr != prevDepthStencilView_)
	{
		prevDepthStencilView_->Release();
	}
}
