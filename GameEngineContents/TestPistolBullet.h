#pragma once

class TestPixelIndicator;
class TestPistolBullet: public GameEngineActor
{
	//�� Ŭ������ ���� ����: ���� �Ѿ�.
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
	void SetFiringDirection(float _direction)
	{

		firingDirection_ = float4::DegreeToDirection2D(_direction);
	}

private:
	bool CheckGroundHit();

private:
	const float bulletSpeed_;
	GameEngineCollision* pistolBulletCollision_;
	GameEngineTextureRenderer* pistolBulletRenderer_;
	GameEngineTextureRenderer* effectiveHitSparkRenderer_;
	GameEngineTextureRenderer* glancingHitSparkRenderer_;
	TestPixelIndicator* groundChecker_;
	
	float4 firingDirection_;
};

