#pragma once

class GameEngineTexture;
class TestPointer : public GameEngineDefaultRenderer
{
	//�� Ŭ������ ���� ����: Ư�� ������ �ҷ��� ������� ǥ��.
 
	//���� �ڽ�Ŭ������ �Ӹ� �ƴ϶� �ٸ� Ʈ������������Ʈ�� �θ�� �� �� �ְ� �ϰų�,
	// �ڱ� �ڽ��� �ƿ� ��Ʈ ������Ʈ�� �� �� �ְ� �� ��.
 
	//�ڽ�Ŭ���� TestPixelPointer�� ���� �ȼ��浹 �ؽ�ó �� �ȼ��浹 ���� ����� ���� �������� ������ ��.
	//�Ѱ��ϸ� ���� ������ ����������, ���� ���̴��� ���� ������ ������ ������ ����� �� ��.

	static GameEngineTexture* pixelCollisionTexture_;

	friend class TestPointerBase;

protected:
	static bool isRendering_;

protected:
	TestPointer();
	virtual ~TestPointer();
	TestPointer(const TestPointer& _other) = delete;
	TestPointer(TestPointer&& _other) noexcept = delete;
	TestPointer& operator=(const TestPointer& _other) = delete;
	TestPointer& operator=(const TestPointer&& _other) = delete;

public:
	UINT GetColorValue_UINT();



	

public:
	static TestPointer* CreatePointer(
		const std::string& _name,
		GameEngineActor* _parentActor,
		const float4& _color,
		const float4& _localPos,
		const float4& _localScale
	)
	{
		TestPointer* newPointer = new TestPointer();
		newPointer->SetName(_name);
		newPointer->color_ = _color;
		newPointer->SetParent(_parentActor);
		newPointer->GetTransform().SetLocalScale(_localScale);
		newPointer->GetTransform().SetLocalPosition(_localPos);
		newPointer->Start();
		//newPointer->SetRenderingOrder(5);
		return newPointer;
	}

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

protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void Render(float _deltaTime) override;

protected:
	float4 color_;


};

