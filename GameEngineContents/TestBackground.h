#pragma once

class TestBackground: public GameEngineActor
{
	//이 클래스의 존재 이유: 픽셀충돌 테스트.

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
	PixelColor GetPixelColor(int _x, int _y)
	{
		return pixelCollisionBGRenderer_->GetCurrentTexture()->GetPixelToPixelColor(
			GameEngineWindow::GetScale().HIX() + _x,
			(GameEngineWindow::GetScale().IY() - (_y + GameEngineWindow::GetScale().HIY())
			- (GameEngineWindow::GetScale().IY() - pixelCollisionBGRenderer_->GetCurrentTexture()->GetScale().IY()))
		);
	}

private:

private:
	GameEngineTextureRenderer* firstBGRenderer_;
	GameEngineTextureRenderer* pixelCollisionBGRenderer_;

};

