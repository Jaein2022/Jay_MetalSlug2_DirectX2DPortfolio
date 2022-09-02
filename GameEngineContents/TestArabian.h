#pragma once

class TestIndicator;
class TestPixelIndicator;
class TestArabian: public GameEngineActor
{
	//이 클래스의 존재 이유: 적과의 교전, 적/플레이어 사망 기능 구현.
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

	bool isJumping_;		//false: 착지 상태. true: 공중에 떠 있는 상태.

	//const int arabianRendererLocalPosX_;
	//const int arabianRendererLocalPosY_;
	//const int arabianRendererLocalPosZ_;


	GameEngineTextureRenderer* arabianRenderer_;


	TestIndicator* renderPivotPointer_;		//액터의 렌더피봇 표시.


	TestPixelIndicator* upperLandingChecker_;
	TestPixelIndicator* arabianWorldPosPointer_;	//액터의 월드포지션 표시.
	TestPixelIndicator* lowerLandingChecker_;

	//const int slopeCheckerLocalPosX_;		//경사체커들의 로컬포스 X값. 0 넣지 말 것!
	TestPixelIndicator* slopeChecker_;
	TestPixelIndicator* ascendingSlopeChecker_;
	TestPixelIndicator* flatSlopeChecker_;
	TestPixelIndicator* descendingSlopeChecker_;


	//const float initialJumpSpeed_;
	float fallingSpeed_;

	float runningSpeed_;

	float aimingAngle_;

	GameEngineCollision* arabianCollision_;
	//픽셀충돌 제외한 모든 충돌체는 월드크기 z값, 월드좌표 z값 10으로 고정.

};

