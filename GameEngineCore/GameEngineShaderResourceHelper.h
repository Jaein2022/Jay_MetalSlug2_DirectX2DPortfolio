#pragma once
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShader.h"

class GameEngineShaderResourceHelper
{
	//이 클래스의 존재 이유는 무엇?? 리소스 세팅함수들 정리?? 반드시 정리해둘 것.
	//ID3D11Resource의 자식 리소스들이 한두개가 아닌데 왜 셰이더 관련된 리소스들만 이런식으로 특별 관리하는 이유는??
	//->

	//같은 셰이더가 다른 리소스, 다른 세팅을 사용하는 경우를 대비.

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
	//리소스 준비 단계에 외부 렌더링 파이프라인의 셰이더들이 가진 셰이더리소스들을 셰이더리소스헬퍼에 등록하고, 렌더링 파이프라인에 연결하는 함수.
	void ResourceCheck(GameEngineRenderingPipeLine* _pipeLine);

	//이 셰이더 리소스 세터가 주어진 이름의 상수버퍼를 가지고 있는가를 외부에서 확인하는 함수.
	bool IsConstantBuffer(const std::string& _name);

	//이 셰이더 리소스 세터가 주어진 이름의 텍스처를 가지고 있는가를 외부에서 확인하는 함수.
	bool IsTexture(const std::string& _name);

	//이 셰이더 리소스 세터가 주어진 이름의 샘플러를 가지고 있는가를 외부에서 확인하는 함수.
	bool IsSampler(const std::string& _name);

	void SetConstantBuffer_Link(const std::string& _name, const void* _data, UINT _dataSize);	//참조로 연결.

	void SetConstantBuffer_New(const std::string& _name, const void* _data, UINT _dataSize);	//깊은 복사.
	//외부 데이터를 복사받아야 하는 등의 SetConstantBufferLink()를 사용할 수 없는 예외적인 상황에만 사용할 것.

	GameEngineTexture* SetTexture(const std::string& _textureSetterName, const std::string& _textureName);
	GameEngineTexture* SetTexture(const std::string& _textureSetterName, GameEngineTexture* _texture);
	GameEngineTexture* SetTexture(const std::string& _textureSetterName, const std::string& _folderTextureName, int _index);

	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, GameEngineSampler* _sampler);
	GameEngineSampler* SetSampler(const std::string& _samplerSetterName, const std::string& _samplerName);


	//해당 셰이더리소스헬퍼가 가진 모든 상수버퍼, 텍스처, 샘플러를 매 루프마다 갱신하고 정점셰이더나 픽셀셰이더에 연결하는 함수.
	void AllResourcesSetting();

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
	//셰이더가 가진 셰이더리소스들을 셰이더리소스헬퍼에 등록하고, 렌더링 파이프라인에 해당 셰이더에 연결하는 함수. 
	void ShaderCheck(GameEngineShader* _shader);


private:
	void BindConstantBuffer(GameEngineConstantBufferSetter& _cBufferSetter, GameEngineConstantBuffer* _cBuffer);
	void BindTexture(GameEngineTextureSetter& _textureSetter, GameEngineTexture* _texture);
	void BindSampler(GameEngineSamplerSetter& _samplerSetter, GameEngineSampler* _sampler);

private:
	std::multimap<std::string, GameEngineConstantBufferSetter> constantBufferSetterMap_;
	std::multimap<std::string, GameEngineTextureSetter> textureSetterMap_;
	std::multimap<std::string, GameEngineSamplerSetter> samplerSetterMap_;

	//셰이더 리소스 세터들의 저장, 관리를 멀티맵으로 한 이유:
	//그냥 std::map으로는 중복되는 리소스 이름을 키값으로 쓸 수 없지만 std::multimap으로는 저장할 수 있다는 특성을 이용해,
	// 이름만 똑같은 다른 상수버퍼나 텍스쳐 등의 리소스들을 여러 셰이더들이 동시에 사용하게 되는 경우에도 원활하게 저장하기 위해서.

	//셰이더 리소스 세터들을 참조형이 아닌 값형으로 저장하는 이유:
	//셰이더마다 각각의 셰이더 리소스 세터들을 만들어서 저장해야 하는데,
	// 포인터 형태로 저장하면 각각의 셰이더 리소스 세터가 아니라, 
	// 같은 셰이더 리소스 세터를 가리키는 각각의 '포인터 변수'를 만들어 저장하기 때문이다.
	// 그걸 막기 위해 값형으로 저장한다.

};

