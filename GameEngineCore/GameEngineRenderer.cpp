#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineCamera.h"
#include "GameEngineShaderResourceHelper.h"

GameEngineRenderer::GameEngineRenderer()
	: camera_(nullptr),
	cameraOrder_(CameraOrder::MainCamera),
	renderOption_(),
	renderingOrder_(0)
{
}

GameEngineRenderer::~GameEngineRenderer()
{
}

void GameEngineRenderer::ChangeCamera(CameraOrder _order)
{
	this->GetActor()->GetLevel()->PushRenderer(this, _order);
}

GameEngineRenderingPipeLine* GameEngineRenderer::ClonePipeLine(GameEngineRenderingPipeLine* _renderingPipeLine)
{
	GameEngineRenderingPipeLine* clone = GameEngineRenderingPipeLine::Create();
	clone->Copy(_renderingPipeLine);
	return clone;
}

void GameEngineRenderer::SetRenderingOrder(int _renderingOrder)
{
	camera_->ChangeRenderingOrder(this, _renderingOrder);
}

void GameEngineRenderer::EngineShaderResourceSetting(GameEngineShaderResourceHelper* _shaderResourceHelper)
{
	if (true == _shaderResourceHelper->IsConstantBuffer("TRANSFORMDATA"))
	{
		_shaderResourceHelper->SetConstantBuffer_Link(
			"TRANSFORMDATA",
			&GetTransformData(),
			sizeof(GetTransformData())
		);
	}
	if (true == _shaderResourceHelper->IsConstantBuffer("RENDEROPTION"))
	{
		_shaderResourceHelper->SetConstantBuffer_Link(
			"RENDEROPTION",
			&renderOption_,
			sizeof(renderOption_)
		);
	}
}

void GameEngineRenderer::Start()
{
}

void GameEngineRenderer::PushRendererToMainCamera()
{
	this->GetActor()->GetLevel()->PushRendererToMainCamera(this);
}

void GameEngineRenderer::PushRendererToUICamera()
{
	this->GetActor()->GetLevel()->PushRendererToUICamera(this);
}

//void GameEngineRenderer::Render(float _deltaTime)
//{
//	//�ӽ� ������ �ڵ�.
//	GameEngineVertexBuffer* renderVertex = GameEngineVertexBuffer::Find("BoxVertex");
//	GameEngineIndexBuffer* renderIndex = GameEngineIndexBuffer::Find("BoxIndex");
//
//	std::vector<POINT> drawVertex;
//	drawVertex.resize(renderIndex->indexes_.size());
//
//	std::vector<float4> copiedVertex;
//	copiedVertex.resize(renderIndex->indexes_.size());
//
//	for (size_t i = 0; i < renderIndex->indexes_.size(); i++)
//	{
//		int triIndex = renderIndex->indexes_[i];
//		copiedVertex[i] = renderVertex->vertexes_[triIndex];
//		//tempIndex�� ����� �ε����� triIndex��� �̸����� �ű� �� 
//		//triIndex�� �����Ǵ� ���ؽ� ������ ������� copiedVertex�� �ű��.
//
//		copiedVertex[i] *= this->GetTransform().GetWorldViewProjection();	
//		//���� world��Ŀ� �����, ������ı��� ����� ���� copiedVertex[i]�� �ݿ��Ѵ�.
//
//		copiedVertex[i] /=  copiedVertex[i].w;
//		//��������� ������� ����� w��ġ��ŭ copiedVertex�� ��ǥ���� ������ ��ҽ�Ų��.
//		
//		copiedVertex[i] *= tempViewport_;
//		//�ӽú���Ʈ ����� �����ؼ� ���ϴ� ũ���� �ٽ� Ȯ��� ��ǥ�� ������ �Է��Ѵ�.
//	
//		drawVertex[i] = copiedVertex[i].ConvertToWindowsPoint();
//		//�Էµ� ��ǥ��� POINT ����ü�� ���� ����.
//	}
//
//	for (size_t i = 0; i < drawVertex.size(); i += 3)
//	{
//		Polygon(GameEngineWindow::GetHDC(), &drawVertex[i], 3);
//	}
//
//}