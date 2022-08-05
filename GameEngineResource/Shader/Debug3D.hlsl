#include "TransformHeader.hlsli"

struct Input
{
    float4 pos_ : POSITION;
};

struct Output
{
    float4 pos_ : SV_Position;
    float4 posLocal_ : POSITION;
};

Output Debug3D_VS(Input _input)
{
    Output newOutput = (Output) 0;
    newOutput.pos_ = mul(_input.pos_, worldViewProjectionMatrix_);
    newOutput.posLocal_ = _input.pos_;
    return newOutput;
}

cbuffer DebugInfo : register(b8)
{
    int4 type_;
    float4 color_;
}

float4 Debug3D_PS(Output _input): SV_Target0
{
    return color_;
}