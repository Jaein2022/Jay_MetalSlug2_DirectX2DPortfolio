#pragma once
#include "GameEngineRes.h"
#include "GameEngineShader.h"

class GameEngineVertexShader : public GameEngineRes<GameEngineVertexShader>, public GameEngineShader
{
	//���ؽ� ���̴�: ��ǲ���̾ƿ����� �˷��� ���Ĵ�� �Ѿ�� ���ؽ�����, �ϳ��ϳ� ���� ���ϴ� ��ġ�� �����ϴ� ��.
	//�밳�� WVP ����� �����Ų��.

	friend GameEngineRes<GameEngineVertexShader>;
	//GameEngineVertexShader Ŭ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.

	friend GameEngineShader;
	//


	friend class GameEngineInputLayout;
	//


private:
	GameEngineVertexShader();
	~GameEngineVertexShader();
	//�ܺο��� ���ڴ�� ���ҽ��� ����/�����ϴ°� ���� ���ؼ� ������/�Ҹ��ڸ� �����̺����� �����ؼ� �ܺ� ������ ���´�.
	//�� �����ӿ�ũ�� ���ҽ��� �ݵ�� �Ҹ��ڰ� �ƴ϶� ResourceDestroy()�Լ����� �����ؾ� �Ѵ�.
	//���α׷� ���������� ���ҽ������� ���ϸ� ������ �������� �׷��� �ִ��� ���Ƶд�.

	GameEngineVertexShader(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader(GameEngineVertexShader&& _other) noexcept = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader& _other) = delete;
	GameEngineVertexShader& operator=(const GameEngineVertexShader&& _other) = delete;


public:
	void Setting();	//�ش� ���ҽ��� ������ ���������ο� �����ϴ� �Լ�.

private:
	static GameEngineVertexShader* Load(
		const std::string& _path,
		const std::string& _entryPoint,
		UINT _versionHigh = 5,
		UINT _versionLow = 0
	);
	static GameEngineVertexShader* Load(
		const std::string& _path,
		const std::string& _name,
		const std::string& _entryPoint,
		UINT _versionHigh = 5,
		UINT _versionLow = 0
	);
	void CreateVertexShader();

private:
	ID3D11VertexShader* vertexShader_;

};
