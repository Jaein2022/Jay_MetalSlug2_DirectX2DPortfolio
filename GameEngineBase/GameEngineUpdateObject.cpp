#include "PreCompile.h"
#include "GameEngineUpdateObject.h"
#include "GameEngineTime.h"

GameEngineUpdateObject::GameEngineUpdateObject()
	: order_(0),
	isReleaseUpdate_(false),
	deadTime_(0.f),
	accTime_(0.f),
	isUpdate_(true),
	isDead_(false),
	parent_(nullptr)
{
}

GameEngineUpdateObject::~GameEngineUpdateObject()
{
}

void GameEngineUpdateObject::SetParent(GameEngineUpdateObject* _newParent)
{
	DetachObject();

	parent_ = _newParent;
	parent_->children_.push_back(this);
}

void GameEngineUpdateObject::ReleaseHierarchy()
{
	for (std::list<GameEngineUpdateObject*>::iterator iter = children_.begin();
		iter != children_.end(); iter++)
	{
		if (nullptr != (*iter))
		{
			(*iter)->ReleaseHierarchy();
			//���̻��� �ڽ��� ���������� ��� ������� ReleaseHierarchy()�Լ��� ��� ȣ���Ѵ�.
		}
	}

	//�� �̻��� �ڽ��� ���ٸ� �Լ� ȣ���� ���߰� ������� �����´�.
	delete this;	//�� �����ӿ�ũ���� ��¥�� ������Ʈ�� �޸������� �ϴ� �κ�.
}

void GameEngineUpdateObject::AllUpdate(float _deltaTime)
{
	this->AddAccTime(_deltaTime);
	this->ReleaseUpdate(_deltaTime);
	this->Update(GameEngineTime::GetInst()->GetTimeScale(this->GetOrder()) * _deltaTime);

	for (GameEngineUpdateObject* object : children_)
	{
		object->AddAccTime(_deltaTime);
		object->ReleaseUpdate(_deltaTime); //deadTime_�� 0�� �� ������Ʈ�鿡�� ��� ������ ������.
		if (true == object->IsUpdate())
		{
			object->AllUpdate(_deltaTime);
		}
	}
}

void GameEngineUpdateObject::AllOnEvent()
{
	this->OnEvent();

	for (GameEngineUpdateObject* object : children_)
	{
		if (true == object->IsUpdate())
		{
			object->AllOnEvent();
		}
	}
}

void GameEngineUpdateObject::AllOffEvent()
{
	this->OffEvent();

	for (GameEngineUpdateObject* object : children_)
	{
		if (true == object->IsUpdate())
		{
			object->AllOffEvent();
		}
	}
}

void GameEngineUpdateObject::ReleaseObject(std::list<GameEngineUpdateObject*>& _releaseList)
{
	//�ڱ� �ڽ��� (true == IsDead())���� == ��������� �޾Ҵ��� Ȯ���Ѵ�.
	if (true == this->IsDead())
		//�ڱ� �ڽŸ� Ȯ���ϴ� isDead_ ������ �ƴ϶� �θ���� Ȯ���ϴ� IsDead()�Լ��� Ȯ���ؾ� �Ѵ�.
	{
		DetachObject();		//����������� ��� ������Ʈ���� �θ� ������Ʈ�� chlidren����Ʈ���� �����Ѵ�.
		_releaseList.push_back(this);
		//����������� ��� ������Ʈ���� �ش� ������ _releaseObjects�� �ִ´�.

		return;	//�Ʒ��� �������� ���� �ٷ� �Լ� ����.
	}

	//this�� isDead�� false�� ������� ���� �Դٸ� �ڽ� ������Ʈ�� �� ����������� ������Ʈ�� �ִ��� Ž���Ѵ�.
	for (std::list<GameEngineUpdateObject*>::iterator iter = children_.begin();
		iter != children_.end(); /*iter++*/)
	{
		if (true == (*iter)->IsDead())
		{
			//�ڽ� ������Ʈ�� �� ����������� ������Ʈ�� �ִٸ�,
			_releaseList.push_back((*iter));	//_releaseObjects�� �־� ���� ������� ����Ѵ�.

			GameEngineUpdateObject* tempObject = (*iter);
			++iter;	//chlidren����Ʈ���� ������Ʈ�� �����ϱ� ���� ���ͷ����͸� ���� �̵���Ű�� ������,
			//���ͷ����Ͱ� ���� ���� �̵��� �� ����.

			tempObject->DetachObject();	//�θ� ������Ʈ�� chlidren����Ʈ���� �����Ѵ�.
		}
		else
		{
			//�ڽ� ������Ʈ�� �� ����������� ������Ʈ�� ���ٸ� �ڽ��� �ڽĵ���� ��� ������� ���� Ž���Ѵ�.
			(*iter)->ReleaseObject(_releaseList);
			++iter;
		}
	}

}

void GameEngineUpdateObject::DetachObject()
{
	if (nullptr != parent_)
	{
		parent_->children_.remove(this);
	}
}