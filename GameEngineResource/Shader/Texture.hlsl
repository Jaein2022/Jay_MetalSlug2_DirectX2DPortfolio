#include "TransformHeader.hlsli"

struct Input
{
    //��������� ������ ���� �߿����� �ʴ�. ��ǲ ���̾ƿ��� ���� �� ��������� �����°��� ��Ȯ�� �˷��ָ� �ȴ�??
    //���� ���Ǹ� ���ؼ� �ǵ��� ������ ��ų ���̴�??
    float4 pos_ : POSITION;
    float4 texcoord_ : TEXCOORD;
};

struct Output
{
    //�ø�ƽ(semantic). GPU���� �˷��ִ�, �ش� ���� ����. GPU�� �ڵ����� ó���� �� �ִ� ������ ���̸� �ڵ����� ó�����ش�.
    float4 pos_ : SV_Position; //SV_Position: �� �ø�ƽ�� ���ؼ����� ���ؽ� ���̴����� ����� ���� ��ġ�� �����Ͷ������� ������ �� �ִ�.
    //SV�� System-Value semantic�� ���ڷ�, DirectX10�������� ���Ե�, �뵵�� ������ �־ GPU�� �ڵ����� ó���ϴ� �ø�ƽ Ű�����̴�.
    
    float4 posLocal_ : POSITION;    //���� ������ǥ(-0.5~0.5 ���� ��ǥ). 

    float4 texcoord_ : TEXCOORD; //TEXCOORD[n]: �ؽ����� UV���� �ǹ��ϴ� �ø�ƽ����. �ؽ�����ǥ�� ���ϴ� Texture Coordinate�� ���Ӹ�.

};

Output Texture_VS(Input _input)
{
    Output newOutput = (Output) 0; //OutputŸ�� ���� newOutput�� 0���� �ʱ�ȭ.
    //HLSL�� ��쿡�� ��κ��� ��Ȳ���� ����ȯ�� �����ϴ�.

    newOutput.pos_ = mul(_input.pos_, worldViewProjectionMatrix_); //WVP��� ����.
    //�� �� �κ��� �ּ�ó���ϸ� ������۰� ��������� ���� �ľ��� ��.
    //���� ���� 1: �� �κ��� �ּ�ó���Ǹ� ������۸� ������� �ʰ�, ������� �ʴ� ������۴� �����ϵ��� �ʾƼ� �������� ��.
    //->�ذ� ���: � �����ε� ������۸� ����Ѵ�.
    
    //newOutput.pos_.w = 1.f;   <- �̰� ������ ��������� ������� �ְ�Ǿ� � ������Ʈ�� ȭ�� ��ü�� �ڵ��´�.
    //������ ����?? �ݵ�� ���� ����غ� ��.
    
    newOutput.posLocal_ = _input.pos_; //WVP��� ������. ���� ������ǥ(-0.5~0.5 ���� ��ǥ) ����.
    
    newOutput.texcoord_ = _input.texcoord_;
    
    return newOutput;
}

Texture2D Tex : register(t0);
SamplerState Smp : register(s0);

float4 Texture_PS(Output _input) : SV_Target0 //SV_Target[n]: n�� ����Ÿ�ٿ� ������� �����Ѵ�.
{   
    float4 color = Tex.Sample(Smp, _input.texcoord_.xy);
    
    if (color.a <= 0.f)
    {
        clip(-1);
    }

    return color;
}