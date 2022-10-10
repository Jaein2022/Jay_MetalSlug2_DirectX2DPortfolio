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
	//�� Ŭ������ ���� ����: ���¼���Ʈ��.
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
	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	void Drive();

	void UpdateTruckState(float _deltaTime);

	void MoveTruck(float _deltaTime);

private: 
	GameEngineStateManager truckStateManager_;

	TruckState currentTruckState_;

	std::unordered_map<TruckState, const char*> allTruckStates_;



	PixelIndicator* upperLandingChecker_;
	PixelIndicator* truckWorldPosPointer_;	//������ ���������� ǥ��.
	PixelIndicator* lowerLandingChecker_;

	bool isAirborne_;
	float4 movementFor1Second_;
	float fallingSpeed_;

	GameEngineTextureRenderer* truckRenderer_;
	const float4 truckRendererLocalPos_;

	GameEngineCollision* truckDurabilityCollisionBody_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.
	int truckDurability_;


	GameEngineCollision* truckSlopeCollisionBody_;
	GameEngineCollision* truckWreckageCollisionBody1_;
	GameEngineCollision* truckWreckageCollisionBody2_;
	GameEngineCollision* truckWreckageCollisionBody3_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

	const float drivingSpeed_;

};

