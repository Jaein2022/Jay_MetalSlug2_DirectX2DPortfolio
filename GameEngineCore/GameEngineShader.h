#pragma once

enum class ShaderType
{
	VertexShader,
	PixelShader,
	Max
};

class GameEngineShader;
class ShaderResSetter : public GameEngineNameObject
{
	//각각의 셰이더들끼리 상수버퍼나 다른 셰이더 리소스들은 공유하더라도, 바인드포인트 등의
	// 셰이더 리소스 스스로가 알 수 없는 정보가 다른 경우에 대응하기 위해 만든, 셰이더리소스 외부정보 전담관리용 클래스.

public:
	GameEngineShader* parentShader_;	//이 리소스를 사용할 부모 셰이더.
	int bindPoint_;		//해당 셰이더리소스의 바인드포인트(레지스터 등록 번호).
	ShaderType parentShaderType_;	//이 리소스가 연결될 부모 셰이더의 종류.
	std::function<void()> settingFunction_;	//셰이더가 가진 리소스들을 세팅하는 함수.
	std::function<void()> resetFunction_;	//셰이더가 가진 리소스들을 리셋하는 함수.

protected:
	ShaderResSetter()
		: parentShader_(nullptr),
		bindPoint_(-1),
		parentShaderType_(ShaderType::Max),
		settingFunction_(nullptr),
		resetFunction_(nullptr)
	{
	}

	~ShaderResSetter() {}
};

class GameEngineConstantBuffer;
class GameEngineConstantBufferSetter : public ShaderResSetter
{
	//셰이더들이 필요로 하는 각각의 상수버퍼가 가져야 하는 정보 관리 클래스.
	//렌더러가 n개->렌더링파이프라인과 트랜스폼이 각각 n개 필요. ->각각의 트랜스폼 데이터를 GPU로 전달할 상수버퍼도 n개 필요.
	// ->n개의 렌더링 파이프라인에 딸린 2(+@)*n개의 셰이더들이 2(+@)*n개의 상수버퍼를 필요로 한다.
	// 그 2(+@)*n개의 상수버퍼들을 통해 셰이더로 전달해야 하는 정보, 상수버퍼들의 목적지 셰이더, 
	// 상수버퍼들을 관리하는데 필요한 이름과 그 크기 등을 저장한다.


	friend class GameEngineShader;

public:
	GameEngineConstantBuffer* constantBuffer_;
	//

	//Map()함수를 통해 GPU로 보내질, 각각의 상수버퍼들이 가진 데이터의 주소값. 
	const void* settingDataToGPU_;

	UINT byteWidth_;	//상수버퍼 전체 크기.

	std::vector<char> originalData_;
	// 아예 자기 메모리로 만든다??

	void Setting() const;

	//private:	<-일일히 다 막기엔 일이 너무 복잡해져서 막지는 않지만 외부에서 사용해선 안된다.
	// 여기서 외부는 이 리소스를 직접 사용할 GameEngineShader의 자식 클래스들과 셰이더를 사용할 GameEngineRenderingPipeLine,
	// 렌더링 파이프라인을 사용할 GameEngineDefaultRenderer클래스와 그 자식클래스들을 제외하고는 다 외부이다.

	GameEngineConstantBufferSetter()
		: constantBuffer_(nullptr),
		settingDataToGPU_(nullptr),
		byteWidth_(-1)
	{
	}

};

class GameEngineTexture;
class GameEngineTextureSetter : public ShaderResSetter
{
	//이 클래스의 목적은??

	friend class GameEngineShader;

public:
	GameEngineTexture* texture_;

	void Setting() const;
	void Reset() const;

	//private:	<-일일히 다 막기엔 일이 너무 복잡해져서 막지는 않지만 외부에서 사용해선 안된다.
	// 여기서 외부는 이 리소스를 직접 사용할 GameEngineShader의 자식 클래스들과 셰이더를 사용할 GameEngineRenderingPipeLine,
	// 렌더링 파이프라인을 사용할 GameEngineDefaultRenderer클래스와 그 자식클래스들을 제외하고는 다 외부이다.

	GameEngineTextureSetter() : texture_(nullptr)
	{
	}
};

class GameEngineSampler;
class GameEngineSamplerSetter : public ShaderResSetter
{



public:
	GameEngineSampler* sampler_;

	void Setting() const;

	//private:	<-일일히 다 막기엔 일이 너무 복잡해져서 막지는 않지만 외부에서 사용해선 안된다.
	// 여기서 외부는 이 리소스를 직접 사용할 GameEngineShader의 자식 클래스들과 셰이더를 사용할 GameEngineRenderingPipeLine,
	// 렌더링 파이프라인을 사용할 GameEngineDefaultRenderer클래스와 그 자식클래스들을 제외하고는 다 외부이다.

	GameEngineSamplerSetter() : sampler_(nullptr)
	{
	}
};

class GameEngineStructuredBuffer;
class GameEngineStructuredBufferSetter : public ShaderResSetter
{
public:
	void Setting() const;

	GameEngineStructuredBuffer* structuredBuffer_;

	GameEngineStructuredBufferSetter() : structuredBuffer_(nullptr)
	{
	}
};


class GameEngineShader
{
	//이 프레임워크 내 모든 셰이더와 셰이더에서 사용하는 리소스들을 관리하는 클래스.
	friend class GameEngineShaderResourceHelper;

public:
	GameEngineShader();
	virtual ~GameEngineShader();

protected:
	GameEngineShader(const GameEngineShader& _other) = delete;
	GameEngineShader(GameEngineShader&& _other) noexcept = delete;

private:
	GameEngineShader& operator=(const GameEngineShader& _other) = delete;
	GameEngineShader& operator=(const GameEngineShader&& _other) = delete;


public:
	static void AutoCompile(const std::string& _path);

public:
	GameEngineConstantBufferSetter& GetConstantBufferSetter(const std::string& _name);


protected:
	void CreateVersion(const std::string& _shaderType, UINT _versionHigh, UINT _versionLow);
	void CompileHLSLCode(const std::string& _path);	//ShaderCompile()함수를 대체하는, 셰이더 클래스 공용 HLSL코드 컴파일 함수.
	void ShaderResCheck();	//내가 입력한 셰이더코드와 실제 셰이더코드가 같은지 확인하는 함수.
	//없어도 작동은 하지만 조금이라도 문제가 생겼을 때 잡아낼 수가 없으므로 이 함수로 문제 여지를 미리 차단한다.

protected:
	std::string entryPoint_;	//HLSL 코드의 진입점함수 이름.
	std::string shaderVersion_;	//HLSL 코드의 사용 목적과 컴파일러의 버전.
	ID3DBlob* binaryCode_;	//HLSL 코드를 컴파일한 결과물(바이트코드).
	ShaderType shaderType_;		//이 셰이더의 종류.

	std::map<std::string, GameEngineConstantBufferSetter> constantBufferMap_;
	std::map<std::string, GameEngineTextureSetter> textureMap_;
	std::map<std::string, GameEngineSamplerSetter> samplerMap_;
	std::map<std::string, GameEngineStructuredBufferSetter> structuredBufferMap_;

	//셰이더리소스세터들을 값형으로 저장한 이유는??
	//->셰이더 리소스 종류가 다양하지 않아서 값형으로 보관해도 많은 컨테이너들을 만들 필요가 없고, 
	// 각각의 리소스들을 분리해서 보다 세밀하게 관리할 수 있다는 장점을 이용할 수 있기 때문.

protected:
	inline void SetEntrtyPoint(const std::string& _entryPoint)
	{
		entryPoint_ = _entryPoint;
	}


};

