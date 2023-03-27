#pragma once
#include "Rebel.h"

enum class BerserkerState
{
	Waiting_SittingDown,
	Waiting_Standing,
	Idling,
	Running,
	Turning,
	MeleeAttack,
	Dead
};

class PixelIndicator;
class Soldier;
class Berserker : public Rebel
{
	//�� Ŭ������ ���� ����: ������.

	friend class DevelopmentTool;

public:
	Berserker();
	~Berserker();

protected:
	Berserker(const Berserker& _other) = delete;
	Berserker(Berserker&& _other) noexcept = delete;

private:
	Berserker& operator=(const Berserker& _other) = delete;
	Berserker& operator=(const Berserker&& _other) = delete;


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
	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	//�÷��̾���� �Ÿ�, ���� �Ǵ�.
	void UpdateDistanceAndDirection(float _deltaTime);

	//�ƶ��� ������Ʈ ��ȯ �� ������Ʈ.
	void UpdateBerserkerState(float _deltaTime);

	//��� ����� ����� �̵����� ������ �����ϰ� �̵����� �����ϴ� �Լ�.
	void MoveBerserker(float _deltaTime);


	//�޸���.
	void Run();

	//��簪 ���ϱ�.
	float GetSlope(char _localDirection);

	//�������� �ִϸ��̼� �� ������.
	void MoveInJumpDeath(const FrameAnimation_Desc& _desc);

	void MeleeAttack();


	void Flicker(		//������.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);

private:
	inline void SetBerserkerState(BerserkerState _newState)
	{
		currentBerserkerState_ = _newState;
	}

	void ResetRebelState() override
	{
		currentBerserkerState_ = BerserkerState::Idling;
	}


private:
	BerserkerState currentBerserkerState_;

	bool isAirborne_;		//true: ���߿� �� �ִ� ����. false: ���� ����.
	bool isEngaging_;		//true: ���� ��. false: �÷��̾� �ν� ����.

	const float4 berserkerRendererLocalPos_;

	GameEngineTextureRenderer* berserkerWholeRenderer_;
	GameEngineTextureRenderer* berserkerLegRenderer_;
	GameEngineTextureRenderer* berserkerTopRenderer_;

	GameEngineCollision* berserkerLifeCollisionBody_;
	GameEngineCollision* berserkerCloseCombatCollisionBody_;
	//�ȼ��浹 ������ ��� �浹ü�� ����ũ�� z��, ������ǥ z�� 10���� ����.

	PixelIndicator* upperLandingChecker_;
	PixelIndicator* midLandingChecker_;		//������ ���������ǵ� ǥ��.
	PixelIndicator* lowerLandingChecker_;


	GameEngineStateManager berserkerStateManager_;

	std::unordered_map<BerserkerState, const char*> allBerserkerStates_;
	//����, ��ȸ, ������ �ѹ��� �ϰ�, Ž���� ��Ÿ�� ���� �� �����̹Ƿ� Ž�� ȿ���� ���ٰ� �ϴ� ������ �� ���.
	//��� ������ ���� �Ẹ�� �; ����.

	float fallingSpeed_;

	const float runningSpeed_;

	float4 movementFor1Second_;	//1�� ������ �̵���. ��ŸŸ�Ӱ� �÷��� �ӵ��� MoveBerserker()�Լ����� �ѹ��� ����Ѵ�.

	Soldier* enemySoldier_;

	float currentTurningDelay_;
	const float turningDelay_;
	bool isBerserkerDirectionWrong_;	//true: �����簡 ������ȯ�� �ؾ� �Ѵ�.

	float horizontalDistance_;			//�÷��̾���� ���� �Ÿ�. ���̴� �ݿ����� ����.

	int hp_;

	const float flickeringPeriod_;//������ �ֱ�.
	float remainingPeriod_;
	bool flickeringSwitch_;

};

