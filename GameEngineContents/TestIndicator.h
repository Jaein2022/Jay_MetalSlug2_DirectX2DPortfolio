#pragma once

class GameEngineTexture;
class TestIndicator : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유: 특정 지점을 소량의 오버헤드로 표시 + 픽셀충돌 보조.

	static bool isRendering_;
	static GameEngineTexture* pixelCollisionTexture_;




private:
	TestIndicator();
	~TestIndicator();
	TestIndicator(const TestIndicator& _other) = delete;
	TestIndicator(TestIndicator&& _other) noexcept = delete;
	TestIndicator& operator=(const TestIndicator& _other) = delete;
	TestIndicator& operator=(const TestIndicator&& _other) = delete;


public:
	static TestIndicator* CreateIndicator(
		const std::string& _name,
		GameEngineActor* _parentActor,
		const float4& _color,
		const float4& _localPos,
		const float4& _localScale = float4(5, 5, 1)
	);
	UINT GetColorValue_UINT();
	

public:
	static void RenderingOnOffSwitch()
	{
		isRendering_ = !isRendering_;
	}
	static void SetTexture(GameEngineTexture* _pixelCollisionTexture)
	{
		pixelCollisionTexture_ = _pixelCollisionTexture;
		if (nullptr == pixelCollisionTexture_)
		{
			MsgBoxAssert("텍스처가 없습니다.");
			return;
		}
	}
	static void SetTexture(const std::string&  _pixelCollisionTextureName)
	{
		pixelCollisionTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pixelCollisionTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": 그런 이름의 텍스처가 없습니다.");
			return;
		}
	}

private:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void Render(float _deltaTime) override;

private:
	float4 color_;


};

