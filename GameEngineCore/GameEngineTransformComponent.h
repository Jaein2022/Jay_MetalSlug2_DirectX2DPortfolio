#pragma once
#include "GameEngineComponent.h"
#include "GameEngineTransformBase.h"
#include "GameEngineActor.h"

class GameEngineActor;
class GameEngineTransformComponent : public GameEngineComponent, public GameEngineTransformBase
{
	//이 클래스의 존재 이유:
public:

	GameEngineTransformComponent();
	~GameEngineTransformComponent();

	GameEngineTransformComponent(const GameEngineTransformComponent& _other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _other) noexcept = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _other) = delete;

public:
	void SetParent(GameEngineUpdateObject* _newParent) override;
	void DetachObject() override;


public:

	GameEngineActor* GetActor()
	{
		return this->GetParent<GameEngineActor>();
	}

	template<typename ParentType>
	ParentType* GetActor()
	{
		return this->GetParent<ParentType>();
	}

};

