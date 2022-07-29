#include "PreCompile.h"
#include "GameEngineDepthStencilTexture.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"

GameEngineDepthStencilTexture::GameEngineDepthStencilTexture() : depthBuffer_(nullptr)
{
}

GameEngineDepthStencilTexture::~GameEngineDepthStencilTexture()
{
}

GameEngineDepthStencilTexture* GameEngineDepthStencilTexture::Create(
    const float4& _scale
)
{
    GameEngineDepthStencilTexture* newRes = CreateUnnamedRes();
    newRes->CreateDepthTexture(_scale);
    return newRes;
}

void GameEngineDepthStencilTexture::CreateDepthTexture(const float4& _scale)
{
    //typedef struct D3D11_TEXTURE2D_DESC
    //{
    //    UINT Width;                   ������ �ؽ�ó ���α���.
    //    UINT Height;                  ������ �ؽ�ó ���α���.
    //    UINT MipLevels;               �ؽ�ó�� �ִ� �Ӹ� ����??
    //    UINT ArraySize;               ??
    //    DXGI_FORMAT Format;           �ؽ�ó ����.
    //    DXGI_SAMPLE_DESC SampleDesc;  ���ø� �ɼ�.
    // 
    //      typedef struct DXGI_SAMPLE_DESC
    //      {
    //          UINT Count;     ���ø��� �� �� �ȼ��� ���� ����.
    //          UINT Quality;   ���ø� ����. 0~1 ����.
    //      } DXGI_SAMPLE_DESC;
    // 
    //    D3D11_USAGE Usage;            �ؽ�ó ���ó.
    //    UINT BindFlags;               ������ ���������ο����� �ؽ��� ��� ���.
    //    UINT CPUAccessFlags;          �� �ؽ�ó�� CPU ���ٱ��� ����.
    //    UINT MiscFlags;               �� �� �ΰ� �ɼ�.
    //} 	D3D11_TEXTURE2D_DESC;

    D3D11_TEXTURE2D_DESC desc = { 0 };

    desc.Width = _scale.UIX();
    desc.Height = _scale.UIY();
    desc.MipLevels = 1;
    //1: �Ӹ��� �� �� ������ ���ڴ�.

    desc.ArraySize = 1;
    //1: �ؽ�ó�� �Ѱ��� ����ڴ�.

    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    //DXGI_FORMAT_D24_UNORM_S8_UINT:
    //8����Ʈ�� 0~1�� ����ȣ ����ȭ�� float�� 3���� ���̰� ���������, 
    //8����Ʈ UINT�� �Ѱ��� ���ٽǰ� ��������� ���.

    desc.SampleDesc.Count = 1;
    //���ø��� ����� ���� ����: 1 == ���ø� ����.

    desc.SampleDesc.Quality = 0;
    //���ø� ����: 0 == ���ø� �� ��.

    desc.Usage = D3D11_USAGE_DEFAULT;
    //D3D11_USAGE_DEFAULT: �� �ؽ�ó���� GPU�� �а��� ����.

    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    //D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL: ������ ���������ο� ���� ���ٽ� Ÿ������ ���.

    desc.CPUAccessFlags = 0;
    //0: CPU�� �� �ؽ�ó�� �а��� �� �� �Ұ���.

    desc.MiscFlags = 0;
    //0: �ΰ� �ɼ� �ƹ��͵� �������� ����.

    depthBuffer_ = GameEngineTexture::Create(desc);
}

ID3D11DepthStencilView* GameEngineDepthStencilTexture::CreateDepthStencilView()
{
    return depthBuffer_->CreateDepthStencilView();
}
