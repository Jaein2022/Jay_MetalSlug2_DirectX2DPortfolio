#pragma once
#include "TestPlayer_Header.h"

class TestPixelIndicator;
class TestIndicator;
class TestPlayer : public GameEngineActor
{
	//�� Ŭ������ ���� ����: 
public:
	TestPlayer();
	~TestPlayer();

protected:
	TestPlayer(const TestPlayer& _other) = delete;
	TestPlayer(TestPlayer&& _other) noexcept = delete;

private:
	TestPlayer& operator=(const TestPlayer& _other) = delete;
	TestPlayer& operator=(const TestPlayer&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


public:


private:
	void CreatePlayerAnimations();
	void CreatePlayerStates();

	void UpdateInputInfo();	//Ű�Է� ������Ʈ.
	void ConvertInputToPlayerStates();

	void UpdatePlayerState(float _deltaTime);	//�÷��̾� ��ü ���� ������Ʈ.

	void Run(float _deltaTime);
	void DuckStep(float _deltaTime);
	float GetSlope();
	void Fall(float _deltaTime);
	void CheckGround();
	void ControlMuzzle();
	void Fire();


private:
	//TestLevel* testLevel_;

	PlayerState currentPlayerState_;

	PlayerWeaponType weapon_;
	PlayerLegState leg_;
	PlayerTopState top_;
	AimingDirection direction_;

	GameEngineStateManager playerStateManager_;

	std::map<const int, const std::pair<const PlayerState, const char*>> allPlayerStates_;

	bool isFalling_;		//false: ���� ����. true: ���߿� �� �ִ� ����.


	const int playerRendererLocalPosX_;
	const int playerRendererLocalPosY_;
	const int playerRendererLocalPosZ_;


	GameEngineTextureRenderer* legRenderer_;
	GameEngineTextureRenderer* topPistolRenderer_;
	GameEngineTextureRenderer* wholePistolRenderer_;
	GameEngineTextureRenderer* topWeaponRenderer_;
	GameEngineTextureRenderer* wholeWeaponRenderer_;


	char horizontalInputValue_;	//-1: ���� �Է�, 0: �߸�, 1: ���� �Է�.
	char verticalInputValue_;	//-1: �ϴ� �Է�, 0, �߸�, 1: ��� �Է�.
	bool isJumpKeyDown_;
	bool isAttackKeyDown_;
	bool isSpecialKeyDown_;

	//TestIndicator* renderPivotPointer_;		//�÷��̾��� �����Ǻ� ǥ��.


	TestPixelIndicator* upperLandingChecker_;		
	TestPixelIndicator* playerWorldPosPointer_;	//������ ���������� ǥ��.
	TestPixelIndicator* lowerLandingChecker_;		

	const int slopeCheckerLocalPosX_;		//���üĿ���� �������� X��. 0 ���� �� ��!
	TestPixelIndicator* slopeChecker_;		
	TestPixelIndicator* ascendingSlopeChecker_;			
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;


	TestIndicator* muzzleIndicator_;		//�ѱ���ġ ǥ�ñ�.
	const float4 pistolForwardMuzzlePosition_;
	const float4 pistolUpwardMuzzlePosition_;
	const float4 pistolDownwardMuzzlePosition_;
	const float4 pistolDuckingMuzzlePosition_;

	const float initialJumpSpeed_;
	float fallingSpeed_;

	const float runningSpeed_;
	const float duckStepSpeed_;

	float aimingAngle_;


	GameEngineCollision* playerCollision_;
};

