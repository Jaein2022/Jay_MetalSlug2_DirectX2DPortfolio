#pragma once

class GameEngineTexture;
class TestIndicator : public GameEngineDefaultRenderer
{
	//�� Ŭ������ ���� ����: Ư�� ������ �ҷ��� �������� ǥ�� + �ȼ��浹 ����.

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
			MsgBoxAssert("�ؽ�ó�� �����ϴ�.");
			return;
		}
	}
	static void SetTexture(const std::string&  _pixelCollisionTextureName)
	{
		pixelCollisionTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pixelCollisionTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": �׷� �̸��� �ؽ�ó�� �����ϴ�.");
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

