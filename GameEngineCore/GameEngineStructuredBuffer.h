#pragma once
#include "GameEngineRes.h"


class GameEngineStructuredBuffer : public GameEngineRes<GameEngineStructuredBuffer>
{
	friend GameEngineRes<GameEngineStructuredBuffer>;
	//GameEngineStructuredBuffer Ŭ������ �����̺� �Ҹ��ڸ� GameEngineResŬ�������� ȣ���ϱ� ���� ���.

private:
	GameEngineStructuredBuffer();
	~GameEngineStructuredBuffer();
	//�ܺο��� ���ڴ�� ���ҽ��� ����/�����ϴ°� ���� ���ؼ� ������/�Ҹ��ڸ� �����̺����� �����ؼ� �ܺ� ������ ���´�.
	//�� �����ӿ�ũ�� ���ҽ��� �ݵ�� �Ҹ��ڰ� �ƴ϶� ResourceDestroy()�Լ����� �����ؾ� �Ѵ�.
	//���α׷� ���������� ���ҽ������� ���ϸ� ������ �������� �׷��� �ִ��� ���Ƶд�.

	GameEngineStructuredBuffer(const GameEngineStructuredBuffer& _other) = delete;
	GameEngineStructuredBuffer(GameEngineStructuredBuffer&& _other) noexcept = delete;
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer& _other) = delete;
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer&& _other) = delete;


public:
	static GameEngineStructuredBuffer* Create(
		const std::string& _name,
		const D3D11_SHADER_BUFFER_DESC& _desc,
		int _dataCount
	);
	static GameEngineStructuredBuffer* Find(const std::string& _name, int _byteWidth);
	static GameEngineStructuredBuffer* CreateAndFind(
		const std::string& _name,
		const D3D11_SHADER_BUFFER_DESC& _desc,
		int _dataCount
	);
	static void ResourceDestroy();
	//GameEngineRes�� ���ҽ����� �����̳ʸ� �� �� �����Ƿ� ���� �����̳ʸ� ���� ����� ���ҽ� ������ ���� �Լ��� �ٽ� ������ �Ѵ�.

protected:
	static GameEngineStructuredBuffer* CreateNamedRes(const std::string& _name, int _byteWidth);
	//GameEngineRes�� ���ҽ����� �����̳ʸ� �� �� �����Ƿ� ���� �����̳ʸ� ���� ����� ���ҽ� ������ ���� �Լ��� �ٽ� ������ �Ѵ�.


private:
	void CreateOrResize(
		const D3D11_SHADER_BUFFER_DESC& _desc,
		int _dataCount,
		void* _initializationData = nullptr
	);
	void Release();


private:
	static std::map<std::string, std::map<int, GameEngineStructuredBuffer*>> allStructuredBuffers_;
	//�Ϲ����� GameEngineRes<ResType> ���ҽ������ �ٸ��� �̸��� ũ�⸦ �� �� �����ϰ� �����غ��� �ϹǷ� 
	// ���߸� ������ ���� �����̳ʿ� �����ϰ� �����Ҷ��� ���� �����Ѵ�.

	ID3D11Buffer* structuredBuffer_;
	D3D11_BUFFER_DESC structuredBufferDesc_;
	D3D11_SHADER_BUFFER_DESC shaderBufferDesc_;
	D3D11_MAPPED_SUBRESOURCE settingResource_;

	ID3D11ShaderResourceView* shaderResourceView_;	//
	//����ȭ���۴� �������� ���̴� ���ҽ��� �з��ǹǷ� ���̴� ���ҽ� �䰡 �ʿ��ϴ�.

	int dataCount_;
};

