#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineCamera.h"
#include "GameEngineCameraActor.h"
#include "GameEngineDevice.h"
#include "GameEngineGUI.h"

GameEngineLevel::GameEngineLevel()
{
	cameras_.resize(static_cast<UINT>(CameraOrder::Max));

	GameEngineCameraActor* mainCameraActor = CreateActor<GameEngineCameraActor>();
	mainCameraActor->GetTransform().SetLocalPosition(0, 0, -100);
	mainCameraActor->GetCameraComponent()->SetProjectionMode(CameraProjectionMode::Orthographic);
	mainCameraActor->GetCameraComponent()->SetCameraOrder(CameraOrder::MainCamera);


	GameEngineCameraActor* UICameraActor = CreateActor<GameEngineCameraActor>();
	UICameraActor->GetTransform().SetLocalPosition(0, 0, -100);
	UICameraActor->GetCameraComponent()->SetProjectionMode(CameraProjectionMode::Orthographic);
	UICameraActor->GetCameraComponent()->SetCameraOrder(CameraOrder::UICamera);
}

GameEngineLevel::~GameEngineLevel()
{
	for (const std::pair<int, std::list<GameEngineActor*>>& actorGroup : allActors_)
	{
		for (GameEngineActor* actor : actorGroup.second)
		{
			if (nullptr != actor)
			{
				//delete actor;
				//actor = nullptr;
				actor->ReleaseHierarchy();
				//���̻� ������ ���� ���͵��� �׳� �������� �ʰ�, ReleaseHierarchy() �Լ��� ����
				// �ڽ����� ���� ������Ʈ���� �����ϴ� ������ ��ģ �� �����Ѵ�.	
			}
		}
	}
}

GameEngineCameraActor* GameEngineLevel::GetMainCameraActor()
{
	return cameras_[static_cast<int>(CameraOrder::MainCamera)]->GetActor<GameEngineCameraActor>();
}

GameEngineTransform& GameEngineLevel::GetMainCameraActorTransform()
{
	return cameras_[static_cast<int>(CameraOrder::MainCamera)]->GetActor()->GetTransform();
}

GameEngineCameraActor* GameEngineLevel::GetUICameraActor()
{
	return cameras_[static_cast<int>(CameraOrder::UICamera)]->GetActor<GameEngineCameraActor>();
}

GameEngineTransform& GameEngineLevel::GetUICameraActorTransform()
{
	return cameras_[static_cast<int>(CameraOrder::UICamera)]->GetActor()->GetTransform();
}

void GameEngineLevel::LevelUpdate(float _deltaTime)
{
	this->AddAccTime(_deltaTime);
	this->Update(_deltaTime);		//������ �� �ڽ� Ŭ�������� ���� ��ҵ� �� ���� ������ �͵��� ������Ʈ �ϴ� �Լ�.
	this->ActorsUpdate(_deltaTime);	//������ ���� ���͵��� ������Ʈ�ϴ� �Լ�.
	this->Render(_deltaTime);
	this->Release(_deltaTime);
}

void GameEngineLevel::ActorsUpdate(float _deltaTime)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& actorList : allActors_)
	{
		float scaleTime = GameEngineTime::GetDeltaTime(actorList.first);

		for (GameEngineActor* const actor : actorList.second)
			//�� ��ġ�� const�� actor�� ����Ű�� �޸� ��ġ�� ������ �� ���� �ϰڴٴ� �ǹ��̴�. 
			//������ actor�� ����Ű�� �޸𸮰� ���� ���� �󸶵��� ���� �����ϴ�.
		{
			if (true == actor->IsUpdate())
			{
				actor->AllUpdate(scaleTime, _deltaTime);
				//���͵�� �� �Ʒ� ������Ʈ���� ������Ʈ�� ���� AllUpdate�� ����.	
			}
		}
	}
}

void GameEngineLevel::Render(float _deltaTime)
{
	GameEngineDevice::RenderStart();
	//RenderStart()�� RenderEnd()���̿� ��� ������ ������ ���� �Ѵ�. 

	for (size_t i = 0; i < cameras_.size(); i++)
	{
		if (nullptr == cameras_[i])
		{
			continue;
		}

		cameras_[i]->Render(_deltaTime);
	}

	GameEngineGUI::Render(this, _deltaTime);


	GameEngineDevice::RenderEnd();
}

void GameEngineLevel::RemoveActor(GameEngineActor* _rootActor)
{
	if (allActors_.end() == allActors_.find(_rootActor->GetOrder()))
	{
		MsgBoxAssert("��Ʈ ���Ͱ� �ƴ� ���ʹ� ���⼭ ������ �� �����ϴ�.");
	}
	else
	{
		allActors_[_rootActor->GetOrder()].remove(_rootActor);
	}
}

void GameEngineLevel::Release(float _deltaTime)
{
	for (GameEngineUpdateObject* object : deleteObjects)
	{
		object->ReleaseHierarchy();
		//���� �������� ��������Ǽ� objectsInDeletion_�� ����� ��� ������Ʈ��� �� �ڽĵ��� ���� �����Ѵ�.
	}
	deleteObjects.clear();
	//������Ʈ���� ���� �����ߴٸ� objectsInDeletion_����Ʈ�� ����.


	for (size_t i = 0; i < cameras_.size(); i++)
	{
		if (nullptr == cameras_[i])
		{
			continue;
		}

		cameras_[i]->ReleaseRenderer(_deltaTime);	//i��° ī�޶󿡼� ��� ������ �������� ��� ����.
		//������ ������ �ƴ϶� i��° ī�޶��� allRenderers_����Ʈ���� ���� ��������� �Ϳ� ����.
	}

	for (std::map<int, std::list<GameEngineActor*>>::iterator actorGroupIter = allActors_.begin();
		actorGroupIter != allActors_.end(); actorGroupIter++)
	{
		for (std::list<GameEngineActor*>::iterator actorIter = actorGroupIter->second.begin();
			actorIter != actorGroupIter->second.end(); /*listIter++*/)
		{
			(*actorIter)->ReleaseObject(deleteObjects);
			//���Ϳ� �� �ڽ� ������Ʈ���� ��� ������ �޾Ҵ��� Ȯ���ϰ�, 
			//��� �����޾Ҵٸ� �θ� ������Ʈ�� children_����Ʈ���� ��� objectsInDeletion_�� �ִ´�.

			if (true == (*actorIter)->IsDead())
			{
				//delete (*listIter); �� �׳� ������ ���ϰ� �̷��� ������ ����/���� ü�踦 ����� �־���??
				actorIter = actorGroupIter->second.erase(actorIter);
				//���Ͱ� ��������޾Ҵٸ� allActors_�ʿ��� ����.
			}
			else
			{
				++actorIter;
			}
		}
	}
}


void GameEngineLevel::PushRenderer(GameEngineRenderer* _renderer, int _cameraOrder)
{
	if (static_cast<int>(CameraOrder::Max) <= _cameraOrder
		|| static_cast<int>(CameraOrder::MainCamera) > _cameraOrder)
	{
		MsgBoxAssert("ī�޶� ������ ��� ������ �Ѿ���ϴ�.");
		return;
	}

	cameras_[_cameraOrder]->PushRenderer(_renderer);
}

void GameEngineLevel::PushCamera(GameEngineCamera* _camera, int _cameraOrder)
{
	if (static_cast<int>(CameraOrder::Max) <= _cameraOrder
		|| static_cast<int>(CameraOrder::MainCamera) > _cameraOrder)
	{
		MsgBoxAssert("ī�޶� ������ ��� ������ �Ѿ���ϴ�.");
		return;
	}

	cameras_[_cameraOrder] = _camera;
}

void GameEngineLevel::OverChildMove(GameEngineLevel* _nextLevel)
{
	std::map<int, std::list<GameEngineActor*>>::iterator startGroupIter = allActors_.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator endGroupIter = allActors_.end();

	std::list<GameEngineActor*> overList;

	for (; startGroupIter != endGroupIter; ++startGroupIter)
	{
		std::list<GameEngineActor*>& group = startGroupIter->second;

		std::list<GameEngineActor*>::iterator groupStart = group.begin();
		std::list<GameEngineActor*>::iterator groupEnd = group.end();
		for (; groupStart != groupEnd; )
		{
			if (true == (*groupStart)->isLevelOver_)
			{
				overList.push_back((*groupStart));
				groupStart = group.erase(groupStart);
			}
			else
			{
				++groupStart;
			}

		}
	}

	for (GameEngineActor* OverActor : overList)
	{
		_nextLevel->allActors_[OverActor->GetOrder()].push_back(OverActor);
	}

	for (size_t i = 0; i < cameras_.size(); i++)
	{
		if (nullptr == cameras_[i])
		{
			continue;
		}

		cameras_[i]->OverRenderer(_nextLevel->cameras_[i]);
	}

}

