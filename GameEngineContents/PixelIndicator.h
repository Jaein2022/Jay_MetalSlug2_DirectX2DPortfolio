#pragma once
#include "Indicator.h"

class PixelIndicator: public Indicator
{
	//이 클래스의 존재 이유: 인디케이터에 픽셀충돌 보조기능 추가.

	static GameEngineTexture* pcTexture_;	//픽셀충돌판정용 텍스처.
	//모든 픽셀인디케이터가 공유해야하는 텍스처이므로 정적 멤버변수로 만든다.

public:
	PixelIndicator();
	~PixelIndicator();

protected:
	PixelIndicator(const PixelIndicator& _other) = delete;
	PixelIndicator(PixelIndicator&& _other) noexcept = delete;

private:
	PixelIndicator& operator=(const PixelIndicator& _other) = delete;
	PixelIndicator& operator=(const PixelIndicator&& _other) = delete;


public:
	//픽셀인디케이터의 현재 위치에 해당하는 pc텍스처의 색상값 반환.
	//텍스처의 좌측하단이 윈도우의 좌측하단에 닿아있는 상태 기준.
	UINT GetColorValue_UINT();

public:
	//미션 시작 전에 반드시 호출할 것.
	static void SetPCTexture(GameEngineTexture* _pixelCollisionTexture)
	{
		pcTexture_ = _pixelCollisionTexture;
		if (nullptr == pcTexture_)
		{
			MsgBoxAssert("텍스처가 없습니다.");
			return;
		}
	}

	//미션 시작 전에 반드시 호출할 것.
	static void SetPCTexture(const std::string& _pixelCollisionTextureName)
	{
		pcTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pcTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": 그런 이름의 텍스처가 없습니다.");
			return;
		}
	}

	static GameEngineTexture* GetPCTexture()
	{
		return pcTexture_;
	}

	inline bool IsOnSteppablePixel(PixelColor _currentSteppingColor = steppablePixelColor_)
	{
		return _currentSteppingColor.color_ <= this->GetColorValue_UINT();
	}

	inline bool IsOnGroundPixel()	
	{
		return groundColor_.color_ <= this->GetColorValue_UINT();
	}


private:

};

