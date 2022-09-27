#pragma once


enum class ArabianState
{
	Shuffling,
	PreparingToAttack,
	JumpingBackward,
	Running,
	ThrowingSword,
	MeleeAttack,
	Jumping,
	Turning,
	Falling,
	FallingToIdling,
	Idling,
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
	void CreateArabianAnimations();
	void CreateArabianStates();

	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	//�÷��̾���� �Ÿ� �Ǵ�.
	void GetDistance(float _deltaTime);

	//�ƶ��� ������Ʈ ��ȯ �� ������Ʈ.
	void UpdateArabianState(float _deltaTime);

	//��� ����� ����� �̵����� ������ �����ϰ� �̵����� �����ϴ� �Լ�.
	void MoveArabian(float _deltaTime);

	//�ݶ��� �ƶ����� ���� ������Ʈ�� �����ϴ� �Լ�. 
	//_minStateIndex�� _maxStateIndex�� ������ �־�� ������ �������ڷ� ���� ���ܴ���� �ƶ��Ƚ�����Ʈ�� �־�� �Ѵ�.
	//_maxStateIndex�� 1���� ū ������ �־�� ������ _minStateIndex�� 0�̳� ������ ���� �� �ְ�,
	// �������� Ŀ������ 0���� ArabianState::Shuffling�� ���� Ȯ���� �ö󰡼� �׸�ŭ ���� �󵵿� ���̵��� �پ���.
	void SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...);

	void Shuffle();	//������ �̵�.
	void Run();		//�޸���.
	void SetSlopeCheckerDirection(char _localDirection);	//���üĿ�� ���ù��� ��ȯ.
	float GetSlope(char _localDirection);
	void ThrowSword();		
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);	//�������� �ִϸ��̼� �� ������.
	void MeleeAttack();
	void JumpBackWard();	//������ �ִϸ��̼� �� ������.

private:

	ArabianState currentArabianState_;

	bool isAirborne_;		//true: ���߿� �� �ִ� ����. false: ���� ����.
	bool isEngaging_;		//true: ���� ��. false: �÷��̾� �ν� ����.

	const int arabianRendererLocalPosX_;
	const int arabianRendererLocalPosY_;
	const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianLifeCollisionBody_;
	GameEngineCollision* arabianCloseCombatCollisionBody_;
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

	std::unordered_map<ArabianState, const char*> allArabianStates_;
	//����, ��ȸ, ������ �ѹ��� �ϰ�, Ž���� ��Ÿ�� ���� �� �����̹Ƿ� Ž�� ȿ���� ���ٰ� �ϴ� ������ �� ���.
	//��� ������ ���� �Ẹ�� �; ����.
	//�ؽ� �Լ��� ���� ������ ����� ��� �Ϲ� ������ ������ų ��.

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float shufflingSpeed_;

	char shuffleDirection_;		//���� ���϶� �̵�����.
	//-1: ���� ��������� ������. 0: ����. 1: ���� ��������� ������.

	float4 movementFor1Second_;	//1�� ������ �̵���. ��ŸŸ�Ӱ� �÷��� �ӵ��� MoveArabian()�Լ����� �ѹ��� ����Ѵ�.

	const float turningDelay_;
	char nextWorldDirection_;	//�ƶ����� ���ƺ��� �ϴ� ����.
	//-1: ���� ��������� ���������� ������ȯ. 0: ������ȯ �ʿ����. 1: ���� ��������� ���������� ������ȯ.

	TestIndicator* releasePoint_;	//�� ��ô ����. �÷��̾��� ����� ���� ����.
	float releaseAngle_;			//�� ��ô ����.
	float releaseVelocity_;			//�� ��ô �ӵ�.

	float horizontalDistance_;			//�÷��̾���� ���� �Ÿ�. ���̴� �ݿ����� ����.
	const float recognitionDistance_;	//�νİŸ�.
	const float engagementDistance_;	//�����Ÿ�.
	const float chargeDistance_;		//�����Ÿ�.

	int hp_;

};

