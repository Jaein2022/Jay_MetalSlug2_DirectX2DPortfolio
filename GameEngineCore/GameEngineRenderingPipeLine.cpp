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
	//다른 리소스들은 포인터만 가져다 쓰기 때문에 직접 삭제하면 안되지만, 
	//인풋레이아웃은 렌더링 파이프라인이 직접 생성하므로 삭제도 직접 해야한다.
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
		MsgBoxAssertString(_name + ": 그런 이름의 버텍스버퍼가 존재하지 않습니다.");
		return;
	}

	if (nullptr != inputLayout_)
	{
		delete inputLayout_;
		inputLayout_ = nullptr;
	}

	if (nullptr == inputLayout_ && nullptr != vertexShader_)
		//버텍스버퍼와 버텍스셰이더가 준비된 상태에서 인풋레이아웃이 없다면 그 때 생성한다.
	{
		//inputLayout_ = GameEngineInputLayout::CreateNamedRes("");
		//게임엔진인풋레이아웃이 이 클래스를 프렌드로 두면 이렇게도 가능하긴 하다.
		//만약 그렇게 바뀐다면 직접 삭제해선 안된다.

		inputLayout_ = new GameEngineInputLayout();
		inputLayout_->CreateInputLayout(*(vertexBuffer_->GetInputLayoutInfo()), vertexShader_);
	}
}

void GameEngineRenderingPipeLine::SetVertexShader(const std::string& _name)
{
	this->vertexShader_ = GameEngineVertexShader::Find(_name);

	if (nullptr == vertexShader_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 버텍스셰이더가 존재하지 않습니다.");
		return;
	}

	if (nullptr != inputLayout_)
	{
		delete inputLayout_;
		inputLayout_ = nullptr;
	}

	if (nullptr == inputLayout_ && nullptr != vertexBuffer_)
		//버텍스버퍼와 버텍스셰이더가 준비된 상태에서 인풋레이아웃이 없다면 그 떄 생성한다.
	{
		//inputLayout_ = GameEngineInputLayout::CreateNamedRes("");
		//게임엔진인풋레이아웃이 이 클래스를 프렌드로 두면 이렇게도 가능하긴 하다.
		//만약 그렇게 바뀐다면 직접 삭제해선 안된다.

		inputLayout_ = new GameEngineInputLayout();
		inputLayout_->CreateInputLayout(*(vertexBuffer_->GetInputLayoutInfo()), vertexShader_);
	}
}

void GameEngineRenderingPipeLine::SetIndexBuffer_InputAssembler2(const std::string& _name)
{
	this->indexBuffer_ = GameEngineIndexBuffer::Find(_name);

	if (nullptr == indexBuffer_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 인덱스버퍼가 존재하지 않습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetRasterizer(const std::string& _name)
{
	this->rasterizer_ = GameEngineRasterizer::Find(_name);

	if (nullptr == rasterizer_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 래스터라이저가 존재하지 않습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetPixelShader(const std::string& _name)
{
	this->pixelShader_ = GameEnginePixelShader::Find(_name);

	if (nullptr == pixelShader_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 픽셀셰이더가 존재하지 않습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetDepthStencil_OutputMerger(const std::string& _name)
{
	this->depthStencil_ = GameEngineDepthStencil::Find(_name);

	if (nullptr == depthStencil_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 깊이스텐실이 존재하지 않습니다.");
		return;
	}
}

void GameEngineRenderingPipeLine::SetBlend_OutputMerger(const std::string& _name)
{
	this->blend_ = GameEngineBlend::Find(_name);

	if (nullptr == blend_)
	{
		MsgBoxAssertString(_name + ": 그런 이름의 블렌드가 존재하지 않습니다.");
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

	//모든 렌더링 리소스 세팅이 끝난 후에 렌더링을 한다.
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
	//기본 토폴로지 세팅: D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST.
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
	GameEngineDevice::GetContext()->DrawIndexed(	//인덱스에 연동된 정점들을 그리는 함수.
		this->indexBuffer_->GetIndexCount(),//인덱스 개수.
		0,									//읽기 시작할 인덱스 버퍼의 원소 번호. 
		0									//읽기 시작할 버텍스 버퍼의 원소 번호. 
	);
	//내가 사용하려는 백버퍼 렌더타겟을 어떻게 알고 거기다 그리는거지??
	//->GameEngineRenderTarget 클래스의 Setting()함수 안에 있는 ID3D11DeviceContext::OMSetRenderTargets()함수를 통해
	// 렌더링파이프라인의 아웃풋머져 단계에 내가 원하는 렌더타겟을 연결시키고 그 후에 나머지 렌더링 파이프라인을 진행하므로
	// 내가 지정한 백버퍼용 렌더타겟에 내가 원하는대로 그릴 수 있다.
	//렌더링 파이프라인 리소스 세팅에 순서는 별로 중요하지 않다는 증거이기도 하다.
}