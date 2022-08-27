#pragma once

class TestBackground: public GameEngineActor
{
	//�� Ŭ������ ���� ����:

public:
	TestBackground();
	~TestBackground();

protected:
	TestBackground(const TestBackground& _other) = delete;
	TestBackground(TestBackground&& _other) noexcept = delete;

private:
	TestBackground& operator=(const TestBackground& _other) = delete;
	TestBackground& operator=(const TestBackground&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

public:

private:

private:
	GameEngineTextureRenderer* firstBGRenderer_;
	GameEngineTextureRenderer* pixelCollisionBGRenderer_;

};

