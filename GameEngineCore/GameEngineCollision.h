#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"

class GameEngineCollision : public GameEngineTransformComponent
{
	friend class GameEngineCollisionFunctionInit;

	static bool (*collisionFunctions_[static_cast<int>(CollisionType::CT_Max)][static_cast<int>(CollisionType::CT_Max)])
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
		CollisionType _thisType,
		int _collisionOrder,
		CollisionType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr);
	void DebugRender();	//�Լ� �̸����� �ٸ���, �����δ� �浹ü�� ���� �׸��� �ʰ� �׸��µ� �ʿ��� ������ �����ϴ� �Լ�.
	//�� �Լ����� ����� ������� GameEngineCoreDebug�� Debug3DRender()�Լ����� ��¥ �������� �Ѵ�.

	void SetUIDebugCamera();	//�� �ݸ����� �������� UIī�޶󿡼� �ϰ� �ϴ� �Լ�.
	//UIī�޶�� ���� �⺻���� ī�޶��̹Ƿ� ���� ���ؿ��� �̷� �����Լ��� ������ �� �ִ�.

public:
	template<typename EnumType>
	void ChangeOrder(EnumType _collisionOrder)
	{
		ChangeOrder(static_cast<int>(_collisionOrder));
	}

	template<typename EnumType>
	bool IsCollision(
		CollisionType _thisType,
		EnumType _collisionOrder,
		CollisionType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr)
	{
		return IsCollision(_thisType, static_cast<int>(_collisionOrder), _otherType, _function);
	}

	void SetDebugSetting(CollisionType _debugType, const float4& _color)
	{
		debugType_ = _debugType;
		color_ = _color;
	}

	void SetDebugCamera(CameraOrder _cameraOrder)
	{
		debugCameraOrder_ = _cameraOrder;
	}

private:
	void Start() override;

private:
	CollisionType debugType_;
	float4 color_;
	CameraOrder debugCameraOrder_;
};

