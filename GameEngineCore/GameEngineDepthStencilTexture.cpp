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
    //    UINT Width;                   생성할 텍스처 가로길이.
    //    UINT Height;                  생성할 텍스처 세로길이.
    //    UINT MipLevels;               텍스처의 최대 밉맵 레벨??
    //    UINT ArraySize;               ??
    //    DXGI_FORMAT Format;           텍스처 포맷.
    //    DXGI_SAMPLE_DESC SampleDesc;  샘플링 옵션.
    // 
    //      typedef struct DXGI_SAMPLE_DESC
    //      {
    //          UINT Count;     샘플링할 때 쓸 픽셀당 샘플 갯수.
    //          UINT Quality;   샘플링 수준. 0~1 사이.
    //      } DXGI_SAMPLE_DESC;
    // 
    //    D3D11_USAGE Usage;            텍스처 사용처.
    //    UINT BindFlags;               렌더링 파이프라인에서의 텍스쳐 사용 방식.
    //    UINT CPUAccessFlags;          이 텍스처에 CPU 접근권한 설정.
    //    UINT MiscFlags;               그 외 부가 옵션.
    //} 	D3D11_TEXTURE2D_DESC;

    D3D11_TEXTURE2D_DESC desc = { 0 };

    desc.Width = _scale.UIX();
    desc.Height = _scale.UIY();
    desc.MipLevels = 1;
    //1: 밉맵을 쓸 수 있으면 쓰겠다.

    desc.ArraySize = 1;
    //1: 텍스처를 한개만 만들겠다.

    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    //DXGI_FORMAT_D24_UNORM_S8_UINT:
    //8바이트의 0~1로 무부호 정규화된 float값 3개를 깊이값 저장용으로, 
    //8바이트 UINT값 한개를 스텐실값 저장용으로 사용.

    desc.SampleDesc.Count = 1;
    //샘플링때 사용할 샘플 갯수: 1 == 샘플링 안함.

    desc.SampleDesc.Quality = 0;
    //샘플링 수준: 0 == 샘플링 안 함.

    desc.Usage = D3D11_USAGE_DEFAULT;
    //D3D11_USAGE_DEFAULT: 이 텍스처에는 GPU만 읽고쓰기 가능.

    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    //D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL: 렌더링 파이프라인에 깊이 스텐실 타겟으로 사용.

    desc.CPUAccessFlags = 0;
    //0: CPU는 이 텍스처에 읽고쓰기 둘 다 불가능.

    desc.MiscFlags = 0;
    //0: 부가 옵션 아무것도 적용하지 않음.

    depthBuffer_ = GameEngineTexture::Create(desc);
}

ID3D11DepthStencilView* GameEngineDepthStencilTexture::CreateDepthStencilView()
{
    return depthBuffer_->CreateDepthStencilView();
}
