#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"

GameEngineRenderer::GameEngineRenderer(): cameraOrder_(CameraOrder::UICamera)
{
}

GameEngineRenderer::~GameEngineRenderer()
{
}

void GameEngineRenderer::ChangeCamera(CameraOrder _order)
{
	this->GetActor()->GetLevel()->PushRenderer(this, _order);
}

void GameEngineRenderer::Start()
{
}

void GameEngineRenderer::PushRendererToMainCamera()
{
	this->GetParent<GameEngineActor>()->GetLevel()->PushRendererToMainCamera(this);
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