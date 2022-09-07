#pragma once

class TestIndicator;
class TestPixelIndicator;
class TestSword : public GameEngineActor
{
	//이 클래스의 존재 이유:
public:
	TestSword();
	~TestSword();

protected:
	TestSword(const TestSword& _other) = delete;
	TestSword(TestSword&& _other) noexcept = delete;

private:
	TestSword& operator=(const TestSword& _other) = delete;
	TestSword& operator=(const TestSword&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

public:
	inline void SetReleaseSpeed(float _direction, float _velocity)
	{
		releaseSpeed_ = float4::DegreeToDirection2D(_direction) * _velocity;
		rotationSpeed_ = _velocity * 2.f;
	}

private:
	void Fly(float _deltaTime);
	void StickOnGround();
	void CheckGround();
	bool Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _playerCollision);

private:

	const int swordRendererLocalPosX_;
	const int swordRendererLocalPosY_;
	const int swordRendererLocalPosZ_;

	GameEngineTextureRenderer* flyingSwordRenderer_;
	GameEngineTextureRenderer* stuckSwordRenderer_;

	GameEngineCollision* swordCollision_;

	bool isFalling_;		//false: 착지 상태. true: 공중에 떠 있는 상태.

	TestIndicator* renderPivotPointer_;			//액터의 렌더피봇 표시.
	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* swordWorldPosPointer_;	//액터의 월드포지션 표시.
	TestPixelIndicator* lowerLandingChecker_;

	float rotationSpeed_;

	float4 releaseSpeed_;

};

