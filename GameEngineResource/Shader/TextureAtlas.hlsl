#include "TransformHeader.hlsli"

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
}

Output TextureAtlas_VS(Input _input)
{
    Output newOutput = (Output) 0; //Output타입 변수 newOutput을 0으로 초기화.
    //HLSL의 경우에는 대부분의 상황에서 형변환이 가능하다.

    newOutput.pos_ = mul(_input.pos_, worldViewProjectionMatrix_); //WVP행렬 적용.
    //왜 이 부분을 주석처리하면 상수버퍼가 사라지는지 이유 파악할 것.
    //예상 이유 1: 이 부분이 주석처리되면 상수버퍼를 사용하지 않고, 사용하지 않는 상수버퍼는 컴파일되지 않아서 없어지는 것.
    //->해결 방안: 어떤 식으로든 상수버퍼를 사용한다.
    
    //newOutput.pos_.w = 1.f;     <- 이게 있으면 투영행렬의 결과값이 왜곡되어 어떤 오브젝트든 화면 전체를 뒤덮는다.
    //이유가 뭘까?? 반드시 직접 계산해볼 것.
    
    newOutput.posLocal_ = _input.pos_; //WVP행렬 비적용. 최초 정점좌표(-0.5~0.5 사이 좌표) 유지.
    
    newOutput.texcoord_.x = (_input.texcoord_.x * textureFrameSize_.x) + textureFramePos_.x;
    newOutput.texcoord_.y = (_input.texcoord_.y * textureFrameSize_.y) + textureFramePos_.y;
    
    return newOutput;
}

cbuffer ColorData : register(b0)
{
    float4 mulColor_;
    float4 plusColor_;
};

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 TextureAtlas_PS(Output _input) : SV_Target0 //SV_Target[n]: n번 렌더타겟에 결과값을 저장한다.
{
    return (Tex.Sample(Smp, _input.texcoord_.xy) * mulColor_) + plusColor_;
}