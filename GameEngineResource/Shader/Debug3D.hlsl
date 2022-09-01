#include "TransformHeader.hlsli"

struct Input
{
    float4 pos_ : POSITION;
};

struct Output
{
    float4 pos_ : SV_Position;
    float4 posWorld_ : POSITION;
};

Output Debug3D_VS(Input _input)
{
    Output newOutput = (Output) 0;
    newOutput.pos_ = mul(_input.pos_, worldViewProjectionMatrix_);
    newOutput.posWorld_.xyz = _input.pos_.xyz;
    newOutput.posWorld_.w = 1.f;
    return newOutput;
}

cbuffer DebugInfo : register(b8)
{
    int4 type_;
    float4 color_;
}

float4 Debug3D_PS(Output _input): SV_Target0
{
    if (type_.x == 0 && length(_input.posWorld_.xyz) > 0.7f)
    {
        clip(-1);
    }
    
    return color_;
    
    //float4 resultColor = color_;
    //
    //if (resultColor.a <= 0.f)
    //{
    //    clip(-1);
    //}
    //
    //if (resultColor.a >= 1.f)
    //{
    //    resultColor.a = 1.f;
    //}
    //
    //return resultColor;
}