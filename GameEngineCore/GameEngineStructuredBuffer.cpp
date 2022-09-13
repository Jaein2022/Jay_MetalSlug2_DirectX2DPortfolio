#include "PreCompile.h"
#include "GameEngineStructuredBuffer.h"
#include "GameEngineDevice.h"

std::map<std::string, std::map<int, GameEngineStructuredBuffer*>> GameEngineStructuredBuffer::allStructuredBuffers_;

GameEngineStructuredBuffer::GameEngineStructuredBuffer()
	: structuredBuffer_(nullptr),
	structuredBufferDesc_(),
	shaderBufferDesc_(),
	settingResource_(),
	shaderResourceView_(nullptr),
	dataCount_(0)
{
}

GameEngineStructuredBuffer::~GameEngineStructuredBuffer()
{
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}

	if (nullptr != structuredBuffer_)
	{
		structuredBuffer_->Release();
		structuredBuffer_ = nullptr;
	}
}

GameEngineStructuredBuffer* GameEngineStructuredBuffer::Create(
	const std::string& _name,
	const D3D11_SHADER_BUFFER_DESC& _desc,
	int _dataCount
)
{
	GameEngineStructuredBuffer* newRes = CreateNamedRes(_name, _desc.Size);
	newRes->CreateOrResize(_desc, _dataCount, nullptr);
	return newRes;
}

GameEngineStructuredBuffer* GameEngineStructuredBuffer::Find(const std::string& _name, int _byteWidth)
{
	std::string uppercaseSBufferName = GameEngineString::ToUpperReturn(_name);
	std::map<std::string, std::map<int, GameEngineStructuredBuffer*>>::iterator nameFindIter
		= allStructuredBuffers_.find(uppercaseSBufferName);

	if (allStructuredBuffers_.end() == nameFindIter)
	{
		return nullptr;
	}

	std::map<int, GameEngineStructuredBuffer*>::iterator sizeFindIter = nameFindIter->second.find(_byteWidth);

	if (nameFindIter->second.end() == sizeFindIter)
	{
		return nullptr;
	}

	return sizeFindIter->second;
}

GameEngineStructuredBuffer* GameEngineStructuredBuffer::CreateAndFind(
	const std::string& _name,
	const D3D11_SHADER_BUFFER_DESC& _desc,
	int _dataCount
)
{
	GameEngineStructuredBuffer* findBuffer = Find(_name, _desc.Size);

	if (nullptr != findBuffer)
	{
		return findBuffer;
	}

	GameEngineStructuredBuffer* newBuffer = CreateNamedRes(_name, _desc.Size);

	newBuffer->CreateOrResize(_desc, _dataCount);

	return newBuffer;
}

void GameEngineStructuredBuffer::ResourceDestroy()
{
	for (std::pair<std::string, std::map<int, GameEngineStructuredBuffer*>> nameSortedBuffer : allStructuredBuffers_)
	{
		for (std::pair<int, GameEngineStructuredBuffer*> sizeSortedBuffer : nameSortedBuffer.second)
		{
			delete sizeSortedBuffer.second;
			sizeSortedBuffer.second = nullptr;
		}
	}
}

GameEngineStructuredBuffer* GameEngineStructuredBuffer::CreateNamedRes(const std::string& _name, int _byteWidth)
{
	std::string uppercaseBufferName = GameEngineString::ToUpperReturn(_name);

	GameEngineStructuredBuffer* findBuffer = Find(_name, _byteWidth);

	if (nullptr != findBuffer)
	{
		return findBuffer;
	}
	else
	{
		GameEngineStructuredBuffer* newBuffer = CreateRes(_name);
		//GameEngineRes의 namedRes_에 등록시키지 않기 위해 CreateRes()함수를 직접 호출해서 생성한다.
		allStructuredBuffers_[uppercaseBufferName][_byteWidth] = newBuffer;

		return newBuffer;
	}
}

void GameEngineStructuredBuffer::CreateOrResize(
	const D3D11_SHADER_BUFFER_DESC& _desc,
	int _dataCount,
	void* _initializationData /*= nullptr*/
)
{
	this->shaderBufferDesc_ = _desc;

	if (0 == dataCount_)
	{
		return;
	}

	if (dataCount_ >= _dataCount)	//새 데이터 개수가 이전 데이타 개수보다 작거나 같으면 그냥 넘어간다.
	{
		return;
	}

	this->Release();	//더 크다면 std::vector처럼 지우고 다시 만든다.
	//데이터카운트도 바꿔야 하는거 아닌가??
	//이름은 안바뀌었지만 크기는 바뀌었으니 재분류해야하는거 아닌가??

	structuredBufferDesc_.ByteWidth = shaderBufferDesc_.Size * _dataCount;
	//GPU에 생성할 구조화 버퍼 메모리의 전체 크기(최소단위 ??)

	structuredBufferDesc_.Usage = D3D11_USAGE_DYNAMIC;	//버퍼의 사용 방식.
	//D3D11_USAGE_DYNAMIC: GPU는 읽기만, CPU는 쓰기만 가능.
	//구조화 버퍼의 데이터와 크기는 지속적으로 바뀌므로 CPU가 갱신할 수 있게 해야 한다.

	structuredBufferDesc_.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//렌더링 파이프라인에 셰이더 리소스로 연결.
	//구조화 버퍼는 버퍼지만 버퍼가 아니라, 버퍼 기반 셰이더 리소스로 활용하므로 셰이더 리소스로 연결한다.
	//구조화 버퍼는 D3D11_BIND_SHADER_RESOURCE와 D3D11_BIND_UNORDERED_ACCESS 두가지 바인드플래그밖에 쓸 수 없다.

	structuredBufferDesc_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//버퍼에 대한 CPU의 접근권한 설정. 
	//D3D11_CPU_ACCESS_WRITE: CPU는 버퍼에 쓰기만 가능.
	//구조화 버퍼의 데이터와 크기는 지속적으로 바뀌므로 CPU가 갱신할 수 있게 해야 한다.

	structuredBufferDesc_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	//버퍼에 관련된 부가 옵션 설정.
	//D3D11_RESOURCE_MISC_BUFFER_STRUCTURED: 이 버퍼를 구조화 버퍼로 설정.

	structuredBufferDesc_.StructureByteStride = shaderBufferDesc_.Size;	//구조화 버퍼 데이터의 단위 크기.
	//사실상 구조화버퍼 전용 기능이므로 반드시 넣어준다.

	D3D11_SUBRESOURCE_DATA initializationData = { 0 };
	if (nullptr != _initializationData)
	{
		initializationData.pSysMem = _initializationData;
		initializationData.SysMemPitch = 0;			//2, 3차원 텍스처가 아니므로 0.
		initializationData.SysMemSlicePitch = 0;		//3차원 텍스처가 아니므로 0.
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(
		&structuredBufferDesc_,
		&initializationData,
		&structuredBuffer_
	))
	{
		MsgBoxAssert("구조화 버퍼 생성 실패.");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	//구조화 버퍼의 셰이더 리소스뷰를 만드는게 필요한 명세서.
	//구조화 버퍼는 버퍼지만 렌더링 파이프라인에서 셰이더 리소스로 사용되므로, 그에 맞게 셰이더 리소스뷰를 생성해야 한다.

	shaderResourceViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	//DXGI_FORMAT::DXGI_FORMAT_UNKNOWN: DXGI_FORMAT의 기존 포맷에 없는 형식.
	//구조화 버퍼의 원소는 사용자정의 구조체이므로 DXGI_FORMAT의 기존 포맷으로 정의 할 수 없다.
	//그러므로 DXGI_FORMAT_UNKNOWN으로 형식을 지정한다.

	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	//D3D11_SRV_DIMENSION_BUFFEREX: 셰이더 리소스뷰를 만들려는 리소스가 텍스처나 텍스처 배열이 아니라 
	// 정점버퍼나 인덱스버퍼 등으로 가공되지 않은 형식의 버퍼이다.

	shaderResourceViewDesc.BufferEx.FirstElement = 0;
	//??

	shaderResourceViewDesc.BufferEx.NumElements = _dataCount;
	//??

	shaderResourceViewDesc.BufferEx.Flags = 0;
	//??


	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(
		structuredBuffer_,
		&shaderResourceViewDesc,
		&this->shaderResourceView_
	))
	{
		MsgBoxAssert("구조화 버퍼의 셰이더리소스뷰 생성 실패.");
		return;
	}
}

void GameEngineStructuredBuffer::Release()
{
	if (nullptr != shaderResourceView_)
	{
		shaderResourceView_->Release();
		shaderResourceView_ = nullptr;
	}

	if (nullptr != structuredBuffer_)
	{
		structuredBuffer_->Release();
		structuredBuffer_ = nullptr;
	}
}
