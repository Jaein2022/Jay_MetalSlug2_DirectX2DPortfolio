#pragma once
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"

class GameEngineShaderResourceHelper
{
	//�� Ŭ������ ���� ������ ����?? ���ҽ� �����Լ��� ����?? �ݵ�� �����ص� ��.
	//ID3D11Resource�� �ڽ� ���ҽ����� �ѵΰ��� �ƴѵ� �� ���̴� ���õ� ���ҽ��鸸 �̷������� Ư�� �����ϴ� ������??
	//->

	//���� ���̴��� �ٸ� ���ҽ�, �ٸ� ������ ����ϴ� ��츦 ���.

	friend GameEngineShader;

	friend class GameEngineDefaultRenderer;


public:
	GameEngineShaderResourceHelper();
	~GameEngineShaderResourceHelper();

protected:
	GameEngineShaderResourceHelper(const GameEngineShaderResourceHelper& _other) = delete;
	GameEngineShaderResourceHelper(GameEngineShaderResourceHelper&& _other) noexcept = delete;

private:
	GameEngineShaderResourceHelper& operator=(const GameEngineShaderResourceHelper& _other) = delete;
	GameEngineShaderResourceHelper& operator=(const GameEngineShaderResourceHelper&& _other) = delete;


public:
	//�Ű������� ���� ������ ������������ ���̴����� �ʿ�� �ϴ� ���̴����ҽ����� ���̴����ҽ����ۿ� ����ϴ� �Լ�.
	void ResourceCheck(GameEngineRenderingPipeLine* _pipeLine);

	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� ������۸� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.
	bool IsConstantBuffer(const std::string& _name);

	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� �ؽ�ó�� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.
	bool IsTexture(const std::string& _name);

	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� ���÷��� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.
	bool IsSampler(const std::string& _name);

	void SetConstantBuffer_Link(const std::string& _name, const void* _data, UINT _dataSize);	//������ ����.

	void SetConstantBuffer_New(const std::string& _name, const void* _data, UINT _dataSize);	//���� ����.
	//�ܺ� �����͸� ����޾ƾ� �ϴ� ���� SetConstantBufferLink()�� ����� �� ���� �������� ��Ȳ���� ����� ��.

	GameEngineTexture* SetTexture(const std::string& _textureSetterName, const std::string& _textureName);
	GameEngineTexture* SetTexture(const std::string& _textureSetterName, GameEngineTexture* _texture);
	GameEngineTexture* SetTexture(const std::string& _textureSetterName, const std::string& _folderTextureName, int _index);

	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, GameEngineSampler* _sampler);
	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, const std::string& _samplerName);


	//�� ���̴����ҽ����۰� ���� ��� ���̴����ҽ����͵��� ����̽��� �����ϴ� �Լ�.
	void AllResourcesSetting();
	//

	void AllResourcesReset();

public:
	template<typename Resource>
	void SetConstantBuffer_Link(const std::string& _name, const Resource& _data)
	{
		SetConstantBuffer_Link(_name, &_data, static_cast<UINT>(sizeof(Resource)));
	}

	template<typename Resource>
	void SetConstantBuffer_New(const std::string& _name, const Resource& _data)
	{
		SetConstantBuffer_New(_name, &_data, static_cast<UINT>(sizeof(Resource)));
	}


protected:
	//�Ű������� ���� ���̴��� ���ҽ� ���͵��� �� ���̴� ���ҽ� ���ۿ� ����, �����ϰ� 
	// ������ ���ҽ� ���Ϳ� ���ҽ��� ���� �Լ��� ����ϴ� �Լ�.
	void ShaderCheck(GameEngineShader* _shader);

	
private:
	//������� ���Ϳ� ������ۿ� ������� ���� �Լ��� ��Ͻ�Ű�� �Լ�.
	void BindConstantBuffer(GameEngineConstantBufferSetter& _cBufferSetter, GameEngineConstantBuffer* _cBuffer);
	
	//�ؽ�ó ���Ϳ� �ؽ�ó�� �ؽ�ó ���� �Լ��� ��Ͻ�Ű�� �Լ�.
	void BindTexture(GameEngineTextureSetter& _textureSetter, GameEngineTexture* _texture);
	
	//���÷� ���Ϳ� ���÷��� ���÷� ���� �Լ��� ��Ͻ�Ű�� �Լ�.
	void BindSampler(GameEngineSamplerSetter& _samplerSetter, GameEngineSampler* _sampler);

private:
	std::multimap<std::string, GameEngineConstantBufferSetter> constantBufferSetterMap_;
	std::multimap<std::string, GameEngineTextureSetter> textureSetterMap_;
	std::multimap<std::string, GameEngineSamplerSetter> samplerSetterMap_;

	//���̴� ���ҽ� ���͵��� ����, ������ ��Ƽ������ �� ����:
	//->�׳� std::map���δ� �ߺ��Ǵ� ���̴� ���ҽ� ���� �̸��� Ű������ �� �� ������ std::multimap���δ� �ߺ������� �� �ִٴ� Ư���� 
	// �̿��� �̸��� �Ȱ��� �ٸ� ������ۼ��ͳ� �ؽ�ó���� ���� ���ҽ����͵��� ���� ���̴����� ���ÿ� ����ϰ� �Ǵ� ��쿡�� 
	// ��Ȱ�ϰ� �����ϱ� ���ؼ�.
	//->���� �̸� �ٸ� ���ҽ����Ͱ� ���� �����͸� �ٸ� ���� ���ҽ��� �������̴��� �ȼ����̴��� ���ÿ� ����� �� �ְ� �Ѵ�.

	//���̴� ���ҽ� ���͵��� �������� �ƴ� �������� �����ϴ� ����:
	//->���̴����� ������ ���̴� ���ҽ� ���͵��� ���� �����ؾ� �ϴµ�,
	// ������ ���·� �����ϸ� ������ ���̴� ���ҽ� ���Ͱ� �ƴ϶�, 
	// �ϳ��� ���̴� ���ҽ� ���͸� ����Ű�� ������ '������ ����'�� ����� �����ϱ� �����̴�.
	// �׷� � ���̴��� ����ϵ� ���� �̸��� ��� ���̴� ���ҽ� ���͵��� ���� ���� ���ҽ��� ����ϰ� �ǹǷ�,
	// �װ� ���� ���� �������� �����ؼ� ������ �������� �������� ���� �ϳ��� ���̴� ���ҽ� ���۸� ���� 
	// ���� �̸��� �ٸ� ���ҽ� ���ͷ� ���� ������ �ٸ� ���ҽ��� ����� �� �ְ� �Ѵ�.

};

