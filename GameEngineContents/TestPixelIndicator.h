#pragma once
#include "TestIndicator.h"

class TestPixelIndicator: public TestIndicator
{
	//�� Ŭ������ ���� ����: �ε������Ϳ� �ȼ��浹 ������� �߰�.

	static GameEngineTexture* pcTexture_;	//�ȼ��浹������ �ؽ�ó. 
	//��� �ȼ��ε������Ͱ� �����ؾ��ϴ� �ؽ�ó�̹Ƿ� ���������� �����.

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
	//�ȼ��ε��������� ���� ��ġ�� �ش��ϴ� pc�ؽ�ó�� ���� ��ȯ.
	//�ؽ�ó�� �����ϴ��� �������� �����ϴܿ� ����ִ� ���� ����.
	UINT GetColorValue_UINT();

public:
	static void SetTexture(GameEngineTexture* _pixelCollisionTexture)
	{
		pcTexture_ = _pixelCollisionTexture;
		if (nullptr == pcTexture_)
		{
			MsgBoxAssert("�ؽ�ó�� �����ϴ�.");
			return;
		}
	}
	static void SetTexture(const std::string& _pixelCollisionTextureName)
	{
		pcTexture_ = GameEngineTexture::Find(_pixelCollisionTextureName);
		if (nullptr == pcTexture_)
		{
			MsgBoxAssertString(_pixelCollisionTextureName + ": �׷� �̸��� �ؽ�ó�� �����ϴ�.");
			return;
		}
	}



private:

};

