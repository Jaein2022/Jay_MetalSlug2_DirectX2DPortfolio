#pragma once
#include "GameEngineCore.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"


class GEngine
{
	//�л�� �������̽� ���տ� Ŭ����.
	//���ӿ����ھ��� Ŭ�������� �� Ŭ������ ����ؼ� ���� �ȵȴ�. 
	//������ �����ڸ��� ���� �������̽� ������ Ŭ����.


private:

	GEngine();
	virtual ~GEngine() = 0;

	GEngine(const GEngine& _other) = delete;
	GEngine(GEngine&& _other) noexcept = delete;
	GEngine& operator=(const GEngine& _other) = delete;
	GEngine& operator=(GEngine&& _other) = delete;

public:
	static void ChangeLevel(const std::string& _levelName);

};

