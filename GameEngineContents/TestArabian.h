#pragma once

enum class ArabianState
{
	Idling,
	Shuffling,
	PreparingToAttack,
	Running,
	Jumping, 
	Falling,
	FallingToIdling,
	ThrowingSword,
	MeleeAttack,
	Dead
};


class TestIndicator;
class TestPixelIndicator;
class TestArabian: public GameEngineActor
{
	//�� Ŭ������ ���� ����: ������ ����, ��/�÷��̾� ��� ��� ����.
public:
	TestArabian();
	~TestArabian();

protected:
	TestArabian(const TestArabian& _other) = delete;
	TestArabian(TestArabian&& _other) noexcept = delete;

private:
	TestArabian& operator=(const TestArabian& _other) = delete;
	TestArabian& operator=(const TestArabian&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

public:
	inline void TakeDamage(int _damage)
	{
		hp_ -= _damage;
	}

private:

	//���� ���� ������ �߶��� ���߰�, ���� ���� ������ �߶� �����ϴ� �Լ�. 
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	//�÷��̾� ��ġ�� ���� ������Ʈ ��ȭ.
	void ReactToPlayerPosition();

	//�ƶ��� ������Ʈ ��ȯ �� ������Ʈ.
	void UpdateArabianState(float _deltaTime);

	//��� ����� ����� �̵����� ������ �����ϰ� �̵����� �����ϴ� �Լ�.
	void MoveArabian(float _deltaTime);

	void Shuffle();	//������ �̵�.
	void Run();		//�޸���.
	void SetSlopeCheckerDirection(char _localDirection);	//���üĿ�� ���ù��� ��ȯ.
	float GetSlope(char _localDirection);
	void ThrowSword();		
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);	//�������� �ִϸ��̼� �� ������.

private:

	ArabianState currentArabianState_;

	bool isFalling_;		//false: ���� ����. true: ���߿� �� �ִ� ����.
	//bool isEngaging_;

	const int arabianRendererLocalPosX_;
	const int arabianRendererLocalPosY_;
	const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianLifeCollisionBody_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

	TestIndicator* renderPivotPointer_;		//������ �����Ǻ� ǥ��.


	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* arabianWorldPosPointer_;	//������ ���������� ǥ��.
	TestPixelIndicator* lowerLandingChecker_;

	const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	TestPixelIndicator* slopeChecker_;
	TestPixelIndicator* ascendingSlopeChecker_;
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;

	GameEngineStateManager arabianStateManager_;

	std::map<const ArabianState, const char*> allArabianStates_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float shufflingSpeed_;

	char shuffleDirection_;		//���� ���϶� �̵�����.
	//-1: ���� ��������� ������. 0: ����. 1: ���� ��������� ������.

	float4 movementFor1Second_;	//1�� ������ �̵���. ��ŸŸ�Ӱ� �÷��� �ӵ��� MoveArabian()�Լ����� �ѹ��� ����Ѵ�.


	TestIndicator* releasePoint_;
	float releaseAngle_;
	float releaseVelocity_;

	const float recognitionDistance_;	//�νİŸ�.
	const float engagementDistance_;	//�����Ÿ�.
	const float chargeDistance_;		//�����Ÿ�.

	int hp_;

};

