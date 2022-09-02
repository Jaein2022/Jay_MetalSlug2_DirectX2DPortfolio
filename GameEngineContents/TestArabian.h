#pragma once

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




private:
	GameEngineStateManager arabianStateManager_;

	//std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isJumping_;		//false: ���� ����. true: ���߿� �� �ִ� ����.

	//const int arabianRendererLocalPosX_;
	//const int arabianRendererLocalPosY_;
	//const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;


	TestIndicator* renderPivotPointer_;		//������ �����Ǻ� ǥ��.


	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* arabianWorldPosPointer_;	//������ ���������� ǥ��.
	TestPixelIndicator* lowerLandingChecker_;

	//const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	TestPixelIndicator* slopeChecker_;
	TestPixelIndicator* ascendingSlopeChecker_;
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;


	//const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float aimingAngle_;

	GameEngineCollision* arabianCollision_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

};

