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
	bool IsCollision(		//충돌 여부 판정 함수.
		CollisionType _thisType,
		int _collisionOrder,
		CollisionType _otherType,
		std::function<bool(GameEngineCollision* _this, GameEngineCollision* _other)> _function = nullptr);
	void DebugRender();	//함수 이름과는 다르게, 실제로는 충돌체를 직접 그리지 않고 그리는데 필요한 정보만 저장하는 함수.
	//이 함수에서 저장된 정보대로 GameEngineCoreDebug의 Debug3DRender()함수에서 진짜 렌더링을 한다.

	void SetUIDebugCamera();	//이 콜리전의 렌더링을 UI카메라에서 하게 하는 함수.
	//UI카메라는 엔진 기본제공 카메라이므로 엔진 수준에서 이런 편의함수를 제공할 수 있다.

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

