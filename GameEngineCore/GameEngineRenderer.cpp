#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineCamera.h"
#include "GameEngineShaderResourceHelper.h"

GameEngineRenderer::GameEngineRenderer()
	: camera_(nullptr),
	renderOption_(),
	cameraOrder_(CameraOrder::MainCamera),
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
//	//임시 렌더링 코드.
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
//		//tempIndex에 저장된 인덱스를 triIndex라는 이름으로 옮긴 후 
//		//triIndex에 대응되는 버텍스 정보를 순서대로 copiedVertex로 옮긴다.
//
//		copiedVertex[i] *= this->GetTransform().GetWorldViewProjection();	
//		//최종 world행렬에 뷰행렬, 투영행렬까지 적용된 값을 copiedVertex[i]에 반영한다.
//
//		copiedVertex[i] /=  copiedVertex[i].w;
//		//투영행렬의 결과값이 저장된 w수치만큼 copiedVertex의 좌표값을 나누어 축소시킨다.
//		
//		copiedVertex[i] *= tempViewport_;
//		//임시뷰포트 행렬을 적용해서 원하는 크기대로 다시 확대된 좌표를 정점에 입력한다.
//	
//		drawVertex[i] = copiedVertex[i].ConvertToWindowsPoint();
//		//입력된 좌표대로 POINT 구조체를 만들어서 전달.
//	}
//
//	for (size_t i = 0; i < drawVertex.size(); i += 3)
//	{
//		Polygon(GameEngineWindow::GetHDC(), &drawVertex[i], 3);
//	}
//
//}