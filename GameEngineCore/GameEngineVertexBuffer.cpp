#include "PreCompile.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineDevice.h"

GameEngineVertexBuffer::GameEngineVertexBuffer()
	: vertexBuffer_(nullptr),
	bufferDesc_(),
	resData_(),
	vertexSize_(-1),
	vertexCount_(-1),
	offset_(0),
	inputLayoutInfo_(nullptr)
{
}

GameEngineVertexBuffer::~GameEngineVertexBuffer()
{
	if (nullptr != vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
}

GameEngineVertexBuffer* GameEngineVertexBuffer::Create(
	const std::string& _name,
	const void* _data,
	UINT _vertexSize,
	UINT _vertexCount,
	const GameEngineInputLayoutInfo& _info
)
{
	GameEngineVertexBuffer* newRes = CreateNamedRes(_name);
	newRes->inputLayoutInfo_ = &_info;	//const �ڷ���* ������ const �ڷ���&�� �ּҰ��� �־��ֹǷ� �����ϴµ� �ƹ� ���� ����.
	newRes->CreateVertexBuffer(_data, _vertexSize, _vertexCount);
	return newRes;
}

void GameEngineVertexBuffer::Setting()
{
	if (nullptr == vertexBuffer_)
	{
		MsgBoxAssert("���ؽ� ���۰� �����ϴ�.");
		return;
	}

	//������ �׸� �� �ִ� ����� �����ϱ� ������, �� ���� ���� �� ����.
	GameEngineDevice::GetContext()->IASetVertexBuffers(	//��ǲ ����� �������� ���ؽ����۵��� ���������ο� �����ϴ� �Լ�. 
		0,				//���ؽ����� �迭�� ������ ������������������ ���� ��ȣ. �ļ� ���ҵ��� �Ͻ������� ���� ��ȣ ���Կ� ����ȴٰ� �Ѵ�.
		//D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT������ ���� ������ �����ϴٰ� �Ѵ�. ������ �� ���� ���� �� ����. 
		1,				//������ ���������ο� ������ ���ؽ� ���� �迭�� ���� ����.
		//D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - ù��° ���ҷ� �־��� ��
		&vertexBuffer_,		//���������ο� ������ ���� �迭�� �ּҰ�. 
		//�����Ҷ� ���ε��÷��׸� D3D11_BIND_VERTEX_BUFFER�� �������� ���ۿ��� ���⿡ ������ �� �ִ�.
		&vertexSize_,	//���� �迭�� ���Ұ� ���� == ���ؽ����� �Ѱ��� ũ��.
		&offset_		//������ ������ �迭 �� ������ ���� ��ȣ * ���� ũ��.
	);
}

void GameEngineVertexBuffer::CreateVertexBuffer(
	const void* _data,
	UINT _vertexSize,
	UINT _vertexCount
)
{
	vertexSize_ = _vertexSize;
	vertexCount_ = _vertexCount;


	resData_.pSysMem = _data;	//���� �ʱⵥ��Ÿ ����.
	//�׷���ī�尡 n����Ʈ��ŭ �ڱ� �޸𸮿� �Ҵ��ϴµ� �ʿ��ϹǷ� ���ۿ� ���� ������ ��� �Ѵ�.

	bufferDesc_.ByteWidth = vertexSize_ * vertexCount_;	//���� ����ü�� ����Ʈũ�� ���.

	bufferDesc_.CPUAccessFlags = 0;	//CPU�� ���� ���� ��� ����. 0: �б�/���� �Ѵ� �Ұ�. 
	//0���� �ϴ°��� ����ȭ ȿ���� �����Ѵ�.

	bufferDesc_.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;		//�� ���۸� � ������� ������� ���. 
	//D3D11_USAGE::D3D11_USAGE_DEFAULT: GPU�� �б�/���� ������ ���ҽ�. CPU�� ���� �Ұ�.

	bufferDesc_.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//�� ���۸� ���������ο� � �뵵�� ����ϴ°�.
	//D3D11_BIND_VERTEX_BUFFER: ������ ���������ο� ���ؽ����۷� ���.

	bufferDesc_.StructureByteStride = 0;
	//���� ������ �� �� ���� ���̷�ƮX ���� ���۰� �ƴ�, ������ ũ���� ���ҵ�� ������ ��Ʈ���ĵ� ���۸� ���� �� �ʿ��ϴٰ� �Ѵ�.
	// 0: ��Ʈ���ĵ� ���۸� ������� ����.

	bufferDesc_.MiscFlags = 0;	//���ۿ� ���õ� �ΰ� �ɼ� ����. 0: �ΰ� �ɼ� ������� ����.	

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(
		&bufferDesc_,
		&resData_,
		&vertexBuffer_
	))
	{
		MsgBoxAssert("���ؽ� ���� ���� ����.");
		return;
	}
}

