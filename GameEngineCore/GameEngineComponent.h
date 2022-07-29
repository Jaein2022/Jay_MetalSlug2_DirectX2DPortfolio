#pragma once

class GameEngineComponent : public GameEngineNameObject, public GameEngineUpdateObject
{
	//이 클래스의 존재 이유:

	friend class GameEngineActor;
	//


protected:

	GameEngineComponent();
	~GameEngineComponent();

	GameEngineComponent(const GameEngineComponent& _other) = delete;
	GameEngineComponent(GameEngineComponent&& _other) noexcept = delete;
	GameEngineComponent& operator=(const GameEngineComponent& _other) = delete;
	GameEngineComponent& operator=(GameEngineComponent&& _other) = delete;


public:

public:
	void SetParent(GameEngineUpdateObject* _newParent) override;

protected:
	virtual void Start();
	virtual void Update(float _deltaTime);
	virtual void End();


private:

};

