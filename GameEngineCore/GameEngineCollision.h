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
	void DebugRender();	//함수 이름과는 다르게, 실제로는 충돌체를 직접 그리지 않고 그리는데 필요한 정보만 저장하는 함수.
	//이 함수에서 저장된 정보대로 GameEngineCoreDebug의 Debug3DRender()함수에서 진짜 렌더링을 한다.


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

	void SetDebugSetting(CollisionBodyType _debugType, const float4& _color)
	{
		debugType_ = _debugType;
		color_ = _color;
	}

private:
	void Start() override;

private:
	CollisionBodyType debugType_;
	float4 color_;
};

