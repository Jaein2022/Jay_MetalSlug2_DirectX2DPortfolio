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
	bool IsCollision(		//�浹 ���� ���� �Լ�.
		CollisionBodyType _thisType,
		int _groupOrder,
		CollisionBodyType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr);
	void DebugRender();	//�Լ� �̸����� �ٸ���, �����δ� �浹ü�� ���� �׸��� �ʰ� �׸��µ� �ʿ��� ������ �����ϴ� �Լ�.
	//�� �Լ����� ����� ������� GameEngineCoreDebug�� Debug3DRender()�Լ����� ��¥ �������� �Ѵ�.


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

