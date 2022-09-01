#include "TransformHeader.hlsli"
#include "RenderOption.hlsli"

struct Input
{
    //멤버변수간 순서는 전혀 중요하지 않다. 인풋 레이아웃을 통해 각 멤버변수의 오프셋값만 정확히 알려주면 된다?
    //단지 편의를 위해서 되도록 순서를 지킬 뿐이다?
    float4 pos_ : POSITION;
    float4 texcoord_ : TEXCOORD;
};

struct Output
{
    //시맨틱(semantic). GPU에게 알려주는, 해당 값의 종류. GPU가 자동으로 처리할 수 있는 종류의 값이면 자동으로 처리해준다.
    float4 pos_ : SV_Position; //SV_Position: 이 시맨틱을 통해서만이 버텍스 셰이더에서 변경된 정점 위치를 래스터라이저로 전달할 수 있다.
    //SV는 System-Value semantic의 약자로, DirectX10에서부터 도입된, 용도가 정해져 있어서 GPU가 자동으로 처리하는 시맨틱 키워드이다.
    
    float4 posLocal_ : POSITION;    //최초 정점좌표(-0.5~0.5 사이 좌표). 

    float4 texcoord_ : TEXCOORD; //TEXCOORD[n]: 텍스쳐의 UV값을 의미하는 시맨틱네임. 텍스쳐좌표를 뜻하는 Texture Coordinate의 줄임말.

};

cbuffer AtlasData : register(b1)
{
    float2 textureFramePos_;
    float2 textureFrameSize_;
    float4 pivotPos_;
}

Output TextureAtlas_VS(Input _input)
{
    Output result = (Output) 0; //Output타입 변수 newOutput을 0으로 초기화.
    //HLSL의 경우에는 대부분의 상황에서 형변환이 가능하다.

    _input.pos_ += pivotPos_;
    //??
    
    result.pos_ = mul(_input.pos_, worldViewProjectionMatrix_); //WVP행렬 적용.
    
    result.posLocal_ = _input.pos_; 
    
    result.texcoord_.x = (_input.texcoord_.x * textureFrameSize_.x) + textureFramePos_.x;
    result.texcoord_.y = (_input.texcoord_.y * textureFrameSize_.y) + textureFramePos_.y;
    
    return result;
}

cbuffer PixelData : register(b0)
{
    float4 mulColor_;
    float4 plusColor_;
    float4 slice_;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 TextureAtlas_PS(Output _input) : SV_Target0 //SV_Target[n]: n번 렌더타겟에 결과값을 저장한다.
{
    if (_input.texcoord_.x < slice_.x)
    {
        clip(-1);
    }
    
    //return (Tex.Sample(Smp, _input.texcoord_.xy) * mulColor_) + plusColor_;
    
    //이 아래 부분이 없으면 후순위 렌더링오더로 그려지는 것들이 투명 텍스처에 가려서 그려지지 않는다.
    float4 resultColor = (Tex.Sample(Smp, _input.texcoord_.xy) * mulColor_) + plusColor_;
    
    if (resultColor.a <= 0.f)
    {
        clip(-1);
    }
    
    if (resultColor.a >= 1.f)
    {
        resultColor.a = 1.f;
    }
    
    return resultColor;
}