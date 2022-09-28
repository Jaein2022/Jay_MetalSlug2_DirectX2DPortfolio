#pragma once
#include "GlobalHeader.h"

class PixelIndicator;
class PistolBullet: public GameEngineActor
{
	//이 클래스의 존재 이유: 권총 총알.
public:
	PistolBullet();
	~PistolBullet();

protected:
	PistolBullet(const PistolBullet& _other) = delete;
	PistolBullet(PistolBullet&& _other) noexcept = delete;

private:
	PistolBullet& operator=(const PistolBullet& _other) = delete;
	PistolBullet& operator=(const PistolBullet&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

public:
	inline void SetFiringDirection(float _direction)
	{
		firingDirection_ = float4::DegreeToDirection2D(_direction);
	}

private:
	bool CheckGroundHit();
	CollisionReturn Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _rebelCollision);

private:
	const float bulletSpeed_;
	GameEngineCollision* pistolBulletCollisionBody_;
	GameEngineTextureRenderer* pistolBulletRenderer_;
	GameEngineTextureRenderer* effectiveHitSparkRenderer_;
	GameEngineTextureRenderer* glancingHitSparkRenderer_;
	PixelIndicator* groundChecker_;
	
	float4 firingDirection_;

	const int damage_;
};

