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
	//매개변수로 들어온 렌더링 파이프라인의 셰이더들이 필요로 하는 셰이더리소스들을 셰이더리소스헬퍼에 등록하는 함수.
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


	//이 셰이더리소스헬퍼가 가진 모든 셰이더리소스세터들을 디바이스에 연결하는 함수.
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
	//매개변수로 들어온 셰이더의 리소스 세터들을 이 셰이더 리소스 헬퍼에 복사, 저장하고 
	// 생성된 리소스 세터에 리소스와 세팅 함수를 등록하는 함수.
	void ShaderCheck(GameEngineShader* _shader);

	
private:
	//상수버퍼 세터에 상수버퍼와 상수버퍼 세팅 함수를 등록시키는 함수.
	void BindConstantBuffer(GameEngineConstantBufferSetter& _cBufferSetter, GameEngineConstantBuffer* _cBuffer);
	
	//텍스처 세터에 텍스처와 텍스처 세팅 함수를 등록시키는 함수.
	void BindTexture(GameEngineTextureSetter& _textureSetter, GameEngineTexture* _texture);
	
	//샘플러 세터에 샘플러와 샘플러 세팅 함수를 등록시키는 함수.
	void BindSampler(GameEngineSamplerSetter& _samplerSetter, GameEngineSampler* _sampler);

private:
	std::multimap<std::string, GameEngineConstantBufferSetter> constantBufferSetterMap_;
	std::multimap<std::string, GameEngineTextureSetter> textureSetterMap_;
	std::multimap<std::string, GameEngineSamplerSetter> samplerSetterMap_;

	//셰이더 리소스 세터들의 저장, 관리를 멀티맵으로 한 이유:
	//->그냥 std::map으로는 중복되는 셰이더 리소스 세터 이름을 키값으로 쓸 수 없지만 std::multimap으로는 중복저장할 수 있다는 특성을 
	// 이용해 이름만 똑같은 다른 상수버퍼세터나 텍스처세터 등의 리소스세터들을 여러 셰이더들이 동시에 사용하게 되는 경우에도 
	// 원활하게 저장하기 위해서.
	//->같은 이름 다른 리소스세터가 가진 데이터만 다른 같은 리소스를 정점셰이더와 픽셀셰이더가 동시에 사용할 수 있게 한다.

	//셰이더 리소스 세터들을 참조형이 아닌 값형으로 저장하는 이유:
	//->셰이더마다 각각의 셰이더 리소스 세터들을 만들어서 저장해야 하는데,
	// 포인터 형태로 저장하면 각각의 셰이더 리소스 세터가 아니라, 
	// 하나의 셰이더 리소스 세터를 가리키는 각각의 '포인터 변수'를 만들어 저장하기 때문이다.
	// 그럼 어떤 셰이더가 사용하든 같은 이름의 모든 셰이더 리소스 세터들은 전부 같은 리소스만 사용하게 되므로,
	// 그걸 막기 위해 값형으로 저장해서 각각의 렌더러가 값형으로 가진 하나의 셰이더 리소스 헬퍼를 통해 
	// 같은 이름의 다른 리소스 세터로 같은 종류의 다른 리소스를 사용할 수 있게 한다.

};

