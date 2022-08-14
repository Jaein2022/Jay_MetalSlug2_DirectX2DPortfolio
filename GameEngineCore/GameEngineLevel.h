#pragma once

enum class CameraOrder
{
	MainCamera,
	User0,
	User1,
	User2,
	User3,
	User4,
	User5,
	User6,
	User7,
	UICamera,
};

class GameEngineCamera;
class GameEngineCameraActor;
class GameEngineRenderer;
class GameEngineActor;
class GameEngineTransform;
class GameEngineCollision;
class GameEngineLevel : public GameEngineNameObject, public GameEngineUpdateObject
{
	//����: ���͵��� �����̴� ���.

	friend class GameEngineCore;
	friend GameEngineRenderer;
	friend GameEngineCamera;
	friend GameEngineActor;
	friend GameEngineCollision;

protected:
	GameEngineLevel();	//������ ���� �����ڿ��� ����ī�޶�� UIī�޶� �����ϰ� �����Ѵ�.
	virtual ~GameEngineLevel() = 0;

	GameEngineLevel(const GameEngineLevel& _other) = delete;
	GameEngineLevel(GameEngineLevel&& _other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _other) = delete;

public:
	GameEngineCameraActor* GetMainCameraActor();
	GameEngineTransform& GetMainCameraActorTransform();
	GameEngineCameraActor* GetUICameraActor();
	GameEngineTransform& GetUICameraActorTransform();

	void AllClear();


public:
	//����ī�޶� ������Ʈ�� �������� �Լ�.
	GameEngineCamera* GetMainCamera()
	{
		return cameras_[static_cast<int>(CameraOrder::MainCamera)];
	}

	GameEngineCamera* GetUICamera()
	{
		return cameras_[static_cast<int>(CameraOrder::UICamera)];
	}

	//���͵� �� Ư�� ������ �������� �Լ�.
	template<typename GroupIndexType>
	std::list<GameEngineActor*> GetGroup(GroupIndexType _objectGroupIndex)
	{
		return allActors_[static_cast<int>(_objectGroupIndex)];
	}
	//���͵� �� Ư�� ������ �������� �Լ�.
	std::list<GameEngineActor*> GetGroup(int _objectGroupIndex)
	{
		return allActors_[_objectGroupIndex];
	}

	//���͵� �� Ư�� ������ ���ϴ� ���·� ����ȯ�ؼ� �������� �Լ�.
	template<typename ObjectType>
	std::list<ObjectType*> GetConvertedGroup(int _objectGroupIndex)
	{
		std::list<ObjectType*> result;
		for (GameEngineActor* actor : allActors_[_objectGroupIndex])
		{
			result.push_back(dynamic_cast<ObjectType*>(actor));
		}
		return result;
	}
	//���͵� �� Ư�� ������ ���ϴ� ���·� ����ȯ�ؼ� �������� �Լ�.
	template<typename ObjectType, typename GroupIndexType>
	std::list<ObjectType*> GetConvertedGroup(GroupIndexType _objectGroupIndex)
	{
		return GetConvertedGroup<ObjectType>(static_cast<int>(_objectGroupIndex));
	}

protected:
	//���� ���� �Լ�.
	template<typename ActorType, typename GroupIndexType>
	ActorType* CreateActor(GroupIndexType _type, const std::string& _actorName = "")
	{
		return CreateActor<ActorType>(static_cast<int>(_type), _actorName);
	}

	//���� ���� �Լ�.
	template<typename ActorType>
	ActorType* CreateActor(int _objectGroupIndex = 0, const std::string& _actorName = "")
	{
		GameEngineActor* newActor = new ActorType();

		newActor->SetLevel(this);
		newActor->SetOrder(_objectGroupIndex);
		newActor->SetName(_actorName);
		newActor->Start();

		std::list<GameEngineActor*>& actorGroup = allActors_[_objectGroupIndex];
		//allActors_ �ȿ� _objectGroupIndex�� Ű������ ���� �� �ִٸ� �װ� ã�Ƽ� ��ȯ�ϰ�,
		// ���ٸ� ���� ��ȯ�Ѵ�. 
		//�� allActors_.find(_objectGroupIndex) + 
		// allActors_.insert(std::make_pair(_objectGroupIndex, std::list<GameEngineActor*>(newActor));
		// ������ �ڵ��̴�.

		actorGroup.push_back(newActor);

		return dynamic_cast<ActorType*>(newActor);
	}


private:
	void LevelUpdate(float _deltaTime);
	void ActorsUpdate(float _deltaTime);
	void Render(float _deltaTime);		//����ī�޶� ���� ���������� ������� �����ϴ� �Լ�.
	void RemoveActor(GameEngineActor* _actor);	//allActors_�ʿ��� ���͸� �����ϴ� �Լ�.
	void Release(float _deltaTime);	//�� �����ӿ�ũ�� ���� ������Ʈ ���� ����.

private:
	void PushRenderer(GameEngineRenderer* _renderer, int _cameraOrder);	//����ī�޶� �������� ����ϴ� �Լ�. 
	void PushCamera(GameEngineCamera* _camera, int _cameraOrder);	//�ش� ������ ����ī�޶� ����ϴ� �Լ�.
	void PushCollision(GameEngineCollision* _collision, int _collisionOrder);
	void OverChildMove(GameEngineLevel* _nextLevel);	//������Ʈ�� ���� ������ ������Ű�� �Լ�.
	void ActorOnEvent();
	void ActorOffEvent();

private:
	void PushCamera(GameEngineCamera* _camera, CameraOrder _order)
	{
		PushCamera(_camera, static_cast<int>(_order));
	}

	void PushRenderer(GameEngineRenderer* _renderer, CameraOrder _order)
	{
		PushRenderer(_renderer, static_cast<int>(_order));
	}

	void PushRendererToMainCamera(GameEngineRenderer* _renderer)
	{
		PushRenderer(_renderer, static_cast<int>(CameraOrder::MainCamera));
	}

	void PushRendererToUICamera(GameEngineRenderer* _renderer)
	{
		PushRenderer(_renderer, static_cast<int>(CameraOrder::UICamera));
	}

	void PushActor(GameEngineActor* _actor, int _objectGroupIndex)
	{
		std::list<GameEngineActor*>& actorGroup = allActors_[_objectGroupIndex];
		actorGroup.push_back(_actor);
	}

private:
	std::map<int, std::list<GameEngineActor*>> allActors_;
	//�� ������ ��� ���͵��� ����� ��.

	std::list<GameEngineUpdateObject*> deleteObjects;
	//���� ������ ��� ������Ʈ���� ���� �ҹ��ϰ� ��� ����Ǵ� ������ ������.


	std::vector<GameEngineCamera*> cameras_;	//�� ������ ����ϴ� ��� ī�޶��.

	std::map<int, std::list<GameEngineCollision*>> allCollisions_;	//�� ������ ��� �浹ü��.	
};


