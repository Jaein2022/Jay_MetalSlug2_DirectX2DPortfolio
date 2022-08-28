#pragma once
#include "TestIndicator.h"

class TestPixelIndicator: public TestIndicator
{
	//이 클래스의 존재 이유: 인디케이터에 픽셀충돌 보조기능 추가.

	static GameEngineTexture* pcTexture_;	//픽셀충돌판정용 텍스처. 
	//모든 픽셀인디케이터가 공유해야하는 텍스처이므로 정적변수로 만든다.

public:
	TestPixelIndicator();
	~TestPixelIndicator();

protected:
	TestPixelIndicator(const TestPixelIndicator& _other) = delete;
	TestPixelIndicator(TestPixelIndicator&& _other) noexcept = delete;

private:
	TestPixelIndicator& operator=(const TestPixelIndicator& _other) = delete;
	TestPixelIndicator& operator=(const TestPixelIndicator&& _other) = delete;


public:
	//픽셀인디케이터의 현재 위치에 해당하는 pc텍스처의 색상값 반환.
	//텍스처의 좌측하단이 윈도우의 좌측하단에 닿아있는 상태 기준.
	UINT GetColorValue_UINT();

public:
	static void SetTexture(GameEngineTexture* _pixelCollisionTexture)
	{
		pcTexture_ = _pixelCollisionTexture;
		if (nullptr == pcTexture_)
		{
			MsgBoxAssert("텍스처가 없습니다.");
			return;
		}
	}
	static void SetTexture(const std::string& _pixelCollisionTextureName)
	{
		pcTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pcTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": 그런 이름의 텍스처가 없습니다.");
			return;
		}
	}



private:

};

