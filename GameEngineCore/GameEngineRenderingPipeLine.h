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
	//�� Ŭ������ ���� ������??

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

	//���� �غ� �Լ���.
	//void SetInputLayout_InputAssembler1(const std::string& _name);
	// ��ǲ���̾ƿ��� ���� �������� �ʰ�, ���ؽ����ۿ� ���ؽ����̴��� �� �� ���߾����� ��ǲ���̾ƿ��� �����ǰ� �Ѵ�.

	void SetVertexBuffer_InputAssembler1(const std::string& _name);
	void SetVertexShader(const std::string& _name);	//���ؽ����ۿ� ���ؽ����̴� ������ ������ ��ǲ���̾ƿ��� �ڵ����� ����, ���õȴ�
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
	//���� �Լ���.
	//���������� ���� ������ �״��� �߿����� �ʴ�. DrawIndexed() �Լ� ȣ�� ���������� ��� ������ ���������� �ȴ�.

	void InputAssembler1_VertexBufferSetting();
	void VertexShaderSetting();
	void InputAssembler2_IndexBufferSetting();		//�ε������ۿ� �������� ���ñ��� ���⼭.
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

