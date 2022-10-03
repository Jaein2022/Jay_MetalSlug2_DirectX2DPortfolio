#pragma once
#include "Rebel.h"



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

class Indicator;
class PixelIndicator;
class Soldier;
class Arabian : public Rebel
{
	//�� Ŭ������ ���� ����: ������ ����, ��/�÷��̾� ��� ��� ����.
public:
	Arabian();
	~Arabian();

protected:
	Arabian(const Arabian& _other) = delete;
	Arabian(Arabian&& _other) noexcept = delete;

private:
	Arabian& operator=(const Arabian& _other) = delete;
	Arabian& operator=(const Arabian&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	void TakeDamage(
		int _damage,
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) override;


private:
	void CreateArabianAnimations();
	void CreateArabianStates();

	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	//�������� �Ÿ�, ���� �Ǵ�.
	void GetDistanceAndDirection(float _deltaTime);

	//�ƶ��� ������Ʈ ��ȯ �� ������Ʈ.
	void UpdateArabianState(float _deltaTime);

	//��� ����� ����� �̵����� ������ �����ϰ� �̵����� �����ϴ� �Լ�.
	void MoveArabian(float _deltaTime);

	//�ݶ��� �ƶ����� ���� ������Ʈ�� �����ϴ� �Լ�. 
	//_minStateIndex�� _maxStateIndex�� ������ �־�� ������ �������ڷ� ���� ���ܴ���� �ƶ��Ƚ�����Ʈ�� �־�� �Ѵ�.
	//_maxStateIndex�� 1���� ū ������ �־�� ������ _minStateIndex�� 0�̳� ������ ���� �� �ְ�,
	// �������� Ŀ������ 0���� ArabianState::Shuffling�� ���� Ȯ���� �ö󰡼� �׸�ŭ ���� �󵵿� ���̵��� �پ���.
	void SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...);

	//������ �̵�.
	void Shuffle();	

	//�޸���.
	void Run();		

	//����.
	void Jump(const FrameAnimation_Desc& _desc);

	//���üĿ�� ���ù��� ��ȯ.
	void SetSlopeCheckerDirection(char _localDirection);	

	//��簪 ���ϱ�.
	float GetSlope(char _localDirection);

	//�� ��ô.
	void ThrowSword();		

	//�������� �ִϸ��̼� �� ������.
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);	

	void MeleeAttack();

	//������ �ִϸ��̼� �� ������.
	void JumpBackWard();	

private:

	ArabianState currentArabianState_;

	bool isAirborne_;		//true: ���߿� �� �ִ� ����. false: ���� ����.
	bool isEngaging_;		//true: ���� ��. false: �÷��̾� �ν� ����.

	const float4 arabianRendererLocalPos_;

	GameEngineTextureRenderer* arabianRenderer_;

	GameEngineCollision* arabianLifeCollisionBody_;
	GameEngineCollision* arabianCloseCombatCollisionBody_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

	Indicator* renderPivotPointer_;		//������ �����Ǻ� ǥ��.


	PixelIndicator* upperLandingChecker_;
	PixelIndicator* arabianWorldPosPointer_;	//������ ���������� ǥ��.
	PixelIndicator* lowerLandingChecker_;

	const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	PixelIndicator* slopeChecker_;
	PixelIndicator* ascendingSlopeChecker_;
	PixelIndicator* flatSlopeChecker_;
	PixelIndicator* descendingSlopeChecker_;

	PixelColor currentSteppingColor_;


	GameEngineStateManager arabianStateManager_;

	std::unordered_map<ArabianState, const char*> allArabianStates_;
	//����, ��ȸ, ������ �ѹ��� �ϰ�, Ž���� ��Ÿ�� ���� �� �����̹Ƿ� Ž�� ȿ���� ���ٰ� �ϴ� ������ �� ���.
	//��� ������ ���� �Ẹ�� �; ����.

	const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float shufflingSpeed_;

	char shuffleDirection_;		//���� ���϶� �̵�����.
	//-1: ���� ��������� ������. 0: ������ȯ �ʿ����. 1: ���� ��������� ������.

	float4 movementFor1Second_;	//1�� ������ �̵���. ��ŸŸ�Ӱ� �÷��� �ӵ��� MoveArabian()�Լ����� �ѹ��� ����Ѵ�.

	Soldier* enemySoldier_;

	float currentTurningDelay_;
	const float turningDelay_;
	char nextWorldDirection_;	//�ƶ����� ���ƺ��� �ϴ� ����.
	//-1: ���� ��������� ���������� ������ȯ. 0: ������ȯ �ʿ����. 1: ���� ��������� ���������� ������ȯ.

	Indicator* releasePoint_;	//�� ��ô ����. �÷��̾��� ����� ���� ����.
	float releaseAngle_;			//�� ��ô ����.
	float releaseVelocity_;			//�� ��ô �ӵ�.

	float horizontalDistance_;			//�÷��̾���� ���� �Ÿ�. ���̴� �ݿ����� ����.
	const float recognitionDistance_;	//�νİŸ�.
	const float engagementDistance_;	//�����Ÿ�.
	const float chargeDistance_;		//�����Ÿ�.

	int hp_;

};

