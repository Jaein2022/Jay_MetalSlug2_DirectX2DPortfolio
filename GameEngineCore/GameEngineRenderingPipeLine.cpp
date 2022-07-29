#include "PreCompile.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineDevice.h"
#include "GameEngineInputLayout.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineRasterizer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineBlend.h"

GameEngineRenderingPipeLine::GameEngineRenderingPipeLine()
	: vertexBuffer_(nullptr),
	inputLayout_(nullptr),
	vertexShader_(nullptr),
	topology_(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	indexBuffer_(nullptr),
	rasterizer_(nullptr),
	pixelShader_(nullptr),
	depthStencil_(nullptr),
	blend_(nullptr)
{
}

GameEngineRenderingPipeLine::~GameEngineRenderingPipeLine()
{
	//�ٸ� ���ҽ����� �����͸� ������ ���� ������ ���� �����ϸ� �ȵ�����, 
	//��ǲ���̾ƿ��� ������ ������������ ���� �����ϹǷ� ������ ���� �ؾ��Ѵ�.
	if (nullptr != inputLayout_)
	{
		delete inputLayout_;
		inputLayout_ = nullptr;
	}
}

GameEngineRenderingPipeLine* GameEngineRenderingPipeLine::Create(const std::string& _name)
{
	return CreateNamedRes(_name);
}

void GameEngineRenderingPipeLine::SetVertexBuffer_InputAssembler1(const std::string& _name)
{
	this->vertexBuffer_ = GameEngineVertexBuffer::Find(_name);

	if (nullptr == vertexBuffer_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ���ؽ����۰� �������� �ʽ��ϴ�.");
		return;
	}

	if (nullptr != inputLayout_)
	{
		delete inputLayout_;
		inputLayout_ = nullptr;
	}

	if (nullptr == inputLayout_ && nullptr != vertexShader_)
		//���ؽ����ۿ� ���ؽ����̴��� �غ�� ���¿��� ��ǲ���̾ƿ��� ���ٸ� �� �� �����Ѵ�.
	{
		//inputLayout_ = GameEngineInputLayout::CreateNamedRes("");
		//���ӿ�����ǲ���̾ƿ��� �� Ŭ������ ������� �θ� �̷��Ե� �����ϱ� �ϴ�.
		//���� �׷��� �ٲ�ٸ� ���� �����ؼ� �ȵȴ�.

		inputLayout_ = new GameEngineInputLayout();
		inputLayout_->CreateInputLayout(*(vertexBuffer_->GetInputLayoutInfo()), vertexShader_);
	}
}

void GameEngineRenderingPipeLine::SetVertexShader(const std::string& _name)
{
	this->vertexShader_ = GameEngineVertexShader::Find(_name);

	if (nullptr == vertexShader_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ���ؽ����̴��� �������� �ʽ��ϴ�.");
		return;
	}

	if (nullptr != inputLayout_)
	{
		delete inputLayout_;
		inputLayout_ = nullptr;
	}

	if (nullptr == inputLayout_ && nullptr != vertexBuffer_)
		//���ؽ����ۿ� ���ؽ����̴��� �غ�� ���¿��� ��ǲ���̾ƿ��� ���ٸ� �� �� �����Ѵ�.
	{
		//inputLayout_ = GameEngineInputLayout::CreateNamedRes("");
		//���ӿ�����ǲ���̾ƿ��� �� Ŭ������ ������� �θ� �̷��Ե� �����ϱ� �ϴ�.
		//���� �׷��� �ٲ�ٸ� ���� �����ؼ� �ȵȴ�.

		inputLayout_ = new GameEngineInputLayout();
		inputLayout_->CreateInputLayout(*(vertexBuffer_->GetInputLayoutInfo()), vertexShader_);
	}
}

void GameEngineRenderingPipeLine::SetIndexBuffer_InputAssembler2(const std::string& _name)
{
	this->indexBuffer_ = GameEngineIndexBuffer::Find(_name);

	if (nullptr == indexBuffer_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� �ε������۰� �������� �ʽ��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string& _name)
{
	this->rasterizer_ = GameEngineRasterizer::Find(_name);

	if (nullptr == rasterizer_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� �����Ͷ������� �������� �ʽ��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetPixelShader(const std::string& _name)
{
	this->pixelShader_ = GameEnginePixelShader::Find(_name);

	if (nullptr == pixelShader_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� �ȼ����̴��� �������� �ʽ��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetDepthStencil_OutputMerger(const std::string& _name)
{
	this->depthStencil_ = GameEngineDepthStencil::Find(_name);

	if (nullptr == depthStencil_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ���̽��ٽ��� �������� �ʽ��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetBlend_OutputMerger(const std::string& _name)
{
	this->blend_ = GameEngineBlend::Find(_name);

	if (nullptr == blend_)
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ���尡 �������� �ʽ��ϴ�.");
		return;
	}
}

void GameEngineRenderingPipeLine::Rendering()
{
	InputAssembler1_VertexBufferSetting();
	VertexShaderSetting();
	InputAssembler2_IndexBufferSetting();
	RasterizerSetting();
	PixelShaderSetting();
	OutputMerger_BlendSetting();
	OutputMerger_DepthStencilSetting();

	//��� ������ ���ҽ� ������ ���� �Ŀ� �������� �Ѵ�.
	this->Draw();
}

void GameEngineRenderingPipeLine::InputAssembler1_VertexBufferSetting()
{
	this->inputLayout_->Setting();
	this->vertexBuffer_->Setting();
}

void GameEngineRenderingPipeLine::VertexShaderSetting()
{
	this->vertexShader_->Setting();
}

void GameEngineRenderingPipeLine::InputAssembler2_IndexBufferSetting()
{
	this->indexBuffer_->Setting();

	GameEngineDevice::GetContext()->IASetPrimitiveTopology(topology_);
	//�⺻ �������� ����: D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST.
}

void GameEngineRenderingPipeLine::RasterizerSetting()
{
	this->rasterizer_->Setting();
}

void GameEngineRenderingPipeLine::PixelShaderSetting()
{
	this->pixelShader_->Setting();
}

void GameEngineRenderingPipeLine::OutputMerger_BlendSetting()
{
	this->blend_->Setting();
}

void GameEngineRenderingPipeLine::OutputMerger_DepthStencilSetting()
{
	this->depthStencil_->Setting();
}

void GameEngineRenderingPipeLine::Draw()
{
	GameEngineDevice::GetContext()->DrawIndexed(	//�ε����� ������ �������� �׸��� �Լ�.
		this->indexBuffer_->GetIndexCount(),//�ε��� ����.
		0,									//�б� ������ �ε��� ������ ���� ��ȣ. 
		0									//�б� ������ ���ؽ� ������ ���� ��ȣ. 
	);
	//���� ����Ϸ��� ����� ����Ÿ���� ��� �˰� �ű�� �׸��°���??
	//->GameEngineRenderTarget Ŭ������ Setting()�Լ� �ȿ� �ִ� ID3D11DeviceContext::OMSetRenderTargets()�Լ��� ����
	// ������������������ �ƿ�ǲ���� �ܰ迡 ���� ���ϴ� ����Ÿ���� �����Ű�� �� �Ŀ� ������ ������ ������������ �����ϹǷ�
	// ���� ������ ����ۿ� ����Ÿ�ٿ� ���� ���ϴ´�� �׸� �� �ִ�.
	//������ ���������� ���ҽ� ���ÿ� ������ ���� �߿����� �ʴٴ� �����̱⵵ �ϴ�.
}