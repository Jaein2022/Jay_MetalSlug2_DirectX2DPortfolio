#pragma once
#include "GlobalHeader.h"

class CamelRider;
class Mission1BG;
class Soldier;
class Arabian;
class IndicatorBase;
class Indicator;
class Mission1 : public GameEngineLevel
{
	//이 클래스의 존재 이유: 미션 1.
public:
	Mission1();
	~Mission1();

protected:
	Mission1(const Mission1& _other) = delete;
	Mission1(Mission1&& _other) noexcept = delete;

private:
	Mission1& operator=(const Mission1& _other) = delete;
	Mission1& operator=(const Mission1&& _other) = delete;


public:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	void UpdateDestFocusMovement(float _deltaTime);
	void UpdateCameraActorMovement(float _deltaTime);


private:
	Mission1BG* mission1BG_;

	Indicator* currentFocusPointer_;
	IndicatorBase* destFocus_;

	const float destFocusVelocity_;
	bool isDestFocusHolding_;

	Soldier* soldier_Mission1_;

	Arabian* arabian1_;
	Arabian* arabian2_;
	Arabian* arabian3_;
	Arabian* arabian4_;
	Arabian* arabian5_;
	Arabian* arabian6_;
	Arabian* arabian7_;
	Arabian* arabian8_;
	Arabian* arabian9_;
	Arabian* arabian10_;
	Arabian* arabian11_;
	Arabian* arabian12_;
	Arabian* arabian13_;
	Arabian* arabian14_;
	Arabian* arabian15_;
	Arabian* arabian16_;
	Arabian* arabian17_;
	Arabian* arabian18_;
	Arabian* arabian19_;
	Arabian* arabian20_;

	CamelRider* camelRider_;

	bool isCamelRiderDead_;
	bool isTruckDestroyed_;

	GameEngineSoundPlayer mission1BgmPlayer_;

};

