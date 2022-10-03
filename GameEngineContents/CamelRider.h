#pragma once
#include "Rebel.h"

enum class CamelRiderState
{
	Up_Entrance,
	Up_Idling,
	Up_Running,
	Up_RunningBackward,
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

class Soldier;
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

	void TakeDamage(
		int _damage,
		GameEngineCollision* _soldierWeaponCollision,
		GameEngineCollision* _rebelCollision
	) override;

private:
	//착지할때 정확한 액터위치 조정/땅에 발이 안 닿아있으면 추락 판정.
	void CheckGround();

	//추락 이동값 적용.
	void Fall(float _deltaTime);

	
	void GetDirection(float _deltaTime);

	//낙타기수 스테이트 변환 및 업데이트.
	void UpdateCamelRiderState(float _deltaTime);

	//모든 계산이 적용된 이동값을 실제로 적용하는 함수.
	void MoveCamel(float _deltaTime);

	void SelectNextState(int _minStateIndex, int _maxStateIndex, int _exclusionCount, ...);

	void Run();			//달리기.
	void RunBackward();	//뒤로 달리기.

	void Fire();




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
	PixelIndicator* camelRiderWorldPosPointer_;	//액터의 월드포지션 표시.
	PixelIndicator* lowerLandingChecker_;

	float fallingSpeed_;
	float runningSpeed_;

	float4 movementFor1Second_;	//1초 동안의 이동량.

	bool isAirborne_;		//true: 공중에 떠 있는 상태. false: 착지 상태.
	bool isEngaging_;		//true: 교전 중. false: 플레이어 인식 못함.
	bool isSwordBroken_;	//true: 검 깨짐.


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

	Soldier* enemySoldier_;
	
	char riderLocalDirection_;	//기수가 보는 방향.
	//-1: 액터 월드방향의 역방향. 0: 에러. 1: 액터 월드방향의 정방향.


	//Indicator* muzzle_;
	//const float4 muzzlePosition_Up_;
	//const float4 muzzlePosition_Down_;




};

