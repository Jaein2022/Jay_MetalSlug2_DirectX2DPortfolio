#pragma once

class StateInfo
{
public:
	float stateTime_;	//이 스테이트가 지금까지 얼마나 오래 지속되었나.
	std::string prevState_;	//이전 스테이트는 뭐였나.

	StateInfo(): stateTime_(0.f), prevState_("")
	{
	}
};

class GameEngineStateManager;
class GameEngineState : public GameEngineNameObject
{
	friend GameEngineStateManager;

public:
	void StateUpdate(float _deltaTime)
	{
		if (nullptr == update_)
		{
			return;
		}

		update_(_deltaTime, info_);
	}

public:
	GameEngineState() : start_(nullptr), update_(nullptr), end_(nullptr)
	{
	}

private:
	StateInfo info_;
	std::function<void(const StateInfo& _info)> start_;
	std::function<void(float _deltaTime, const StateInfo& _info)> update_;
	std::function<void(const StateInfo& _info)> end_;
};

class GameEngineStateManager final	//final: <-이 밑으로 추가 상속 불가.
{
	//이 클래스의 존재 이유:
public:
	GameEngineStateManager();
	~GameEngineStateManager();

protected:
	GameEngineStateManager(const GameEngineStateManager& _other) = delete;
	GameEngineStateManager(GameEngineStateManager&& _other) noexcept = delete;

private:
	GameEngineStateManager& operator=(const GameEngineStateManager& _other) = delete;
	GameEngineStateManager& operator=(const GameEngineStateManager&& _other) = delete;


public:
	void Update(float _deltaTime);

public:
	template<typename ObjectType>
	void CreateState_MemberFunctions(
		const std::string& _stateName,
		ObjectType* _object,
		void(ObjectType::* _update)(float, const StateInfo&),
		void(ObjectType::* _start)(const StateInfo&) = nullptr,
		void(ObjectType::* _end)(const StateInfo&) = nullptr
	)
	{
		if (allStates_.end() != allStates_.find(_stateName))
		{
			MsgBoxAssertString(_stateName + ": 이미 같은 이름의 스테이트가 존재합니다.");
			return;
		}

		GameEngineState& newState = allStates_[_stateName];
		newState.SetName(_stateName);

		if (nullptr != _update)
		{
			newState.update_ = std::bind(_update, _object, std::placeholders::_1, std::placeholders::_2);
		}

		if (nullptr != _start)
		{
			newState.start_ = std::bind(_start, _object, std::placeholders::_1);
		}

		if (nullptr != _end)
		{
			newState.end_ = std::bind(_end, _object, std::placeholders::_1);
		}
	}


	void ChangeState(const std::string& _stateName)
	{
		if (allStates_.end() == allStates_.find(_stateName))
		{
			MsgBoxAssertString(_stateName + ": 그런 이름의 스테이트가 없습니다.");
			return;
		}

		std::string prevState = "";

		if (nullptr != currentState_)
		{
			prevState = currentState_->GetNameCopy();

			if (nullptr != currentState_->end_)
			{
				currentState_->end_(currentState_->info_);
			}
		}

		currentState_ = &allStates_[_stateName];
		currentState_->info_.stateTime_ = 0.f;
		currentState_->info_.prevState_ = prevState;

		if (nullptr != currentState_->start_)
		{
			currentState_->start_(currentState_->info_);
		}
	}
private:
	//
	std::map<std::string, GameEngineState> allStates_;
	GameEngineState* currentState_;

};

