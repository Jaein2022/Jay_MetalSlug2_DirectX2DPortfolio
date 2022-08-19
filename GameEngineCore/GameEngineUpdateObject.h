#pragma once

class GameEngineUpdateObject : public GameEngineDebugObject
{
	//�� Ŭ������ ���� ������ ����?? �ݵ�� �����ص� ��.


public:

	GameEngineUpdateObject();
	virtual ~GameEngineUpdateObject();

	GameEngineUpdateObject(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject(GameEngineUpdateObject&& _other) noexcept = delete;
	GameEngineUpdateObject& operator=(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject& operator=(GameEngineUpdateObject&& _other) = delete;


public:
	virtual void SetParent(GameEngineUpdateObject* _newParent);
	virtual void ReleaseHierarchy(); //�� ������Ʈ�� �� �ڽĵ��� ���� �����ϴ� �Լ�.

	// �� ������Ʈ�� �����ӱ����ȿ��� ���� ��ġ�� ����.
	virtual void Update(float _deltaTime) = 0;

	void AllUpdate(float _deltaTime);

	virtual void LevelStartEvent() {};
	virtual void LevelEndEvent() {};

public:
	inline void On()
	{
		this->isUpdate_ = true;
	}

	inline void Off()
	{
		this->isUpdate_ = false;
	}

	inline void OnOffSwitch()
	{
		this->isUpdate_ = !this->isUpdate_;
	}

	inline bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return this->isUpdate_ && false == this->isDead_;
		}
		else
		{
			return this->isUpdate_ && false == this->isDead_ && true == parent_->IsUpdate();
			//�ڱ� �ڽ��� isUpdate_ == true���� �θ� ������Ʈ�� isUpdate_ == false�� ������Ʈ���� ���ܵȴ�. 
		}
	}

	inline bool& IsUpdateRef()
	{
		return isUpdate_;
	}

	inline bool IsDead()
	{
		if (nullptr == parent_)
		{
			return this->isDead_;
		}
		else
		{
			return this->isDead_ || true == parent_->IsDead();
			//�ڱ� �ڽ��� isDead_ == false���� �θ� ������Ʈ�� isUpdate_ == true�� ��� �����ް� �����ȴ�.
		}
	}

	//Ư�� ������Ʈ�� ������ �������κ��� �󸶳� �ð��� �������� �����ϴ� �Լ�.
	void AddAccTime(float _deltaTime)
	{
		this->accTime_ += _deltaTime;
	}

	//������ �������κ��� �󸶸�ŭ�� �ð��� �������� �޾ƺ��� �Լ�.
	float GetAccTime()
	{
		return this->accTime_;
	}

	//Ư�� ������Ʈ�� ������ �ð� �ʱ�ȭ.
	void ResetAccTime()
	{
		this->accTime_ = 0.0f;
	}

	//Ư�� ������Ʈ�� ��� ������� ������ �Լ�.
	inline void Death()
	{
		this->isDead_ = true;
	}

	//�Է��� �ð��� ���� �� Ư�� ������Ʈ�� ��� �����ϴ� �Լ�.
	inline void Death(float _time)
	{
		this->isReleaseUpdate_ = true;
		this->deadTime_ = _time;
	}

	void ReleaseUpdate(float _deltaTime)
	{
		if (false == isReleaseUpdate_)
		{
			return;
		}

		this->deadTime_ -= _deltaTime;

		if (0.0f >= deadTime_)
		{
			this->isDead_ = true;
		}
	}

	inline int GetOrder()
	{
		return this->order_;
	}

	virtual inline void SetOrder(int _order)
	{
		this->order_ = _order;
	}

	//����ȯ �θ� �ޱ�.
	template<typename ParentType>
	ParentType* GetParent()
	{
		return dynamic_cast<ParentType*>(parent_);
	}

	//GameEngineUpdateObject* ���·� �θ� �ޱ�.
	GameEngineUpdateObject* GetParent()
	{
		return parent_;
	}

	template<typename ParentType>
	ParentType* GetRoot()
	{
		return dynamic_cast<ParentType*>(GetRoot());
	}

	GameEngineUpdateObject* GetRoot()
	{
		GameEngineUpdateObject* currentObject = this;

		while (nullptr != currentObject->GetParent())
		{
			currentObject = currentObject->GetParent();
		}
		return currentObject;
	}



protected:
	//�� ������Ʈ�� ������ �ϱ� �����Ҷ� ȣ���ϴ� �Լ�.
	virtual void OnEvent() {}

	//�� ������Ʈ�� ������ ������ �� ȣ���ϴ� �Լ�.
	virtual void OffEvent() {}

	//�� ������Ʈ�� �����ɶ� ��ġ�� �������� �����ϴ� �Լ�.
	virtual void Start() = 0;

	// �� ������Ʈ�� �޸𸮰� ������ �� ��ġ�� �������� �����ϴ� �Լ�.
	virtual void End() = 0;

	void AllLevelStartEvent();
	void AllLevelEndEvent();

	void AllOnEvent();
	void AllOffEvent();

	void ReleaseObject(std::list<GameEngineUpdateObject*>& _releaseList);
	//����������� ������Ʈ�� �� �ڽĵ��� ������Ʈ �������� ��� ���� ��� ����Ʈ�� ����ϴ� �Լ�.
	//���⿡ ���� ������Ʈ���� ���� �������� �����ȴ�. �ٷ� �������� �ʴ°Ϳ� ����.

	virtual void DetachObject();
	//�� ������Ʈ�� �θ� ������Ʈ�� children_����Ʈ���� �����ϴ� �Լ�.

protected:
	template<typename ConvertType>
	std::list<ConvertType*> GetConvertChildren()
	{
		std::list<ConvertType*> newList;

		for (GameEngineUpdateObject* child : children_)
		{
			ConvertType* convertPtr = dynamic_cast<ConvertType*>(child);
			if (nullptr != convertPtr)
			{
				newList.push_back(convertPtr);
			}
		}

		return newList;
	}

protected:
	std::list<GameEngineUpdateObject*> children_;	//�ڽ� ������Ʈ��.

private:
	int order_;			//������Ʈ ����.
	bool isReleaseUpdate_;	//true: Ư�� ������Ʈ�� ���� �ð��� ���.
	float deadTime_;	//���� ������Ʈ�� ��� ���� �ð�. �� �ð��� 0�̳� ������ �Ǹ� ���.
	float accTime_;		//������Ʈ ���� �������κ��� ���� �ð�.

	bool isUpdate_;	//true: ������Ʈ ����. false: ������Ʈ ����.
	bool isDead_;	//true: ��� ���� ����. false: ��� ���� �� ����. ���ó���� �ƴ� ��� �����ӿ� ����.

	GameEngineUpdateObject* parent_;	//�θ� ������Ʈ.


};

