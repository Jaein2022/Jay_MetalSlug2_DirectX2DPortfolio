#pragma once

class AnimationTester : public GameEngineActor
{
	//�� Ŭ������ ���� ����: �ִϸ��̼� �׽�Ʈ �� ���ҽ� �������� ����.
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

