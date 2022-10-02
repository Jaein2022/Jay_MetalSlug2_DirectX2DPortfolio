#pragma once
#include "Rebel.h"

enum class CamelRiderState
{
	Up_Entrance,
	Up_Idling,
	Up_Running,
	Up_Turning,
	Up_Firing,
	Up_SwordBreaking,
	UpToDown,
	Down_Idling,
	Down_Firing,
	Down_SwordBreaking,
	DownToUp,
	Dead
};

class PixelIndicator;
class Indicator;
class CamelRider: public Rebel
{

public:
	CamelRider();
	~CamelRider();

protected:
	CamelRider(const CamelRider& _other) = delete;
	CamelRider(CamelRider&& _other) noexcept = delete;

private:
	CamelRider& operator=(const CamelRider& _other) = delete;
	CamelRider& operator=(const CamelRider&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	//�����Ҷ� ��Ȯ�� ������ġ ����/���� ���� �� ��������� �߶� ����.
	void CheckGround();

	//�߶� �̵��� ����.
	void Fall(float _deltaTime);

	
	void GetDirection(float _deltaTime);

	//��Ÿ��� ������Ʈ ��ȯ �� ������Ʈ.
	void UpdateCamelRiderState(float _deltaTime);

	//��� ����� ����� �̵����� ������ �����ϴ� �Լ�.
	void MoveCamel(float _deltaTime);

	void Run();

	void Fire();

	void TakeDamage(
		int _damage,
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) override;


private:
	GameEngineStateManager camelRiderStateManager_;

	std::unordered_map<CamelRiderState, const char*> allCamelRiderStates_;

	CamelRiderState currentCamelRiderState_;

	GameEngineTextureRenderer* camelRenderer_;
	GameEngineTextureRenderer* riderRenderer_;
	GameEngineTextureRenderer* riderArmRenderer_;

	const float4 camelRendererLocalPos_;
	const float4 riderRendererLocalPos_Up_;
	const float4 riderRendererLocalPos_Down_;
	const float4 riderArmRendererLocalPos_Up_;
	const float4 riderArmRendererLocalPos_Down_;

	PixelIndicator* upperLandingChecker_;
	PixelIndicator* camelRiderWorldPosPointer_;	//������ ���������� ǥ��.
	PixelIndicator* lowerLandingChecker_;

	float fallingSpeed_;
	float runningSpeed_;

	float4 movementFor1Second_;	//1�� ������ �̵���.

	bool isAirborne_;		//true: ���߿� �� �ִ� ����. false: ���� ����.
	bool isEngaging_;		//true: ���� ��. false: �÷��̾� �ν� ����.
	bool isSwordBroken_;	//true: �� ����.


	GameEngineCollision* riderCollisionBody_;
	GameEngineCollision* swordCollisionBody_;

	const float4 riderCollisionBodyScale_Up_;
	const float4 riderCollisionBodyPosition_Up_;
	const float4 riderCollisionBodyScale_Down_;
	const float4 riderCollisionBodyPosition_Down_;

	const float4 swordCollisionBodyScale_Up_;
	const float4 swordCollisionBodyPosition_Up_;
	const float4 swordCollisionBodyScale_Down_;
	const float4 swordCollisionBodyPosition_Down_;

	int hp_;
	int swordDuration_;
	
	char riderLocalDirection_;

	//Indicator* muzzle_;
	//const float4 muzzlePosition_Up_;
	//const float4 muzzlePosition_Down_;




};
