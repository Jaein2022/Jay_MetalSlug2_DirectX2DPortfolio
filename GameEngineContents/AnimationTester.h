#pragma once

class AnimationTester : public GameEngineActor
{
	//이 클래스의 존재 이유: 애니메이션 테스트 및 리소스 편집방향 결정.
public:
	AnimationTester();
	~AnimationTester();

protected:
	AnimationTester(const AnimationTester& _other) = delete;
	AnimationTester(AnimationTester&& _other) noexcept = delete;

private:
	AnimationTester& operator=(const AnimationTester& _other) = delete;
	AnimationTester& operator=(const AnimationTester&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* legRenderer_;
	float velocity_;
	bool isMoving_;

};

