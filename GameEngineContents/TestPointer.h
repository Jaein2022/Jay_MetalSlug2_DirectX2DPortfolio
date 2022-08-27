#pragma once

class GameEngineTexture;
class TestPointer : public GameEngineDefaultRenderer
{
	//이 클래스의 존재 이유: 특정 지점을 소량의 비용으로 표시.
 
	//액터 자식클래스들 뿐만 아니라 다른 트랜스폼컴포넌트를 부모로 둘 수 있게 하거나,
	// 자기 자신을 아예 루트 컴포넌트로 할 수 있게 할 것.
 
	//자식클래스 TestPixelPointer를 만들어서 픽셀충돌 텍스처 및 픽셀충돌 관련 기능은 전부 그쪽으로 이전할 것.
	//한가하면 전용 렌더링 파이프라인, 전용 셰이더를 만들어서 간단한 지시자 모양들을 만들어 볼 것.

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

protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;
	void Render(float _deltaTime) override;

protected:
	float4 color_;


};

