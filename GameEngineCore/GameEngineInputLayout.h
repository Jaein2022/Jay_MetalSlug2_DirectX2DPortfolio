#pragma once
#include "GameEngineRes.h"

class GameEngineInputLayoutInfo;
class GameEngineVertexShader;
class GameEngineInputLayout : public GameEngineRes<GameEngineInputLayout>
{
	//��ǲ���̾ƿ�: 
	//��ǲ ����� �ܰ迡�� �׷���ī�� �޸𸮿� ����� �޽��� ��� ��������(���ؽ� ����)�� � �������� �����Ǿ��ִ����� 
	// ���� ������ ���ؽ����̴����� �ѱ�� ���� �������̽�.

	friend GameEngineRes<GameEngineInputLayout>;
	friend class GameEngineRenderingPipeLine;
	//GameEngineInputLayout Ŭ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.
	//�׷��� ���� CreateNameRes()�Լ��� �����ϴ°��� �ƴ϶� ���ӿ��������������������� new�����ڷ� ���� �����ϹǷ�
	// �ǹ̰� ���������� �׷��� �ϴ� ������ �Ѵ�.


private:
	GameEngineInputLayout();
	~GameEngineInputLayout();

	GameEngineInputLayout(const GameEngineInputLayout& _other) = delete;
	GameEngineInputLayout(GameEngineInputLayout&& _other) noexcept = delete;
	GameEngineInputLayout& operator=(const GameEngineInputLayout& _other) = delete;
	GameEngineInputLayout& operator=(const GameEngineInputLayout&& _other) = delete;


public:
	void CreateInputLayout(
		const GameEngineInputLayoutInfo& _info,
		GameEngineVertexShader* _vertexShader
	);

	void Setting();	//�ش� ���ҽ��� ������ ���������ο� �����ϴ� �Լ�.


private:
	ID3D11InputLayout* inputLayout_;

};