#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

GameEngineActor::GameEngineActor() : parentLevel_(nullptr), isLevelOver_(false)
{
}

GameEngineActor::~GameEngineActor()
{
	//for (GameEngineComponent* component : allComponents_)
	//{
	//	if (nullptr != component)
	//	{
	//		delete component;
	//		component = nullptr;
	//	}
	//}

	//for (GameEngineTransformComponent* transformComponent : allTransformComponents_)
	//{
	//	if (nullptr != transformComponent)
	//	{
	//		delete transformComponent;
	//		transformComponent = nullptr;
	//	}
	//}
	//�� ���̻� �Ҹ��ڿ��� �������� ����??
	//->���Ͱ� ������Ʈ���� ���̻� ���� ������ �ʰ� GameEngineUpdateObjectŬ������ children_�� ���� ���������� ������ 
	// ������ ������Ʈ ������ ���� ������ �� ����, ���� ����� ������Ʈ�� ���� Death()�Լ��� ȣ���ؾ� ���� ������ ���� 
	// ����� ������ �� �ִ�.
}

void GameEngineActor::DetachObject()
{
	GameEngineUpdateObject::DetachObject();

	this->GetTransform().DetachTransform();
	//�� �̻� ���� �θ�� ������ �´ٴ� ������ ����.
}

void GameEngineActor::SetParent(GameEngineUpdateObject* _newParent)
{
	if (nullptr == this->GetParent())	//������ �θ� ���� == ������ ������ �θ��.	
	{
		this->GetLevel()->RemoveActor(this);	//������ allActors_�� ��ϵ� ���͸� �����Ѵ�.
	}

	this->GameEngineUpdateObject::SetParent(_newParent);	//���Ϳ��� ���ο� �θ� �������ش�.

	GameEngineTransformBase* newParent = dynamic_cast<GameEngineTransformBase*>(_newParent);
	if (nullptr == newParent)
	{
		MsgBoxAssert("Ʈ�������� �ִ� ������Ʈ���� ������ �θ� �� �� �ֽ��ϴ�.");
	}
	else
	{
		this->GetTransform().SetParentTransform(newParent->GetTransform());	//Ʈ�������� �θ��ڽ� ���踦 �δ´�.
	}
}

void GameEngineActor::Start()
{
}

void GameEngineActor::Update(float _deltaTime)
{
}

void GameEngineActor::End()
{
}

void GameEngineActor::AllUpdate(float _scaleTime, float _deltaTime)
{
	this->AddAccTime(_deltaTime);
	this->ReleaseUpdate(_deltaTime);
	this->Update(_scaleTime);

	for (GameEngineUpdateObject* object : children_)
	{
		object->AddAccTime(_deltaTime);
		object->ReleaseUpdate(_deltaTime); //deadTime_�� 0�� �� ������Ʈ�鿡�� ��� ������ ������.
		if (true == object->IsUpdate())
		{
			object->Update(_scaleTime);
		}
	}
}
