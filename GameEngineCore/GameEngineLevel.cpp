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
				//더이상 레벨이 가진 액터들을 그냥 삭제하지 않고, ReleaseHierarchy() 함수를 통해
				// 자식으로 가진 컴포넌트들을 삭제하는 과정을 거친 후 삭제한다.	
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
	this->Update(_deltaTime);		//레벨과 그 자식 클래스들이 가진 요소들 중 액터 제외한 것들을 업데이트 하는 함수.
	this->ActorsUpdate(_deltaTime);	//레벨이 가진 액터들을 업데이트하는 함수.
	this->Render(_deltaTime);
	this->Release(_deltaTime);
}

void GameEngineLevel::ActorsUpdate(float _deltaTime)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& actorList : allActors_)
	{
		float scaleTime = GameEngineTime::GetDeltaTime(actorList.first);

		for (GameEngineActor* const actor : actorList.second)
			//이 위치의 const는 actor가 가리키는 메모리 위치를 변경할 수 없게 하겠다는 의미이다. 
			//하지만 actor가 가리키는 메모리가 가진 값은 얼마든지 변경 가능하다.
		{
			if (true == actor->IsUpdate())
			{
				actor->AllUpdate(scaleTime, _deltaTime);
				//액터들과 그 아래 컴포넌트들의 업데이트는 전부 AllUpdate로 통합.	
			}
		}
	}
}

void GameEngineLevel::Render(float _deltaTime)
{
	GameEngineDevice::RenderStart();
	//RenderStart()와 RenderEnd()사이에 모든 렌더링 과정이 들어가야 한다. 

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
		MsgBoxAssert("루트 액터가 아닌 액터는 여기서 삭제할 수 없습니다.");
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
		//이전 루프에서 사망판정되서 objectsInDeletion_에 저장된 모든 오브젝트들과 그 자식들을 전부 삭제한다.
	}
	deleteObjects.clear();
	//오브젝트들을 전부 삭제했다면 objectsInDeletion_리스트를 비운다.


	for (size_t i = 0; i < cameras_.size(); i++)
	{
		if (nullptr == cameras_[i])
		{
			continue;
		}

		cameras_[i]->ReleaseRenderer(_deltaTime);	//i번째 카메라에서 사망 판정된 렌더러들 등록 해제.
		//렌더러 삭제가 아니라 i번째 카메라의 allRenderers_리스트에서 빼는 등록해제인 것에 유의.
	}

	for (std::map<int, std::list<GameEngineActor*>>::iterator actorGroupIter = allActors_.begin();
		actorGroupIter != allActors_.end(); actorGroupIter++)
	{
		for (std::list<GameEngineActor*>::iterator actorIter = actorGroupIter->second.begin();
			actorIter != actorGroupIter->second.end(); /*listIter++*/)
		{
			(*actorIter)->ReleaseObject(deleteObjects);
			//액터와 그 자식 컴포넌트들이 사망 판정을 받았는지 확인하고, 
			//사망 판정받았다면 부모 오브젝트의 children_리스트에서 떼어내 objectsInDeletion_에 넣는다.

			if (true == (*actorIter)->IsDead())
			{
				//delete (*listIter); 왜 그냥 삭제를 안하고 이렇게 복잡한 삭제/정리 체계를 만들고 있었지??
				actorIter = actorGroupIter->second.erase(actorIter);
				//액터가 사망판정받았다면 allActors_맵에서 제거.
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
		MsgBoxAssert("카메라 오더가 허용 범위를 넘어섰습니다.");
		return;
	}

	cameras_[_cameraOrder]->PushRenderer(_renderer);
}

void GameEngineLevel::PushCamera(GameEngineCamera* _camera, int _cameraOrder)
{
	if (static_cast<int>(CameraOrder::Max) <= _cameraOrder
		|| static_cast<int>(CameraOrder::MainCamera) > _cameraOrder)
	{
		MsgBoxAssert("카메라 오더가 허용 범위를 넘어섰습니다.");
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

