#pragma once
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"

class GameEngineShaderResourceHelper
{
	//�� Ŭ������ ���� ������ ����?? ���ҽ� �����Լ��� ����?? �ݵ�� �����ص� ��.
	//ID3D11Resource�� �ڽ� ���ҽ����� �ѵΰ��� �ƴѵ� �� ���̴�  ���õ� ���ҽ��鸸 �̷������� Ư�� �����ϴ� ������??
	// 
	//���� ���̴��� �ٸ� ���ҽ�, �ٸ� ������ ����ϴ� ��츦 ���.

	friend GameEngineShader;

	friend class GameEngineDefaultRenderer;


private:
	GameEngineShaderResourceHelper();
	~GameEngineShaderResourceHelper();

protected:
	GameEngineShaderResourceHelper(const GameEngineShaderResourceHelper& _other) = delete;
	GameEngineShaderResourceHelper(GameEngineShaderResourceHelper&& _other) noexcept = delete;

private:
	GameEngineShaderResourceHelper& operator=(const GameEngineShaderResourceHelper& _other) = delete;
	GameEngineShaderResourceHelper& operator=(const GameEngineShaderResourceHelper&& _other) = delete;


public:
	void ResourceCheck(GameEngineRenderingPipeLine* _pipeLine);

	bool IsConstantBuffer(const std::string& _name);
	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� ������۸� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.

	bool IsTexture(const std::string& _name);
	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� �ؽ�ó�� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.

	bool IsSampler(const std::string& _name);
	//�� ���̴� ���ҽ� ���Ͱ� �־��� �̸��� ���÷��� ������ �ִ°��� �ܺο��� Ȯ���ϴ� �Լ�.

	void SetConstantBufferLink(const std::string& _name, const void* _data, UINT _size);	//������ ����??
	void SetConstantBufferNew(const std::string& _name, const void* _data, UINT _size);	//���� ����??
	//�ܺ� ����Ÿ�� ����޾ƾ� �ϴ� ���� SetConstantBufferLink()�� ����� �� ���� �������� ��Ȳ���� ����� ��.

	GameEngineTexture* SetTexture(const std::string& _textureSetterName, const std::string& _textureName);
	GameEngineTexture* SetTexture(const std::string& _textureSetterName, GameEngineTexture* _texture);

	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, GameEngineSampler* _sampler);
	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, const std::string& _samplerName);


	void AllResourcesSetting();




public:
	template<typename Resource>
	void SetConstantBufferLink(const std::string& _name, const Resource& _data)
	{
		SetConstantBufferLink(_name, &_data, static_cast<UINT>(sizeof(Resource)));
	}

	template<typename Resource>
	void SetConstantBufferNew(const std::string& _name, const Resource& _data)
	{
		SetConstantBufferNew(_name, &_data, static_cast<UINT>(sizeof(Resource)));
	}


protected:
	void ShaderCheck(GameEngineShader* _shader);


private:
	void BindConstantBuffer(GameEngineConstantBufferSetter& _cBufferSetter, GameEngineConstantBuffer* _cBuffer);
	void BindTexture(GameEngineTextureSetter& _textureSetter, GameEngineTexture* _texture);
	void BindSampler(GameEngineSamplerSetter& _samplerSetter, GameEngineSampler* _sampler);

private:
	std::multimap<std::string, GameEngineConstantBufferSetter> constantBufferSetterMap_;
	std::multimap<std::string, GameEngineTextureSetter> textureSetterMap_;
	std::multimap<std::string, GameEngineSamplerSetter> samplerSetterMap_;

	//���̴� ���ҽ� ���͵��� ����, ������ ��Ƽ������ �� ����:
	//�׳� std::map���δ� �ߺ��Ǵ� ���ҽ� �̸��� Ű������ �� �� ������ std::multimap���δ� ������ �� �ִٴ� Ư���� �̿���,
	// �̸��� �Ȱ��� �ٸ� ������۳� �ؽ��� ���� ���ҽ����� ���� ���̴����� ���ÿ� ����ϰ� �Ǵ� ��쿡�� ��Ȱ�ϰ� �����ϱ� ���ؼ�.

	//���̴� ���ҽ� ���͵��� �������� �ƴ� �������� �����ϴ� ����:
	//���̴����� ������ ���̴� ���ҽ� ���͵��� ���� �����ؾ� �ϴµ�,
	// ������ ���·� �����ϸ� ������ ���̴� ���ҽ� ���Ͱ� �ƴ϶�, 
	// ���� ���̴� ���ҽ� ���͸� ����Ű�� ������ '������ ����'�� ����� �����ϱ� �����̴�.
	// �װ� ���� ���� �������� �����Ѵ�.

};

