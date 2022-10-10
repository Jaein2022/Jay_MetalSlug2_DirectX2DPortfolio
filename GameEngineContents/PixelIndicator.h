#pragma once
#include "Indicator.h"

class PixelIndicator: public Indicator
{
	//�� Ŭ������ ���� ����: �ε������Ϳ� �ȼ��浹 ������� �߰�.

	static GameEngineTexture* pcTexture_;	//�ȼ��浹������ �ؽ�ó.
	//��� �ȼ��ε������Ͱ� �����ؾ��ϴ� �ؽ�ó�̹Ƿ� ���� ��������� �����.

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
	//�ȼ��ε��������� ���� ��ġ�� �ش��ϴ� pc�ؽ�ó�� ���� ��ȯ.
	//�ؽ�ó�� �����ϴ��� �������� �����ϴܿ� ����ִ� ���� ����.
	UINT GetColorValue_UINT();

public:
	//�̼� ���� ���� �ݵ�� ȣ���� ��.
	static void SetPCTexture(GameEngineTexture* _pixelCollisionTexture)
	{
		pcTexture_ = _pixelCollisionTexture;
		if (nullptr == pcTexture_)
		{
			MsgBoxAssert("�ؽ�ó�� �����ϴ�.");
			return;
		}
	}

	//�̼� ���� ���� �ݵ�� ȣ���� ��.
	static void SetPCTexture(const std::string& _pixelCollisionTextureName)
	{
		pcTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pcTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": �׷� �̸��� �ؽ�ó�� �����ϴ�.");
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

