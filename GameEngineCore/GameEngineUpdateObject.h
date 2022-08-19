#pragma once

class GameEngineUpdateObject : public GameEngineDebugObject
{
	//이 클래스의 존재 이유는 무엇?? 반드시 정리해둘 것.


public:

	GameEngineUpdateObject();
	virtual ~GameEngineUpdateObject();

	GameEngineUpdateObject(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject(GameEngineUpdateObject&& _other) noexcept = delete;
	GameEngineUpdateObject& operator=(const GameEngineUpdateObject& _other) = delete;
	GameEngineUpdateObject& operator=(GameEngineUpdateObject&& _other) = delete;


public:
	virtual void SetParent(GameEngineUpdateObject* _newParent);
	virtual void ReleaseHierarchy(); //이 오브젝트와 그 자식들을 최종 삭제하는 함수.

	// 이 오브젝트가 프레임구조안에서 돌때 거치는 절차.
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
			//자기 자신이 isUpdate_ == true여도 부모 오브젝트가 isUpdate_ == false면 업데이트에서 제외된다. 
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
			//자기 자신이 isDead_ == false여도 부모 오브젝트가 isUpdate_ == true면 사망 판정받고 삭제된다.
		}
	}

	//특정 오브젝트가 생성된 시점으로부터 얼마나 시간이 지났는지 측정하는 함수.
	void AddAccTime(float _deltaTime)
	{
		this->accTime_ += _deltaTime;
	}

	//생성된 시점으로부터 얼마만큼의 시간이 지났는지 받아보는 함수.
	float GetAccTime()
	{
		return this->accTime_;
	}

	//특정 오브젝트가 생성된 시간 초기화.
	void ResetAccTime()
	{
		this->accTime_ = 0.0f;
	}

	//특정 오브젝트를 즉시 사망판정 내리는 함수.
	inline void Death()
	{
		this->isDead_ = true;
	}

	//입력한 시간이 지난 후 특정 오브젝트를 사망 판정하는 함수.
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

	//형변환 부모 받기.
	template<typename ParentType>
	ParentType* GetParent()
	{
		return dynamic_cast<ParentType*>(parent_);
	}

	//GameEngineUpdateObject* 형태로 부모 받기.
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
	//이 오브젝트가 동작을 하기 시작할때 호출하는 함수.
	virtual void OnEvent() {}

	//이 오브젝트가 동작을 정지할 때 호출하는 함수.
	virtual void OffEvent() {}

	//이 오브젝트가 생성될때 거치는 절차들을 실행하는 함수.
	virtual void Start() = 0;

	// 이 오브젝트가 메모리가 삭제될 때 거치는 절차들을 실행하는 함수.
	virtual void End() = 0;

	void AllLevelStartEvent();
	void AllLevelEndEvent();

	void AllOnEvent();
	void AllOffEvent();

	void ReleaseObject(std::list<GameEngineUpdateObject*>& _releaseList);
	//사망판정받은 오브젝트와 그 자식들을 업데이트 루프에서 떼어내 삭제 대상 리스트에 등록하는 함수.
	//여기에 넣은 오브젝트들은 다음 루프에서 삭제된다. 바로 삭제되지 않는것에 유의.

	virtual void DetachObject();
	//이 오브젝트를 부모 오브젝트의 children_리스트에서 제거하는 함수.

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
	std::list<GameEngineUpdateObject*> children_;	//자식 오브젝트들.

private:
	int order_;			//업데이트 순서.
	bool isReleaseUpdate_;	//true: 특정 오브젝트가 일정 시간후 사망.
	float deadTime_;	//지정 오브젝트의 사망 예정 시간. 이 시간이 0이나 음수가 되면 사망.
	float accTime_;		//오브젝트 생성 시전으로부터 지난 시간.

	bool isUpdate_;	//true: 업데이트 참여. false: 업데이트 제외.
	bool isDead_;	//true: 사망 판정 받음. false: 사망 판정 안 받음. 사망처리가 아닌 사망 판정임에 주의.

	GameEngineUpdateObject* parent_;	//부모 오브젝트.


};

