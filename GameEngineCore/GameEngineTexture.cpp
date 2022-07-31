#include "PreCompile.h"
#include "GameEngineTexture.h"
#include "GameEngineDevice.h"
#pragma comment(lib, "DirectXTex.lib")

GameEngineTexture::GameEngineTexture()
	: texture2D_(nullptr),
	renderTargetView_(nullptr),
	shaderResourceView_(nullptr),
	depthStencilView_(nullptr),
	texture2DDesc_(),
	metaData_()
{
}

GameEngineTexture::~GameEngineTexture()
{
	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}
	if (nullptr != depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}

	//근원 리소스니까 제일 마지막에 삭제.
	if (nullptr != texture2D_)
	{
		texture2D_->Release();
		texture2D_ = nullptr;
	}
}

GameEngineTexture* GameEngineTexture::Create(const std::string& _name, ID3D11Texture2D* _texture)
{
	GameEngineTexture* newRes = CreateNamedRes(_name);
	newRes->texture2D_ = _texture;
	_texture->GetDesc(&newRes->texture2DDesc_);
	return newRes;
}

GameEngineTexture* GameEngineTexture::Create(ID3D11Texture2D* _texture)
{
	GameEngineTexture* newRes = CreateUnnamedRes();
	newRes->texture2D_ = _texture;
	_texture->GetDesc(&newRes->texture2DDesc_);
	return newRes;
}

GameEngineTexture* GameEngineTexture::Create(const D3D11_TEXTURE2D_DESC& _desc)
{
	GameEngineTexture* newRes = CreateUnnamedRes();
	newRes->TextureCreate(_desc);
	return newRes;
}

GameEngineTexture* GameEngineTexture::Load(const std::string& _path)
{
	return Load(_path, GameEnginePath::GetFileName(_path));
}

GameEngineTexture* GameEngineTexture::Load(const std::string& _path, const std::string& _name)
{
	GameEngineTexture* newRes = CreateNamedRes(_name);
	newRes->TextureLoad(_path);
	return newRes;
}

void GameEngineTexture::VSSetting(int _bindPoint)
{
	if (nullptr == shaderResourceView_)
	{
		MsgBoxAssert("셰이더리소스뷰가 없습니다.");
		return;
	}

	GameEngineDevice::GetContext()->VSSetShaderResources(
		_bindPoint,
		1,
		&shaderResourceView_
	);
}

void GameEngineTexture::PSSetting(int _bindPoint)
{
	if (nullptr == shaderResourceView_)
	{
		MsgBoxAssert("셰이더리소스뷰가 없습니다.");
		return;
	}

	GameEngineDevice::GetContext()->PSSetShaderResources(
		_bindPoint,
		1,
		&shaderResourceView_
	);
}

ID3D11RenderTargetView* GameEngineTexture::CreateRenderTargetView()
{
	if (nullptr != this->renderTargetView_)
	{
		return this->renderTargetView_;
		//이미 렌더타겟뷰가 생성되어 있다면 다시 생성하지 않고 생성되어 있는것을 반환한다.
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(	//렌더타겟뷰를 생성하는 함수.
		this->texture2D_,	//렌더타겟뷰가 생성되는데 필요한 리소스.
		NULL,	 //렌더타겟뷰를 생성할 때 필요한 세부정보. 
		&renderTargetView_	//결과물을 받을 렌더타겟뷰 인터페이스 포인터의 주소. 
	))
	{
		MsgBoxAssert("렌더타겟뷰 생성 실패.");
		return nullptr;
	}

	return this->renderTargetView_;
}

ID3D11DepthStencilView* GameEngineTexture::CreateDepthStencilView()
{
	if (nullptr != depthStencilView_)
	{
		return depthStencilView_;
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateDepthStencilView(
		texture2D_,
		nullptr,
		&depthStencilView_
	))
	{
		MsgBoxAssert("깊이 스텐실 버퍼 생성 실패.");
		return nullptr;
	}

	return depthStencilView_;
}

void GameEngineTexture::Cut(const std::string& _textureName, int _x, int _y)
{
	GameEngineTexture* findTexture = GameEngineTexture::Find(_textureName);
	if (nullptr == findTexture)
	{
		MsgBoxAssertString(_textureName + ": 그런 이름의 텍스쳐가 존재하지 않습니다.");
		return;
	}
	else
	{
		findTexture->Cut(_x, _y);
	}
}

float4 GameEngineTexture::GetPixel(int _x, int _y)
{
	if (0 > _x)
	{
		return float4::Zero;
	}
	if (0 > _y)
	{
		return float4::Zero;
	}

	if (this->scratchImage_.GetMetadata().width <= _x)
	{
		return float4::Zero;
	}
	if (this->scratchImage_.GetMetadata().height <= _y)
	{
		return float4::Zero;
	}

	DXGI_FORMAT format = this->scratchImage_.GetMetadata().format;
	//색상이 뭔가 이상하다면 여기서 포맷 확인.

	uint8_t* color = this->scratchImage_.GetImages()->pixels;

	int index = _y * static_cast<int>(this->scratchImage_.GetMetadata().width) + _x;

	color = color + static_cast<uint8_t>(index) * 4;

	unsigned char r = color[0];
	unsigned char g = color[1];
	unsigned char b = color[2];
	unsigned char a = color[3];

	return float4(
		static_cast<float>(r) / 255.f,
		static_cast<float>(g) / 255.f,
		static_cast<float>(b) / 255.f,
		static_cast<float>(a) / 255.f
	);
}

void GameEngineTexture::TextureLoad(const std::string& _path)
{
	std::string uppercaseExtension = GameEngineString::ToUpperReturn(GameEnginePath::GetExtension(_path));

	std::wstring unicodePath = GameEngineString::AnsiToUnicodeReturn(_path);


	if (uppercaseExtension == "TGA")
	{
		MsgBoxAssert("TGA: 아직 처리 준비되지 않은 이미지 포맷입니다.");
		return;
	}
	else if (uppercaseExtension == "DDS")
	{
		MsgBoxAssert("DDS: 아직 처리 준비되지 않은 이미지 포맷입니다.");
		return;
	}
	else if (S_OK != DirectX::LoadFromWICFile(//
		unicodePath.c_str(),				  //
		DirectX::WIC_FLAGS_NONE,			  //
		&metaData_,							  //
		scratchImage_						  //
	))
	{
		MsgBoxAssertString(_path + ": 텍스쳐 로드 실패.");
		return;
	}


	//GameEngineDevice::GetDevice()->CreateShaderResourceView(); 사용하지 않음.
	//대신 더 쉬운 DirectXTex의 셰이더리소스뷰 생성함수 사용.

	if (S_OK != DirectX::CreateShaderResourceView(//
		GameEngineDevice::GetDevice(),			  //
		scratchImage_.GetImages(),				  //
		scratchImage_.GetImageCount(),			  //
		scratchImage_.GetMetadata(),			  //
		&shaderResourceView_					  //
	))
	{
		MsgBoxAssertString(_path + ": 셰이더 리소스 생성 실패.");
		return;
	}

	texture2DDesc_.Width = static_cast<UINT>(metaData_.width);
	texture2DDesc_.Height = static_cast<UINT>(metaData_.height);
}

void GameEngineTexture::TextureCreate(const D3D11_TEXTURE2D_DESC& _desc)
{
	texture2DDesc_ = _desc;

	if (S_OK != GameEngineDevice::GetDevice()->CreateTexture2D(
		&texture2DDesc_,	//텍스처2D 생성용 명세서.
		nullptr,			//텍스처 생성시 필요한 초기데이터.
		&texture2D_			//생성한 텍스처를 받을 ID3D11Texture2D 인터페이스 포인터.
	))
	{
		MsgBoxAssert("텍스처 생성 실패.");
		return;
	}

}

void GameEngineTexture::Cut(int _x, int _y)
{
	float sizeX = 1.f / _x;
	float sizeY = 1.f / _y;

	float4 cuttingStart = float4::Zero;

	for (int y = 0; y < _y; y++)
	{
		for (int x = 0; x < _x; x++)
		{
			float4 frameData;
			frameData.posX = cuttingStart.x;
			frameData.posY = cuttingStart.y;
			frameData.sizeX = sizeX;
			frameData.sizeY = sizeY;
			cutData_.push_back(frameData);

			cuttingStart.x += sizeX;
		}

		cuttingStart.x = 0.f;
		cuttingStart.y += sizeY;
	}


}