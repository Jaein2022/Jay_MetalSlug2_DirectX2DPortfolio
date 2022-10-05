#pragma once
#include "GlobalHeader.h"

class Indicator;
class PixelIndicator;
class Sword : public GameEngineActor
{
	//이 클래스의 존재 이유: 아라비안 투척검.

	friend class Arabian;

public:
	Sword();
	~Sword();

protected:
	Sword(const Sword& _other) = delete;
	Sword(Sword&& _other) noexcept = delete;

private:
	Sword& operator=(const Sword& _other) = delete;
	Sword& operator=(const Sword&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

private:
	void Fly(float _deltaTime);
	void StickOnGround(float _deltaTime);
	void CheckGround();
	CollisionReturn Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _soldierCollision);
	void Flicker(		//깜빡임.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);


private:
	inline void SetReleaseSpeed(float _direction, float _velocity)
	{
		releaseSpeed_ = float4::DegreeToDirection2D(_direction) * _velocity;
		rotationSpeed_ = _velocity * 2.f;
	}

private:

	const float4 swordRendererLocalPos_;

	GameEngineTextureRenderer* flyingSwordRenderer_;
	GameEngineTextureRenderer* stuckSwordRenderer_;

	GameEngineCollision* swordCollisionBody_;

	bool isAirborne_;		//false: 착지 상태. true: 공중에 떠 있는 상태.

	Indicator* renderPivotPointer_;			//액터의 렌더피봇 표시.
	PixelIndicator* upperLandingChecker_;
	PixelIndicator* swordWorldPosPointer_;	//액터의 월드포지션 표시.
	PixelIndicator* lowerLandingChecker_;

	float rotationSpeed_;
	const float flickeringPeriod_;
	float remainingPeriod_;
	bool flickeringSwitch_;

	float4 releaseSpeed_;

};

