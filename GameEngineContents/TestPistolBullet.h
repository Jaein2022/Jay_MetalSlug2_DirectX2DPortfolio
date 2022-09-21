#pragma once

class TestPixelIndicator;
class TestPistolBullet: public GameEngineActor
{
	//이 클래스의 존재 이유: 권총 총알.
public:
	TestPistolBullet();
	~TestPistolBullet();

protected:
	TestPistolBullet(const TestPistolBullet& _other) = delete;
	TestPistolBullet(TestPistolBullet&& _other) noexcept = delete;

private:
	TestPistolBullet& operator=(const TestPistolBullet& _other) = delete;
	TestPistolBullet& operator=(const TestPistolBullet&& _other) = delete;


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
	TestPixelIndicator* groundChecker_;
	
	float4 firingDirection_;

	const int damage_;
};

