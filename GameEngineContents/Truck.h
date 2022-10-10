#pragma once
#include "Rebel.h"

enum class TruckState
{
	Idling,
	Driving,
	Deploying,
	Destroyed
};

class PixelIndicator;
class Truck : public Rebel
{
	//이 클래스의 존재 이유: 병력수송트럭.
public:
	Truck();
	~Truck();

protected:
	Truck(const Truck& _other) = delete;
	Truck(Truck&& _other) noexcept = delete;

private:
	Truck& operator=(const Truck& _other) = delete;
	Truck& operator=(const Truck&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	void TakeDamage(
		int _damage,
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) override;

private:
	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();

	//추락 이동값 적용.
	void Fall(float _deltaTime);

	void Drive();

	void UpdateTruckState(float _deltaTime);

	void MoveTruck(float _deltaTime);

private: 
	GameEngineStateManager truckStateManager_;

	TruckState currentTruckState_;

	std::unordered_map<TruckState, const char*> allTruckStates_;



	PixelIndicator* upperLandingChecker_;
	PixelIndicator* truckWorldPosPointer_;	//액터의 월드포지션 표시.
	PixelIndicator* lowerLandingChecker_;

	bool isAirborne_;
	float4 movementFor1Second_;
	float fallingSpeed_;

	GameEngineTextureRenderer* truckRenderer_;
	const float4 truckRendererLocalPos_;

	GameEngineCollision* truckDurabilityCollisionBody_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.
	int truckDurability_;


	GameEngineCollision* truckSlopeCollisionBody_;
	GameEngineCollision* truckWreckageCollisionBody1_;
	GameEngineCollision* truckWreckageCollisionBody2_;
	GameEngineCollision* truckWreckageCollisionBody3_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.

	const float drivingSpeed_;

};

