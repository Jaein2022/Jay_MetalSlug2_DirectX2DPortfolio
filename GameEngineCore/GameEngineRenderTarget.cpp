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
	//_texture를 저장할 newTexture를 생성한다.

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
	//renderTargets_에 newTexture를 저장한다.

	this->renderTargetViews_.push_back(_texture->CreateRenderTargetView());
	//newTexture에서 생성한 렌더타겟뷰를 저장한다.

	this->shaderResourceViews_.push_back(_texture->CreateShaderResourceView());
	//

	this->clearColors_.push_back(_color);
	//_color도 저장한다.
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
		GameEngineDevice::GetContext()->ClearRenderTargetView(	//지정한 렌더타겟뷰를 한 색으로 채우는 함수.
			renderTargetViews_[i],		//대상 렌더타겟뷰.
			clearColors_[i].arr1D		//대상 렌더타겟뷰를 칠할 색.
		);
	}

	if (nullptr != depthStencilView_)
	{
		GameEngineDevice::GetContext()->ClearDepthStencilView(	//깊이 스텐실 뷰 초기화함수.
			depthStencilView_,						//초기화 대상 깊이 스텐실 뷰.
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,//깊이버퍼 | 스텐실버퍼 초기화
			1.f,									//깊이 초기값.
			0										//스텐실 초기값.
		);
	}
}

void GameEngineRenderTarget::Setting()
{
	if (true == renderTargetViews_.empty())
	{
		MsgBoxAssert("세팅할 렌더타겟뷰가 하나도 없습니다.");
		return;
	}

	GameEngineDevice::GetContext()->OMSetRenderTargets(		//지정한 렌더타겟뷰를 렌더링 파이프라인에 연결하는 함수.
		static_cast<UINT>(renderTargetViews_.size()),	//연결할	렌더타겟뷰 수. 0~8개 지정 가능.
		&renderTargetViews_[0],			//렌더타겟뷰 배열 주소.
		depthStencilView_
	);

}
