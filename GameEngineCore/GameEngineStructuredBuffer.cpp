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
		//GameEngineRes�� namedRes_�� ��Ͻ�Ű�� �ʱ� ���� CreateRes()�Լ��� ���� ȣ���ؼ� �����Ѵ�.
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

	if (dataCount_ >= _dataCount)	//�� ������ ������ ���� ����Ÿ �������� �۰ų� ������ �׳� �Ѿ��.
	{
		return;
	}

	this->Release();	//�� ũ�ٸ� std::vectoró�� ����� �ٽ� �����.
	//������ī��Ʈ�� �ٲ�� �ϴ°� �ƴѰ�??
	//�̸��� �ȹٲ������ ũ��� �ٲ������ ��з��ؾ��ϴ°� �ƴѰ�??

	structuredBufferDesc_.ByteWidth = shaderBufferDesc_.Size * _dataCount;
	//GPU�� ������ ����ȭ ���� �޸��� ��ü ũ��(�ּҴ��� ??)

	structuredBufferDesc_.Usage = D3D11_USAGE_DYNAMIC;	//������ ��� ���.
	//D3D11_USAGE_DYNAMIC: GPU�� �б⸸, CPU�� ���⸸ ����.
	//����ȭ ������ �����Ϳ� ũ��� ���������� �ٲ�Ƿ� CPU�� ������ �� �ְ� �ؾ� �Ѵ�.

	structuredBufferDesc_.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//������ ���������ο� ���̴� ���ҽ��� ����.
	//����ȭ ���۴� �������� ���۰� �ƴ϶�, ���� ��� ���̴� ���ҽ��� Ȱ���ϹǷ� ���̴� ���ҽ��� �����Ѵ�.
	//����ȭ ���۴� D3D11_BIND_SHADER_RESOURCE�� D3D11_BIND_UNORDERED_ACCESS �ΰ��� ���ε��÷��׹ۿ� �� �� ����.

	structuredBufferDesc_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//���ۿ� ���� CPU�� ���ٱ��� ����. 
	//D3D11_CPU_ACCESS_WRITE: CPU�� ���ۿ� ���⸸ ����.
	//����ȭ ������ �����Ϳ� ũ��� ���������� �ٲ�Ƿ� CPU�� ������ �� �ְ� �ؾ� �Ѵ�.

	structuredBufferDesc_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	//���ۿ� ���õ� �ΰ� �ɼ� ����.
	//D3D11_RESOURCE_MISC_BUFFER_STRUCTURED: �� ���۸� ����ȭ ���۷� ����.

	structuredBufferDesc_.StructureByteStride = shaderBufferDesc_.Size;	//����ȭ ���� �������� ���� ũ��.
	//��ǻ� ����ȭ���� ���� ����̹Ƿ� �ݵ�� �־��ش�.

	D3D11_SUBRESOURCE_DATA initializationData = { 0 };
	if (nullptr != _initializationData)
	{
		initializationData.pSysMem = _initializationData;
		initializationData.SysMemPitch = 0;			//2, 3���� �ؽ�ó�� �ƴϹǷ� 0.
		initializationData.SysMemSlicePitch = 0;		//3���� �ؽ�ó�� �ƴϹǷ� 0.
	}

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(
		&structuredBufferDesc_,
		&initializationData,
		&structuredBuffer_
	))
	{
		MsgBoxAssert("����ȭ ���� ���� ����.");
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	//����ȭ ������ ���̴� ���ҽ��並 ����°� �ʿ��� ����.
	//����ȭ ���۴� �������� ������ ���������ο��� ���̴� ���ҽ��� ���ǹǷ�, �׿� �°� ���̴� ���ҽ��並 �����ؾ� �Ѵ�.

	shaderResourceViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	//DXGI_FORMAT::DXGI_FORMAT_UNKNOWN: DXGI_FORMAT�� ���� ���˿� ���� ����.
	//����ȭ ������ ���Ҵ� ��������� ����ü�̹Ƿ� DXGI_FORMAT�� ���� �������� ���� �� �� ����.
	//�׷��Ƿ� DXGI_FORMAT_UNKNOWN���� ������ �����Ѵ�.

	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	//D3D11_SRV_DIMENSION_BUFFEREX: ���̴� ���ҽ��並 ������� ���ҽ��� �ؽ�ó�� �ؽ�ó �迭�� �ƴ϶� 
	// �������۳� �ε������� ������ �������� ���� ������ �����̴�.

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
		MsgBoxAssert("����ȭ ������ ���̴����ҽ��� ���� ����.");
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
