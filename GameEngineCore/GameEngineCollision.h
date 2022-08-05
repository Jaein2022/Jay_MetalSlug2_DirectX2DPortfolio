#pragma once
#include "GameEngineTransformComponent.h"

class GameEngineCollision : public GameEngineTransformComponent
{
	friend class GameEngineCollisionFunctionInit;

	static bool (*collisionFunctions_[static_cast<int>(CollisionBodyType::CT_Max)][static_cast<int>(CollisionBodyType::CT_Max)])
		(const GameEngineTransform& _transformA, const GameEngineTransform& _transformB);

public:

	GameEngineCollision();
	~GameEngineCollision();

	GameEngineCollision(const GameEngineCollision& _other) = delete;
	GameEngineCollision(GameEngineCollision&& _other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _other) = delete;

public:
	void ChangeOrder(int _order);
	bool IsCollision(		//충돌 여부 판정 함수.
		CollisionBodyType _thisType,
		int _groupOrder,
		CollisionBodyType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr);


public:
	template<typename EnumType>
	void ChangeOrder(EnumType _order)
	{
		ChangeOrder(static_cast<int>(_order));
	}

	template<typename EnumType>
	bool IsCollision(
		CollisionBodyType _thisType,
		EnumType _groupOrder,
		CollisionBodyType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr)
	{
		return IsCollision(_thisType, static_cast<int>(_groupOrder), _otherType, _function);
	}


private:
	void Start() override;

};

