#pragma once
#include "GameEngineRes.h"

class GameEngineInputLayout;
class GameEngineVertexBuffer;
class GameEngineVertexShader;
class GameEngineIndexBuffer;
class GameEngineRasterizer;
class GameEnginePixelShader;
class GameEngineDepthStencil;
class GameEngineBlend;
class GameEngineRenderingPipeLine : public GameEngineRes<GameEngineRenderingPipeLine>
{
	//이 클래스의 존재 이유는??

public:
	GameEngineRenderingPipeLine();
	~GameEngineRenderingPipeLine();

protected:
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete;
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept = delete;

private:
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete;
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete;


public:

	static GameEngineRenderingPipeLine* Create(const std::string& _name);
	static GameEngineRenderingPipeLine* Create();
	static void AllShaderReset();

public:

	//세팅 준비 함수들.
	//void SetInputLayout_InputAssembler1(const std::string& _name);
	// 인풋레이아웃은 따로 생성하지 않고, 버텍스버퍼와 버텍스셰이더가 둘 다 갖추어지면 인풋레이아웃이 생성되게 한다.

	void SetVertexBuffer_InputAssembler1(const std::string& _name);
	void SetVertexShader(const std::string& _name);	//버텍스버퍼와 버텍스세이더 세팅이 끝나면 인풋레이아웃은 자동으로 생성, 세팅된다
	void SetIndexBuffer_InputAssembler2(const std::string& _name);
	void SetRasterizer(const std::string& _name);
	void SetPixelShader(const std::string& _name);
	void SetBlend_OutputMerger(const std::string& _name);
	void SetDepthStencil_OutputMerger(const std::string& _name);

	void Rendering();
	void Copy(GameEngineRenderingPipeLine* _original);

public:
	inline GameEngineVertexBuffer* GetVertexBuffer()
	{
		return this->vertexBuffer_;
	}
	inline GameEngineVertexShader* GetVertexShader()
	{
		return vertexShader_;
	}
	inline GameEnginePixelShader* GetPixelShader()
	{
		return pixelShader_;
	}

private:
	//세팅 함수들.
	//실질적으로 세팅 순서는 그다지 중요하지 않다. DrawIndexed() 함수 호출 이전까지만 모든 세팅이 끝나있으면 된다.

	void InputAssembler1_VertexBufferSetting();
	void VertexShaderSetting();
	void InputAssembler2_IndexBufferSetting();		//인덱스버퍼와 토폴로지 세팅까지 여기서.
	void RasterizerSetting();
	void PixelShaderSetting();
	void OutputMerger_DepthStencilSetting();
	void OutputMerger_BlendSetting();

	void Draw();

private:
	GameEngineVertexBuffer* vertexBuffer_;
	GameEngineInputLayout* inputLayout_;

	GameEngineVertexShader* vertexShader_;

	GameEngineIndexBuffer* indexBuffer_;
	D3D11_PRIMITIVE_TOPOLOGY topology_;

	GameEngineRasterizer* rasterizer_;

	GameEnginePixelShader* pixelShader_;

	GameEngineDepthStencil* depthStencil_;
	GameEngineBlend* blend_;

};

