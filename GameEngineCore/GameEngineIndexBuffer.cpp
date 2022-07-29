#include "PreCompile.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineDevice.h"

GameEngineIndexBuffer::GameEngineIndexBuffer()
	: indexBuffer_(nullptr),
	resData_(),
	bufferDesc_(),
	indexSize_(-1),
	indexCount_(-1),
	offset_(0)
{
}

GameEngineIndexBuffer::~GameEngineIndexBuffer()
{
	if (nullptr != indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}

GameEngineIndexBuffer* GameEngineIndexBuffer::Create(
	const std::string& _name,
	const void* _data,
	UINT _indexSize,
	UINT _indexCount
)
{
	GameEngineIndexBuffer* newRes = CreateNamedRes(_name);
	newRes->CreateIndexBuffer(_data, _indexSize, _indexCount);
	return newRes;
}

void GameEngineIndexBuffer::Setting()
{
	if (nullptr == indexBuffer_)
	{
		MsgBoxAssert("�ε������۰� �����ϴ�.");
		return;
	}

	GameEngineDevice::GetContext()->IASetIndexBuffer(//��ǲ ����� �������� �ε������۸� ���������ο� �����ϴ� �Լ�.
		indexBuffer_,							//���������ο� ������ ����. 2�����迭 �ϳ��� �־���.
		//�����Ҷ� ���ε��÷��׸� D3D11_BIND_INDEX_BUFFER�� �������� ���ۿ��� ���⿡ ������ �� �ִ�.
		DXGI_FORMAT::DXGI_FORMAT_R32_UINT,		//�ε��������� ����Ÿ ����. 
		0										//������ ������ ������ ���� ��ȣ * �� ������ ũ��.
	);
}

void GameEngineIndexBuffer::CreateIndexBuffer(const void* _data, UINT _indexSize, UINT _indexCount)
{
	indexSize_ = _indexSize;
	indexCount_ = _indexCount;

	resData_.pSysMem = _data;	//���� �ʱⵥ��Ÿ ����.
	//�׷���ī�尡 n����Ʈ��ŭ �ڱ� �޸𸮿� �Ҵ��ϴµ� �ʿ��ϹǷ� ���ۿ� ���� ������ ��� �Ѵ�.

	bufferDesc_.ByteWidth = indexSize_ * indexCount_;	//���� ����ü�� ����Ʈũ�� ���.

	bufferDesc_.CPUAccessFlags = 0;	//CPU�� ���� ���� ��� ����. 0: �б�/���� �Ѵ� �Ұ�. 
	//0���� �ϴ°��� ����ȭ ȿ���� �����Ѵ�.

	bufferDesc_.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;		//�� ���۸� � ������� ������� ���. 
	//D3D11_USAGE::D3D11_USAGE_DEFAULT: GPU�� �б�/���� ������ ���ҽ�. CPU�� ���� �Ұ�.

	bufferDesc_.BindFlags = D3D11_BIND_INDEX_BUFFER;	//�� ���۸� ���������ο� � �뵵�� ����ϴ°�.
	//D3D11_BIND_INDEX_BUFFER: ������ ���������ο� �ε������۷� ���.

	bufferDesc_.StructureByteStride = 0;
	//���� ������ �� �� ���� ���̷�ƮX ���� ���۰� �ƴ�, ������ ũ���� ���ҵ�� ������ ����ü�� ���۸� ���� �� �ʿ��ϴٰ� �Ѵ�.
	// 0: ����ü ���۸� ������� ����.

	bufferDesc_.MiscFlags = 0;	//���ۿ� ���õ� �ΰ� �ɼ� ����. 0: �ΰ� �ɼ� ������� ����.	

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(
		&bufferDesc_,
		&resData_,
		&indexBuffer_
	))
	{
		MsgBoxAssert("�ε��� ���� ���� ����.");
		return;
	}
}
