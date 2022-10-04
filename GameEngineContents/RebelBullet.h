#pragma once

class RebelBullet: public GameEngineActor
{
	//이 클래스의 존재 이유: 반란군 총알.

	friend class CamelRider;

public:
	RebelBullet();
	~RebelBullet();

protected:
	RebelBullet(const RebelBullet& _other) = delete;
	RebelBullet(RebelBullet&& _other) noexcept = delete;

private:
	RebelBullet& operator=(const RebelBullet& _other) = delete;
	RebelBullet& operator=(const RebelBullet&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	CollisionReturn Hit(GameEngineCollision* _thisCollision, GameEngineCollision* _soldierCollision);

private:
	void SetFiringDirection(float _direction)
	{
		firingDirection_ = static_cast<char>(_direction);
		if (0 == firingDirection_)
		{
			MsgBoxAssert("그런 사격 방향은 지정할 수 없습니다.");
			return;
		}
	}

private:
	const float bulletSpeed_;
	char firingDirection_;
	GameEngineTextureRenderer* bulletRenderer_;
	GameEngineCollision* bulletCollisionBody_;


};

