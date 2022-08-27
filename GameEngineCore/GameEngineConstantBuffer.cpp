#include "PreCompile.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineDevice.h"

std::map<std::string, std::map<int, GameEngineConstantBuffer*>> GameEngineConstantBuffer::namedRes_;

GameEngineConstantBuffer::GameEngineConstantBuffer()
	: shaderBufferDesc_(),
	cBufferDesc_(),
	constantBuffer_(nullptr)
{
}

GameEngineConstantBuffer::~GameEngineConstantBuffer()
{
	if (nullptr != constantBuffer_)
	{
		constantBuffer_->Release();
		constantBuffer_ = nullptr;
	}
}



GameEngineConstantBuffer* GameEngineConstantBuffer::Create(
	const std::string& _name,
	const D3D11_SHADER_BUFFER_DESC& _desc,
	ID3D11ShaderReflectionConstantBuffer* _cBufferPtr
)
{
	GameEngineConstantBuffer* newBuffer = GameEngineConstantBuffer::CreateNamedRes(_name, _desc.Size);

	newBuffer->CreateConstantBuffer(_desc, _cBufferPtr);

	return newBuffer;
}

GameEngineConstantBuffer* GameEngineConstantBuffer::Find(const std::string& _name, int _byteSize)
{
	std::string uppercaesCBufferName = GameEngineString::ToUpperReturn(_name);

	std::map<std::string, std::map<int, GameEngineConstantBuffer*>>::iterator namedIter =
		namedRes_.find(uppercaesCBufferName);

	if (namedRes_.end() == namedIter)
	{
		return nullptr;
	}

	std::map<int, GameEngineConstantBuffer*>::iterator sizeIter = namedIter->second.find(_byteSize);

	if (namedIter->second.end() == sizeIter)
	{
		return nullptr;
	}

	return sizeIter->second;
}

GameEngineConstantBuffer* GameEngineConstantBuffer::CreateAndFind(
	const std::string& _name,
	const D3D11_SHADER_BUFFER_DESC& _desc,
	ID3D11ShaderReflectionConstantBuffer* _cBufferPtr
)
{
	GameEngineConstantBuffer* findCBuffer = Find(_name, _desc.Size);

	if (nullptr != findCBuffer)
	{
		return findCBuffer;
	}

	GameEngineConstantBuffer* newCBuffer = CreateNamedRes(_name, _desc.Size);
	newCBuffer->CreateConstantBuffer(_desc, _cBufferPtr);
	return newCBuffer;
}

void GameEngineConstantBuffer::ChangeData(const void* _data, size_t _size) const
{
	if (nullptr == _data)
	{
		MsgBoxAssertString(this->GetNameCopy() + ": ������ ����Ÿ�� �����ϴ�.");
		return;
	}

	if (this->cBufferDesc_.ByteWidth != _size)
	{
		MsgBoxAssertString(this->GetNameCopy() + ": ��������� ũ�Ⱑ ���� ���� �ʽ��ϴ�.");
		return;
	}

	static D3D11_MAPPED_SUBRESOURCE settingResource = { 0 };
	//�̰� ��������� �Լ� ���� �� const�� �� �� ����. 

	memset(
		&settingResource,
		0,
		sizeof(settingResource)
	);
	//settingResource_�� pData�� ����Ű�� ���� ����ΰ�?? ���� �׷���ī�� �� �޸𸮿����� �Ϻζ�� ��� Map()
	// ������ memset()�� �� �� �ִ°�?? 
	// ->settingResource_�� pData�� ���� ��򰡿� �ְ� memset()���δ� �������� ��ġ�� ����Ű�� �� ���̴�. 
	// map()���Ŀ� �׷���ī�� �޸𸮿����� ��򰡸� ����Ű�� �ȴ�.


	//GPU�� �׷��� ���ҽ��� �ǵ帮�� ���ϰ� ���� �۾��� �Ͻ�������Ű�� �׷���ī���� �޸��� ���� ������ �����ϴ� ����.
	//�޸𸮿� ���� ������ ���ܵǰ� GPU�� �Ͻ������ǹǷ� �翬�� �ӵ��� �پ���. �����ϰ� ����ؾ� �Ѵ�.
	GameEngineDevice::GetContext()->Map(	//���ҽ��� �����͸� �׷���ī��� �ű�� ����, GPU�� �޸𸮿� ���� ���� ������ �Ŵ� �Լ�.
		this->constantBuffer_,	//??
		0,					//���긮�ҽ� �迭 �� ��ȣ. �迭 ���� �Ѱ��� �ܵ����� �־��ֹǷ� 0.
		D3D11_MAP_WRITE_DISCARD,	//CPU�� ���ҽ��� ���� ���ٱ��� ���� �÷���.
		//D3D11_MAP_WRITE_DISCARD: �ش� �޸𸮿� �ִ� ���� ������ �����ϰ� ���ο� ����Ÿ�� ������.
		//bufferData_.Usage�� D3D11_USAGE_DYNAMIC���� �����Ǿ��־�� ������ ������ �ʴ´�.
		0,					//GPU�� �ٻܶ� CPU�� �ൿ�� �����ϴ� �÷���. 0: �������� ����.
		&settingResource	//�׷���ī�� �� �޸��� Ư�� ������ ����Ű�� �� ������.
	);

	//ID3D11Buffer*�� ����Ű�� ���� settingResource_.pData�� ����Ű�� ���� ���� �׷���ī�� �� �޸𸮶� �ٸ� �����ΰ�?
	// �ƴϸ� �ܼ��� �������̽��� ���� �������۰� ���������� �����ΰ�??
	//->ID3D11Buffer*�� ����Ű�� ���� �� ���� Ư�� �����̰�, settingResource_.pData�� ����Ű�� ���� �׷���ī�� �� �޸��� ����̴�.
	// ���� ����Ű�� ������ ������ �ٸ���.

	if (nullptr == settingResource.pData)
	{
		MsgBoxAssert("�׷���ī�� ���ۿ� �������� ���߽��ϴ�.");
		return;
	}

	memcpy_s(
		settingResource.pData,	//������ ����Ÿ�� �������� �� �׷���ī�峻 �޸��� �ּ�.
		cBufferDesc_.ByteWidth,	//�������� �Ҵ��ؾ��ϴ� �޸� ũ��.
		_data,					//����Ÿ ���� ����� �� ������ ���� �޸��� �ּ�.
		cBufferDesc_.ByteWidth	//������ ����Ÿ ũ��.
	);		//���� �ִ� ������� ����Ÿ�� �׷���ī��� �����Ѵ�.

	//�Ͻ����� ���� ����.
	GameEngineDevice::GetContext()->Unmap(	//Map()�Լ��� �ɾ���, �޸𸮿� ���� GPU�� ���� ������ Ǫ�� �Լ�
		this->constantBuffer_,			//??
		0								//���긮�ҽ� �迭�� ��ȣ. �迭 ���� �Ѱ��� �ܵ����� �־��ֹǷ� 0.
	);

}

void GameEngineConstantBuffer::VSSetting(int _bindPoint)
{
	if (nullptr == constantBuffer_)
	{
		MsgBoxAssert("��� ���۰� �����ϴ�.");
		return;
	}

	GameEngineDevice::GetContext()->VSSetConstantBuffers(_bindPoint, 1, &constantBuffer_);
}

void GameEngineConstantBuffer::PSSetting(int _bindPoint)
{
	if (nullptr == constantBuffer_)
	{
		MsgBoxAssert("��� ���۰� �����ϴ�.");
		return;
	}

	GameEngineDevice::GetContext()->PSSetConstantBuffers(_bindPoint, 1, &constantBuffer_);
}

void GameEngineConstantBuffer::ResourceDestroy()
{
	for (std::pair<std::string, std::map<int, GameEngineConstantBuffer*>> namedPair : namedRes_)
	{
		for (std::pair<int, GameEngineConstantBuffer*> sizePair : namedPair.second)
		{
			delete sizePair.second;
			sizePair.second = nullptr;
		}
	}
}

GameEngineConstantBuffer* GameEngineConstantBuffer::CreateNamedRes(const std::string& _name, int _byteSize)
{
	std::string uppercaseCBufferName = GameEngineString::ToUpperReturn(_name);

	GameEngineConstantBuffer* findBuffer = Find(_name, _byteSize);

	//���� �̸�, ���� ����Ʈũ���� ������۰� ������ �����, ������ �����Ѵ�.
	if (nullptr != findBuffer)
	{
		return findBuffer;
		//�̹� ������� �ִ°� �� ���� �Ͷ߸��� �ʰ�, ��� �̹� ������� �ִ°� �����Ѵ�.
	}
	else
	{
		GameEngineConstantBuffer* newRes = CreateRes(uppercaseCBufferName);

		namedRes_[uppercaseCBufferName][_byteSize] = newRes;

		return newRes;
	}
}

GameEngineConstantBuffer* GameEngineConstantBuffer::CreateRes(const std::string& _name)
{
	GameEngineConstantBuffer* newRes = new GameEngineConstantBuffer();
	newRes->SetName(_name);
	return newRes;
}

void GameEngineConstantBuffer::CreateConstantBuffer(
	const D3D11_SHADER_BUFFER_DESC& _desc,
	ID3D11ShaderReflectionConstantBuffer* _cBufferPtr	//�ǹ� ����.
)
{
	shaderBufferDesc_ = _desc;

	cBufferDesc_.ByteWidth = shaderBufferDesc_.Size;	//������ ������� ũ��.

	cBufferDesc_.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;	//������ ���ó.
	//D3D11_USAGE_DYNAMIC: GPU�� �б⸸, CPU�� ���⸸ ����.
	//��������� ����Ÿ�� ���������� �ٲ�Ƿ� CPU�� ������ �� �ְ� �ؾ� �Ѵ�.

	cBufferDesc_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//������ ���������ο� ������۷� �����Ѵ�.

	cBufferDesc_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//���ۿ� ���� CPU�� ���ٱ��� ����. 
	//D3D11_CPU_ACCESS_WRITE: CPU�� ���ۿ� ���⸸ ����.
	//��������� ����Ÿ�� ���������� �ٲ�Ƿ� CPU�� ������ �� �ְ� �ؾ� �Ѵ�.

	cBufferDesc_.StructureByteStride = 0;
	//���� ������ �� �� ���� ���̷�ƮX ���� ���۰� �ƴ�, ���̷�ƮX ���� ���ۿ� ������ ũ���� ���ҵ�� ������ ��Ʈ���ĵ� ���۸� ���� �� �ʿ��ϴٰ� �Ѵ�.
	// 0: ��Ʈ���ĵ� ���۸� ������� ����.

	cBufferDesc_.MiscFlags = 0;			//���ۿ� ���õ� �ΰ� �ɼ� ����. 0: �ΰ� �ɼ� ������� ����.	

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(
		&cBufferDesc_,
		nullptr,	//������۴� ��� ���� �ٲ� �����̹Ƿ� �ʱⰪ�� �ǹ̰� ����.
		&constantBuffer_
	))
	{
		MsgBoxAssert("������� ���� ����.");
		return;
	}


}