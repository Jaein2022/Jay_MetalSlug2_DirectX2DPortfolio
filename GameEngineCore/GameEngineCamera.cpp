#include "PreCompile.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineDevice.h"

GameEngineCamera::GameEngineCamera()
	: size_(GameEngineWindow::GetScale()),
	projectionMode_(CameraProjectionMode::Perspective),
	nearZ_(0.1f),
	farZ_(1000.f),
	fovAngleY_(60.f)
{
	viewportDesc_.TopLeftX = 0;
	viewportDesc_.TopLeftY = 0;
	viewportDesc_.Width = size_.x;
	viewportDesc_.Height = size_.y;
	viewportDesc_.MinDepth = 0.0f;
	viewportDesc_.MaxDepth = 1.0f;	//<-1.f�� ���ԵǾ� MinDepth�� ������ ���ܾ� �����׽�Ʈ�� ����� �� �� �ִ�.
}

GameEngineCamera::~GameEngineCamera()
{
}

float4 GameEngineCamera::GetMouseScreenPosition()
{
	POINT pointerPosition;
	if (false == GetCursorPos(&pointerPosition))
		//ȭ�� ��ü ���� ���콺 �������� ��ǥ�� ������ ��ũ�� ��ǥ(�»�� 0, 0)�� ��ȯ�ϴ� �Լ�.
	{
		MsgBoxAssert("���콺 ������ ��ǥ�� �����µ� �����߽��ϴ�.");
		return float4::Zero;
	}

	if (false == ScreenToClient(	//ȭ�� ��ü ���� ���콺 ������ ��ǥ�� Ư�� ������ ���� ��ǥ�� ��ȯ�ϴ� �Լ�.
		GameEngineWindow::GetHWND(),	//���콺 ������ ��ǥ�� �˷��� �ϴ� �������� �ڵ�.
		&pointerPosition	//��ȯ�� ȭ�� ��ü���� ���콺 ������ ��ǥ.
	))
	{
		MsgBoxAssert("���콺 ������ ��ǥ�� ��ȯ�ϴµ� �����߽��ϴ�.");
		return float4::Zero;
	}

	return float4(static_cast<float>(pointerPosition.x), static_cast<float>(pointerPosition.y));
}

float4 GameEngineCamera::GetMouseWorldPosition()
{
	POINT pointerPosition;
	if (false == GetCursorPos(&pointerPosition))
		//ȭ�� ��ü ���� ���콺 �������� ��ǥ�� ������ ��ũ�� ��ǥ(�»�� 0, 0)�� ��ȯ�ϴ� �Լ�.
	{
		MsgBoxAssert("���콺 ������ ��ǥ�� �����µ� �����߽��ϴ�.");
		return float4();
	}

	if (false == ScreenToClient(	//ȭ�� ��ü ���� ���콺 ������ ��ǥ�� Ư�� ������ ���� ��ǥ�� ��ȯ�ϴ� �Լ�.
		GameEngineWindow::GetHWND(),	//���콺 ������ ��ǥ�� �˷��� �ϴ� �������� �ڵ�.
		&pointerPosition	//��ȯ�� ȭ�� ��ü���� ���콺 ������ ��ǥ.
	))
	{
		MsgBoxAssert("���콺 ������ ��ǥ�� ��ȯ�ϴµ� �����߽��ϴ�.");
		return float4();
	}

	float4 pointerPos = float4(static_cast<float>(pointerPosition.x), static_cast<float>(pointerPosition.y));

	float4x4 invertedViewportMatrix;
	invertedViewportMatrix.Viewport(size_.x, size_.y, 0.f, 0.f, 0.f, 1.f);
	invertedViewportMatrix.Inverse();

	float4x4 invertedProjectionMatrix = this->projectionMatrix_.InverseReturn();

	pointerPos *= invertedViewportMatrix;
	pointerPos *= invertedProjectionMatrix;

	return pointerPos;
}

float4 GameEngineCamera::GetMouseWorldPositionToActor()
{
	return GetTransform().GetWorldPosition() + GetMouseWorldPosition();
}

void GameEngineCamera::SetCameraOrder(CameraOrder _order)
{
	this->GetActor()->GetLevel()->PushCamera(this, _order);
}

void GameEngineCamera::Start()
{
}

bool ZSort(GameEngineRenderer* _rendererA, GameEngineRenderer* _rendererB)
{
	//
	return _rendererA->GetTransform().GetWorldPosition().z > _rendererB->GetTransform().GetWorldPosition().z;
}

void GameEngineCamera::Render(float _deltaTime)
{
	GameEngineDevice::GetContext()->RSSetViewports(//���������ο� ����Ʈ���� �����ϴ� �Լ�.
		1,					//������ ����Ʈ ����.
		&viewportDesc_		//����Ʈ ����ü �迭�� �ּҰ�.
	);

	//������Ʈ���� ���ġ�� ������� ���Ѵ�.
	viewMatrix_.LookToLH(
		this->GetParent<GameEngineActor>()->GetTransform().GetLocalPosition(),
		this->GetParent<GameEngineActor>()->GetTransform().GetForwardVector(),
		this->GetParent<GameEngineActor>()->GetTransform().GetUpVector()
	);

	//������忡 �°� ������Ʈ���� ����� ��������� ���Ѵ�.
	switch (projectionMode_)
	{
	case CameraProjectionMode::Perspective:
		//projection_.ProjectPerspectiveLH(
		//	planeSize_.x * 0.0001f,	 //������� ���α���.
		//	planeSize_.y * 0.0001f,	 //������� ���α���.  
		//	nearZ_,
		//	farZ_
		//);
		//������� ���� ����ؼ� �־���� �ϴ� ProjectPerspectiveLH()���� 
		//ȭ���� ��Ⱦ��, ������ ������ũ�⸸ �־��ָ� �˾Ƽ� ����ϴ� ProjectPerspectiveFovLH()�� �� ���ϴ�.


		projectionMatrix_.ProjectPerspectiveFovLH(
			fovAngleY_ * GameEngineMath::DegreeToRadian,
			size_.x / size_.y,
			nearZ_,
			farZ_
		);

		break;
	case CameraProjectionMode::Orthographic:
		projectionMatrix_.ProjectOrthographicLH(
			size_.x,
			size_.y,
			nearZ_,
			farZ_
		);
		break;


	default:
		break;
	}

	for (std::pair<const int, std::list<GameEngineRenderer*>>& rendererGroupPair : allRenderers_)
	{
		float scaleTime = GameEngineTime::GetDeltaTime(rendererGroupPair.first);

		std::list<GameEngineRenderer*>& sortingRendererList = rendererGroupPair.second;
		sortingRendererList.sort(ZSort);	//<-�̰� �ְ� ������ ���̰� ���淡 ������ ����� �������??

		for (GameEngineRenderer* const renderer : rendererGroupPair.second)
			//�� ��ġ�� const�� renderer�� ����Ű�� �޸� ��ġ�� ������ �� ���� �ϰڴٴ� �ǹ��̴�. 
			//������ renderer�� ����Ű�� �޸𸮰� ���� ���� �󸶵��� ���� �����ϴ�.
		{
			if (false == renderer->IsUpdate())
			{
				continue;
			}

			renderer->GetTransform().SetViewMatrix(viewMatrix_);
			renderer->GetTransform().SetProjectionMatrix(projectionMatrix_);
			//ī�޶� ����� ����İ� ��������� �������� Ʈ�������� �����Ѵ�.

			renderer->GetTransform().CalculateWorldViewProjection();
			//ũ���̰��� ��ȯ�� ��ģ ������Ŀ� ����İ� ������ı��� ����Ѵ�.

			renderer->Render(scaleTime);
			//����Ʈ����� ������ ��� ��� ����� ��ģ ������ �޽��� ȭ�鿡 �׸���.
		}
	}
}

void GameEngineCamera::PushRenderer(GameEngineRenderer* _renderer)
{
	allRenderers_[_renderer->GetOrder()].push_back(_renderer);
	//���ӿ���ī�޶��� �Ҹ��ڰ� ȣ����� ������, allRenderers Ǫ�ù� �������� ���� �����Ҵ�� ������ �������� 
	// �ʰ� �״�� ���� ���� �޸� ���� �ȴ�.
}

void GameEngineCamera::ReleaseRenderer(float _deltaTime)
{
	for (std::map<int, std::list<GameEngineRenderer*>>::iterator mapIter = allRenderers_.begin();
		mapIter != allRenderers_.end(); mapIter++)
	{
		for (std::list<GameEngineRenderer*>::iterator listIter = mapIter->second.begin();
			listIter != mapIter->second.end(); /*listIter++*/)
		{
			(*listIter)->ReleaseUpdate(_deltaTime);
			//deadTime_�� 0�� �� �������鿡�� ��� ������ ������.

			if (true == (*listIter)->IsDead())
			{
				//delete (*listIter);	//���̻� ī�޶󿡼� ������ ������ ���� ���� �ʴ´�.
				listIter = mapIter->second.erase(listIter);
				//��� ī�޶��� allRenderers_ ����Ʈ������ �����Ѵ�.
			}
			else
			{
				++listIter;
			}
		}
	}
}

void GameEngineCamera::Update(float _dletaTime)
{
	float4 currentMousePosition = GetMouseWorldPosition();
	//���� ���콺�������� ��ġ�� �˾Ƴ���.

	currentMousePosition.w = 0.f;
	//w�� �� ���� 0??

	mouseDirection_ = currentMousePosition - prevMousePosition_;
	//���� ���콺������ ��ġ - ���� ���콺������ ��ġ = ���콺�������� �̵�����.

	prevMousePosition_ = currentMousePosition;
	//���� ���콺������ ��ġ ����.
}

void GameEngineCamera::OverRenderer(GameEngineCamera* _nextCamera)
{
	if (nullptr == _nextCamera)
	{
		MsgBoxAssert("nextCamera�� �����ϴ�.");
		return;
	}

	std::map<int, std::list<GameEngineRenderer*>>::iterator startGroupIter = allRenderers_.begin();
	std::map<int, std::list<GameEngineRenderer*>>::iterator endGroupIter = allRenderers_.end();

	for (; startGroupIter != endGroupIter; ++startGroupIter)
	{
		std::list<GameEngineRenderer*>& group = startGroupIter->second;

		std::list<GameEngineRenderer*>::iterator groupStart = group.begin();
		std::list<GameEngineRenderer*>::iterator groupEnd = group.end();

		for (; groupStart != groupEnd; )
		{
			GameEngineActor* root = (*groupStart)->GetRoot<GameEngineActor>();

			if (true == root->isLevelOver_)
			{
				_nextCamera->allRenderers_[startGroupIter->first].push_back(*groupStart);
				groupStart = group.erase(groupStart);
			}
			else
			{
				++groupStart;
			}
		}
	}
}
