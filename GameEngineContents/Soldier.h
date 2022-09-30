#pragma once
#include "GlobalHeader.h"
#include "Soldier_Header.h"

class PixelIndicator;
class Indicator;
class Soldier : public GameEngineActor
{
	//�� Ŭ������ ���� ����: 
public:
	Soldier();
	~Soldier();

protected:
	Soldier(const Soldier& _other) = delete;
	Soldier(Soldier&& _other) noexcept = delete;

private:
	Soldier& operator=(const Soldier& _other) = delete;
	Soldier& operator=(const Soldier&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

	//void TakeWeapon(int _weaponType);
	//void ReleasePrisoner(int _prisonerType);
	void TakeDamage(int _rebelWeaponType);	//������ �޴� �Լ�. �÷��̾�� ���� ���°� �ƴϸ� �ѹ濡 �����Ƿ� ���� �¾Ҵ��� ������ �޴´�.
	//�����δ� ȣ�� ����.


private:
	void CreateSoldierAnimations();
	void CreateSoldierStates();

	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();			

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);	

	//Ű�Է� ���� ������Ʈ.
	void UpdateInputInfo();	

	//Ű�Է� ������ �� ������ ������Ʈ�� ��ȯ.
	void ConvertInputToSoldierStates();	

	//�ѱ���ġ ��ȭ.
	void ControlMuzzle();		

	//�÷��̾� ��ü ���� ������Ʈ.
	void UpdateSoldierState(float _deltaTime);	

	//��� ����� ����� �̵����� ���������� �����ϰ� �̵����� �����ϴ� �Լ�.
	void MoveSoldier(float _deltaTime);

	void Run();			//�޸���.
	void DuckStep();	//��������.
	float GetSlope();	//Run(), DuckStep()�Լ��� ���� �̵��Ҷ� ��簢 ���ϴ� �Լ�.
	void Fire();		//�Ѿ� ���� �� �߻�� �ʿ��� ���� �Է�.
	void MeleeAttack();	//��������.
	void Flicker(		//������.
		float _deltaTime,
		bool _isFlickeringOn,
		const float4& _plusColor,
		const float4& _originalColor = float4::Zero
	);

private:

	SoldierState currentSoldierState_;

	SoldierWeaponType weapon_;
	SoldierLegState leg_;
	SoldierTopState top_;
	AimingDirection direction_;

	GameEngineStateManager soldierStateManager_;

	std::unordered_map<int, const std::pair<const SoldierState, const char*>> allSoldierStates_;	//��� ����(�÷��̾�) ������Ʈ.
	//����, ��ȸ, ������ �ѹ��� �ϰ�, Ž���� ��Ÿ�� ���� �� �����̹Ƿ� Ž�� ȿ���� ���ٰ� �˷��� ������ �� ���.
	//��� ������ ���� �Ẹ�� �; ����.

	bool isAirborne_;		//false: ���� ����. true: ���߿� �� �ִ� ����.


	const int soldierRendererLocalPosX_;
	const int soldierRendererLocalPosY_;
	const int soldierRendererLocalPosZ_;


	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;
	GameEngineTextureRenderer* redeployingRenderer_;
	std::list<GameEngineTextureRenderer*> allTextureRenderers_;


	char horizontalInputValue_;	//-1: ���� �Է�, 0: �߸�, 1: ���� �Է�.
	char verticalInputValue_;	//-1: �ϴ� �Է�, 0, �߸�, 1: ��� �Է�.
	bool isJumpKeyDown_;
	bool isAttackKeyDown_;
	bool isSpecialKeyDown_;


	PixelIndicator* upperLandingChecker_;		
	PixelIndicator* soldierWorldPosPointer_;	//������ ���������� ǥ��.
	PixelIndicator* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	PixelIndicator* slopeChecker_;		
	PixelIndicator* ascendingSlopeChecker_;			
	PixelIndicator* flatSlopeChecker_;
	PixelIndicator* descendingSlopeChecker_;

	PixelColor currentGroundColor_;


	Indicator* muzzleIndicator_;		//�ѱ���ġ ǥ�ñ�.
	const float4 pistolForwardMuzzlePosition_;	//���� ���������� �ѱ���ġ 
	const float4 pistolUpwardMuzzlePosition_;	//���� �����������ѱ���ġ 
	const float4 pistolDownwardMuzzlePosition_;	//�Ʒ��� �����������ѱ���ġ 
	const float4 pistolDuckingMuzzlePosition_;	//�ɱ׷� �ɾ����� �ѱ���ġ 

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float4 movementFor1Second_;		//1�� ������ �̵���. ��ŸŸ�Ӱ� �÷��� �ӵ��� MoveSoldier()�Լ����� �ѹ��� ����Ѵ�.

	float aimingAngle_;

	GameEngineCollision* soldierLifeCollisionBody_;
	GameEngineCollision* soldierCloseCombatCollisionBody_;

	const float4 soldierLifeCollisionBodyScale_Standing_;		//���� ������ �浹ü �������� ���� ũ��.
	const float4 soldierLifeCollisionBodyPosition_Standing_;	//���� ������ �浹ü �������� ���� ��ġ.
	const float4 soldierLifeCollisionBodyScale_Ducking_;		//���� ������ �浹ü �ɾ� ������ ���� ũ��.
	const float4 soldierLifeCollisionBodyPosition_Ducking_;		//���� ������ �浹ü �ɾ� ������ ���� ��ġ.

	const int meleeAttackDamage_;	//�������� ������.
	bool isMeleeAttack1_;	//true: �������� �ִϸ��̼�1. false: �������� �ִϸ��̼�2.

	int causeOfDeath_;		//���� �������. 0: ������ ���� �������. 
	bool isDamageProof_;	//true: ��������. 
	float remainedDamageProofDuration_;	//���� ���� ���ӽð�.
	const float damageProofTimeLimit_;	//���� �ð�����.
	const float flickeringPeriod_;//������ �ֱ�.

};

