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
	Death
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


private:
	void ReactToPlayerPosition();	
	void Shuffle(float _deltaTime);
	void UpdateArabianState(float _deltaTime);
	void Run(float _deltaTime);
	float CheckSlope();
	void Fall(float _deltaTime);
	void CheckFalling();
	void ThrowSword();



private:
	GameEngineStateManager arabianStateManager_;

	std::map<const ArabianState, const char*> allArabianStates_;

	ArabianState currentArabianState_;

	bool isFalling_;		//false: ���� ����. true: ���߿� �� �ִ� ����.
	//bool isEngaging_;

	const int arabianRendererLocalPosX_;
	const int arabianRendererLocalPosY_;
	const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;


	TestIndicator* renderPivotPointer_;		//������ �����Ǻ� ǥ��.


	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* arabianWorldPosPointer_;	//������ ���������� ǥ��.
	TestPixelIndicator* lowerLandingChecker_;

	const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	TestPixelIndicator* slopeChecker_;
	TestPixelIndicator* ascendingSlopeChecker_;
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;


	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float shufflingSpeed_;

	bool isLeft_;

	char localDirection_;		//-1: ���� ��������� ������. 0: ����. 1: ���� ��������� ������.

	float aimingAngle_;

	const float recognitionDistance_;	//�νİŸ�.
	const float engagementDistance_;	//�����Ÿ�.
	const float chargeDistance_;		//�����Ÿ�.


	GameEngineCollision* arabianCollision_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

};

