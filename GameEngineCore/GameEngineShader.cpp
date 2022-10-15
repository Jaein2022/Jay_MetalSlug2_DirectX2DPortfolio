#include "PreCompile.h"
#include "GameEngineShader.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineStructuredBuffer.h"

void GameEngineConstantBufferSetter::Setting() const
{
	constantBuffer_->ChangeData(settingDataToGPU_, byteWidth_);

	settingFunction_();	//스위치문 한번 덜 쓰려고 펑셔널 사용.
}

void GameEngineTextureSetter::Setting() const
{
	settingFunction_();	//스위치문 한번 덜 쓰려고 펑셔널 사용.
}

void GameEngineTextureSetter::Reset() const
{
	resetFunction_();	//스위치문 한번 덜 쓰려고 펑셔널 사용.
}

void GameEngineSamplerSetter::Setting() const
{
	settingFunction_();	//스위치문 한번 덜 쓰려고 펑셔널 사용.
}

//void GameEngineStructuredBufferSetter::Setting() const
//{
//	settingFunction_();	//스위치문 한번 덜 쓰려고 펑셔널 사용.
//}

GameEngineShader::GameEngineShader()
	: shaderVersion_(""),
	entryPoint_(""),
	binaryCode_(nullptr),
	shaderType_(ShaderType::Max)
{
}

GameEngineShader::~GameEngineShader()
{
	if (nullptr != binaryCode_)
	{
		binaryCode_->Release();
		binaryCode_ = nullptr;
	}
}

void GameEngineShader::AutoCompile(const std::string& _path)
{
	GameEngineFile file = GameEngineFile(_path);
	file.Open(OpenMode::Read, FileMode::Text);

	std::string allHLSLCode = file.GetString();

	//대소문자 주의!
	size_t vsEntryIndex = allHLSLCode.find("_VS(");
	if (std::string::npos != vsEntryIndex)
	{
		size_t firstIndex = allHLSLCode.find_last_of(" ", vsEntryIndex);
		std::string vsEntryName = allHLSLCode.substr(	//주어진 문자열의 일부를 복사해서 반환하는 클래스.
			firstIndex + 1,									//복사를 시작할 글자 인덱스.
			vsEntryIndex - firstIndex - 1					//복사 할 글자 수.
		);
		vsEntryName += "_VS";
		GameEngineVertexShader::Load(_path, vsEntryName);
	}
	else
	{
		MsgBoxAssert("코드 내에 버텍스셰이더가 없습니다.");
		//셰이더 이름 규칙0: 버텍스셰이더는 “_VS(”, 픽셀셰이더는 “_PS(“가 문자열 내부에 포함되어 있어야 한다.
		return;
	}


	size_t psEntryIndex = allHLSLCode.find("_PS(");
	if (std::string::npos != psEntryIndex)
	{
		size_t firstIndex = allHLSLCode.find_last_of(" ", psEntryIndex);
		std::string psEntryName = allHLSLCode.substr(
			firstIndex + 1,
			psEntryIndex - firstIndex - 1
		);
		psEntryName += "_PS";

		GameEnginePixelShader::Load(_path, psEntryName);
	}
	else
	{
		MsgBoxAssert("코드 내에 픽셀셰이더가 없습니다.");
		//셰이더 이름 규칙 0: 버텍스셰이더는 “_VS(”, 픽셀셰이더는 “_PS(“가 문자열 내부에 포함되어 있어야 한다.
		return;
	}
}

GameEngineConstantBufferSetter& GameEngineShader::GetConstantBufferSetter(const std::string& _name)
{
	std::string uppercaseSetterName = GameEngineString::ToUpperReturn(_name);

	if (constantBufferSetterMap_.end() == constantBufferSetterMap_.find(uppercaseSetterName))
	{
		MsgBoxAssertString(_name + ": 그런 이름의 상수버퍼가 존재하지 않습니다.");
	}

	return constantBufferSetterMap_[uppercaseSetterName];
}

void GameEngineShader::CreateVersion(const std::string& _shaderType, UINT _versionHigh, UINT _versionLow)
{
	shaderVersion_ = "";
	shaderVersion_ += _shaderType + "_";
	shaderVersion_ += std::to_string(_versionHigh);
	shaderVersion_ += "_";
	shaderVersion_ += std::to_string(_versionLow);
}

void GameEngineShader::CompileHLSLCode(const std::string& _path)
{
	unsigned int compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3D10_SHADER_DEBUG;	//디버그 파일/줄/유형/기호 정보를 삽입한다.
#endif // _DEBUG
	compileFlag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;  //행 중심 매트릭스 사용(안하면 전치행렬 사용).

	ID3DBlob* errorMessage = { 0 };		//컴파일 실패시 에러코드를 받는 변수.

	//와이드스트링 경로.
	std::wstring unicodePath = GameEngineString::AnsiToUnicodeReturn(_path);


	if (S_OK != D3DCompileFromFile( //hlsl 컴파일 함수.
		unicodePath.c_str(),          //경로가 포함된 HLSL 파일 이름. C스타일 와이드스트링만 받는다.
		nullptr,							//입력할 코드에서 사용할 매크로가 들어있는 D3D_SHADER_MACRO구조체의 배열 주소값.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  //입력할 코드에서 사용할 전처리기가 들어있는 ID3DInclude구조체의 배열 주소값.
		//D3D_COMPILE_STANDARD_FILE_INCLUDE 매크로가 기본 인클루드 핸들러를 가리키고 있으니,
		//hlsl코드에 #include가 포함되어 있으면 이걸 넣으면 된다고 한다.
		entryPoint_.c_str(),    //셰이더 진입점함수 이름.
		shaderVersion_.c_str(), //사용할 HLSL 컴파일러의 목적과 버전. vs_5_0이라면 HLSL컴파일러 5.0으로 컴파일한 결과물을 버텍스 셰이더로 쓰겠다는 뜻.
		compileFlag,			//컴파일 옵션 플래그 1.
		0,						//컴파일 옵션 플래그 2. 셰이더 컴파일때는 무시됨.
		&binaryCode_,			//컴파일된 바이너리 코드를 받을 ID3DBlob 인터페이스 주소값.
		&errorMessage			//에러메세지를 받을 ID3DBlob 인터페이스 주소값.
	))
	{
		std::string errorText = reinterpret_cast<char*>(errorMessage->GetBufferPointer());
		MsgBoxAssertString(entryPoint_ + ": HLSL코드 컴파일 실패.");
		MsgBoxAssertString(errorText);
		errorMessage->Release();
		return;
	}

	//errorMessage->Release();
	//컴파일에 문제가 없었다면 errorMessage는 널포인터인상태 그대로 내려오므로 릴리즈할 것이 없다.
}

void GameEngineShader::ShaderResCheck()
{
	if (nullptr == binaryCode_)
	{
		MsgBoxAssert("바이너리코드가 없습니다.");
		return;
	}

	//리플렉션: 원래의 구체적인 클래스나 자료형에 대해 몰라도 바이트코드로 되어있는 컴파일 결과물에서 
	// 정보를 추출해내는 프로그램 기법이다. 그러니까 컴파일된 바이너리코드에서 파싱하는것과 마찬가지.

	ID3D11ShaderReflection* compileInfo = { 0 };

	if (S_OK != D3DReflect(				//컴파일된 HLSL코드를 바탕으로 내가 셰이더에서 사용한 변수, 함수, 인자들에 
										// 대한 정보를 추출해서 셰이더 리플렉션이라는 인터페이스를 통해 반환하는 함수.
		binaryCode_->GetBufferPointer(),		//컴파일된 HLSL 코드의 포인터.
		binaryCode_->GetBufferSize(),			//컴파일된 HLSL 코드의 크기.
		IID_ID3D11ShaderReflection,				//반환할때 참조할 ID3D11ShaderReflection의 인터페이스 식별자(GUID).
		reinterpret_cast<void**>(&compileInfo)	//반환받을 ID3D11ShaderReflection의 포인터.
	))
	{
		MsgBoxAssert("셰이더 정보 추출 실패.");
		return;
	}



	D3D11_SHADER_DESC shaderInfo = { 0 };
	compileInfo->GetDesc(&shaderInfo);


	//typedef struct _D3D11_SHADER_INPUT_BIND_DESC
	//{
	//	LPCSTR                      Name;			리소스 이름.
	//	D3D_SHADER_INPUT_TYPE       Type;			리소스 종류(텍스쳐, 상수버퍼 등등...)
	//	UINT                        BindPoint;		해당 리소스의 레지스터 등록 번호.
	//	UINT                        BindCount;		연결할 리소스 개수.
	//	UINT                        uFlags;			Input binding flags
	//	D3D_RESOURCE_RETURN_TYPE    ReturnType;		반환형. 3차원 텍스처에만 사용.
	//	D3D_SRV_DIMENSION           Dimension;		Dimension (if texture) 3차원 텍스처에만 사용.
	//	UINT                        NumSamples;		Number of samples (0 if not MS texture) 텍스쳐가 아니라면 0.
	//} D3D11_SHADER_INPUT_BIND_DESC;
	D3D11_SHADER_INPUT_BIND_DESC resInfo = { 0 };

	for (UINT i = 0; i < shaderInfo.BoundResources; i++)
		//shaderInfo.BoundResources: 이 셰이더에서 '사용된' 총 리소스 양. 사용되지 않은 리소스는 무시된다.
	{
		compileInfo->GetResourceBindingDesc(i, &resInfo);
		std::string uppercaseResourceName = GameEngineString::ToUpperReturn(resInfo.Name);

		switch (resInfo.Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* cBufferPtr = compileInfo->GetConstantBufferByName(resInfo.Name);


			//typedef struct _D3D11_SHADER_BUFFER_DESC
			//{
			//	LPCSTR                  Name;		상수버퍼 이름.
			//	D3D_CBUFFER_TYPE        Type;		상수버퍼의 종류.
			//	UINT                    Variables;	상수버퍼의 멤버변수 개수.
			//	UINT                    Size;		상수버퍼 크기.
			//	UINT                    uFlags;		상수버퍼가 연결될 슬롯을 지정하는 플래그. 
			//		hlsl코드에 지정된 슬롯으로 연결한다는 플래그 하나밖에 없으므로 의미 없음.
			//} D3D11_SHADER_BUFFER_DESC;
			D3D11_SHADER_BUFFER_DESC cBufferDesc = { 0 };
			cBufferPtr->GetDesc(&cBufferDesc);


			GameEngineConstantBufferSetter newCBufferSetter;
			newCBufferSetter.parentShader_ = this;
			newCBufferSetter.SetName(uppercaseResourceName);
			newCBufferSetter.parentShaderType_ = this->shaderType_;
			newCBufferSetter.constantBuffer_ = GameEngineConstantBuffer::CreateAndFind(
				uppercaseResourceName,	//만들려는 상수버퍼가 없으면 만들고, 이미 있으면 공유한다.
				cBufferDesc		//같은 이름, 같은 크기의 상수 버퍼는 셰이더리소스헬퍼들이 포인터를 공유한다.
			);			// 그래서 이미 만들어져 있는걸 또 만들어도 터뜨리지 않고 대신 이미 만들어져 있는걸 공유한다.

			newCBufferSetter.bindPoint_ = resInfo.BindPoint;

			std::pair<std::map<std::string, GameEngineConstantBufferSetter>::iterator, bool> insertResult =
				constantBufferSetterMap_.insert(std::make_pair(uppercaseResourceName, newCBufferSetter));
			//맵에 겹치는 키값을 가진 원소를 삽입하려고 하면 중복된 키값을 가진 원소를 가리키는 
			//이터레이터와 false가 든 페어를 반환하고 삽입 시도는 무시된다.
			//삽입이 성공했다면 삽입한 원소를 가리키는 이터레이터와 true를 가진 페어를 반환한다.


			if (false == insertResult.second)
			{
				MsgBoxAssertString(std::string(resInfo.Name) + ": 이미 같은 이름의 상수버퍼 세터가 존재합니다.");
				//상수버퍼 세터를 중복으로 만드는일이 생겨선 안된다.
				return;
			}

			break;
		}

		case D3D_SIT_TEXTURE:
		{
			GameEngineTextureSetter newTextureSetter;
			newTextureSetter.parentShader_ = this;
			newTextureSetter.SetName(uppercaseResourceName);
			newTextureSetter.parentShaderType_ = this->shaderType_;
			newTextureSetter.texture_ = GameEngineTexture::Find("NSet.png");//<-텍스쳐가 아예 없다는 경고가 뜨면 여기로.
			newTextureSetter.bindPoint_ = resInfo.BindPoint;

			std::pair<std::map<std::string, GameEngineTextureSetter>::iterator, bool> insertResult
				= textureSetterMap_.insert(std::make_pair(uppercaseResourceName, newTextureSetter));
			//맵에 겹치는 키값을 가진 원소를 삽입하려고 하면 중복된 키값을 가진 원소를 가리키는 
			//이터레이터와 false가 든 페어를 반환하고 삽입 시도는 무시된다.
			//삽입이 성공했다면 삽입한 원소를 가리키는 이터레이터와 true를 가진 페어를 반환한다.

			if (false == insertResult.second)
			{
				MsgBoxAssertString(std::string(resInfo.Name) + ":  이미 같은 이름의 텍스처 세터가 존재합니다.");
				return;
			}

			break;
		}

		case D3D_SIT_SAMPLER:
		{
			GameEngineSamplerSetter newSamplerSetter;
			newSamplerSetter.parentShader_ = this;
			newSamplerSetter.SetName(uppercaseResourceName);
			newSamplerSetter.parentShaderType_ = this->shaderType_;
			newSamplerSetter.sampler_ = GameEngineSampler::Find("EngineSampler_Linear");
			newSamplerSetter.bindPoint_ = resInfo.BindPoint;

			std::pair<std::map<std::string, GameEngineSamplerSetter>::iterator, bool> insertResult
				= samplerSetterMap_.insert(std::make_pair(uppercaseResourceName, newSamplerSetter));
			//맵에 겹치는 키값을 가진 원소를 삽입하려고 하면 중복된 키값을 가진 원소를 가리키는 
			//이터레이터와 false가 든 페어를 반환하고 삽입 시도는 무시된다.
			//삽입이 성공했다면 삽입한 원소를 가리키는 이터레이터와 true를 가진 페어를 반환한다.

			if (false == insertResult.second)
			{
				MsgBoxAssertString(std::string(resInfo.Name) + ":  이미 같은 이름의 텍스처 세터가 존재합니다.");
				return;
			}

			break;
		}

		//case D3D_SIT_STRUCTURED:
		//{
		//	//구조화버퍼는 구조적인 특성상 대용량 메모리를 사용하는것이 기본인데, 
		//	//동적 할당되는 특성상 미리 만들수도 없다.

		//	ID3D11ShaderReflectionConstantBuffer* cBufferPtr = compileInfo->GetConstantBufferByName(resInfo.Name);
		//	D3D11_SHADER_BUFFER_DESC shaderBufferDesc = { 0 };
		//	cBufferPtr->GetDesc(&shaderBufferDesc);

		//	GameEngineStructuredBufferSetter newSBufferSetter;
		//	newSBufferSetter.parentShader_ = this;
		//	newSBufferSetter.SetName(uppercaseResourceName);
		//	newSBufferSetter.parentShaderType_ = this->shaderType_;

		//	// 아직은 데이터의 사이즈는 알수있어도 이걸로 몇개짜리 버퍼를 만들지는 알수가 없다.

		//	newSBufferSetter.structuredBuffer_ = GameEngineStructuredBuffer::CreateAndFind(
		//		uppercaseResourceName,	//
		//		shaderBufferDesc,		//
		//		0						//
		//	);
		//	newSBufferSetter.bindPoint_ = resInfo.BindPoint;

		//	std::pair<std::map<std::string, GameEngineStructuredBufferSetter>::iterator, bool> insertResult
		//		= structuredBufferSetterMap_.insert(
		//			std::make_pair(uppercaseResourceName, newSBufferSetter)
		//		);

		//	if (false == insertResult.second)
		//	{
		//		MsgBoxAssertString(std::string(resInfo.Name) + ":  이미 같은 이름의 구조화 버퍼 세터가 존재합니다.");
		//		return;
		//	}

		//	break;
		//}

		default:
		{
			MsgBoxAssert("정의되지 않은 셰이더 리소스 타입입니다.");
			return;
		}
		}
	}
}