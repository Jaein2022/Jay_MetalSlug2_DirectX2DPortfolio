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

	//�ٿ� ���ҽ��ϱ� ���� �������� ����.
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
		MsgBoxAssert("���̴����ҽ��䰡 �����ϴ�.");
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
		MsgBoxAssert("���̴����ҽ��䰡 �����ϴ�.");
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
		//�̹� ����Ÿ�ٺ䰡 �����Ǿ� �ִٸ� �ٽ� �������� �ʰ� �����Ǿ� �ִ°��� ��ȯ�Ѵ�.
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateRenderTargetView(	//����Ÿ�ٺ並 �����ϴ� �Լ�.
		this->texture2D_,	//����Ÿ�ٺ䰡 �����Ǵµ� �ʿ��� ���ҽ�.
		NULL,	 //����Ÿ�ٺ並 ������ �� �ʿ��� ��������. 
		&renderTargetView_	//������� ���� ����Ÿ�ٺ� �������̽� �������� �ּ�. 
	))
	{
		MsgBoxAssert("����Ÿ�ٺ� ���� ����.");
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
		MsgBoxAssert("���� ���ٽ� ���� ���� ����.");
		return nullptr;
	}

	return depthStencilView_;
}

void GameEngineTexture::Cut(const std::string& _textureName, int _x, int _y)
{
	GameEngineTexture* findTexture = GameEngineTexture::Find(_textureName);
	if (nullptr == findTexture)
	{
		MsgBoxAssertString(_textureName + ": �׷� �̸��� �ؽ��İ� �������� �ʽ��ϴ�.");
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
	//������ ���� �̻��ϴٸ� ���⼭ ���� Ȯ��.

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
		MsgBoxAssert("TGA: ���� ó�� �غ���� ���� �̹��� �����Դϴ�.");
		return;
	}
	else if (uppercaseExtension == "DDS")
	{
		MsgBoxAssert("DDS: ���� ó�� �غ���� ���� �̹��� �����Դϴ�.");
		return;
	}
	else if (S_OK != DirectX::LoadFromWICFile(//
		unicodePath.c_str(),				  //
		DirectX::WIC_FLAGS_NONE,			  //
		&metaData_,							  //
		scratchImage_						  //
	))
	{
		MsgBoxAssertString(_path + ": �ؽ��� �ε� ����.");
		return;
	}


	//GameEngineDevice::GetDevice()->CreateShaderResourceView(); ������� ����.
	//��� �� ���� DirectXTex�� ���̴����ҽ��� �����Լ� ���.

	if (S_OK != DirectX::CreateShaderResourceView(//
		GameEngineDevice::GetDevice(),			  //
		scratchImage_.GetImages(),				  //
		scratchImage_.GetImageCount(),			  //
		scratchImage_.GetMetadata(),			  //
		&shaderResourceView_					  //
	))
	{
		MsgBoxAssertString(_path + ": ���̴� ���ҽ� ���� ����.");
		return;
	}

	texture2DDesc_.Width = static_cast<UINT>(metaData_.width);
	texture2DDesc_.Height = static_cast<UINT>(metaData_.height);
}

void GameEngineTexture::TextureCreate(const D3D11_TEXTURE2D_DESC& _desc)
{
	texture2DDesc_ = _desc;

	if (S_OK != GameEngineDevice::GetDevice()->CreateTexture2D(
		&texture2DDesc_,	//�ؽ�ó2D ������ ����.
		nullptr,			//�ؽ�ó ������ �ʿ��� �ʱⵥ����.
		&texture2D_			//������ �ؽ�ó�� ���� ID3D11Texture2D �������̽� ������.
	))
	{
		MsgBoxAssert("�ؽ�ó ���� ����.");
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