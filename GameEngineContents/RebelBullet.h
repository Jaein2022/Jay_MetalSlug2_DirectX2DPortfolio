#pragma once

class RebelBullet: public GameEngineActor
{
	//�� Ŭ������ ���� ����: �ݶ��� �Ѿ�.

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
			MsgBoxAssert("�׷� ��� ������ ������ �� �����ϴ�.");
			return;
		}
	}

private:
	const float bulletSpeed_;
	char firingDirection_;
	GameEngineTextureRenderer* bulletRenderer_;
	GameEngineCollision* bulletCollisionBody_;


};

