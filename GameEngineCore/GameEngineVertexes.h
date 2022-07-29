#pragma once

class GameEngineInputLayoutInfo
{
	//��ǲ���̾ƿ� ������ �ʿ��� D3D11_INPUT_ELEMENT_DESC ����ü��� �ΰ� ������ �� �������� �����ϴ� Ŭ����.
	//�Ϲ����� ��������� ������ �ȵǴ� ������ ����??


	friend class GameEngineInputLayout;


	std::vector<D3D11_INPUT_ELEMENT_DESC> descs_;	//
	std::map<const char*, int> semanticIndexData_;	//
	unsigned int offset_;							//


public:
	GameEngineInputLayoutInfo() : offset_(0)
	{
	}

public:
	void AddInputLayout(
		const char* _semanticName,
		DXGI_FORMAT _format,
		unsigned int _index = -1,
		unsigned int _inputSlot = 0,
		unsigned int _instanceDataStepRate = 0,
		D3D11_INPUT_CLASSIFICATION _inputSlotClass = D3D11_INPUT_PER_VERTEX_DATA
	);

};


struct GameEngineVertex
{
	//���ؽ� �ϳ����� ������ �ϴ� �������� �ϳ��� ��� �����ϴ� Ŭ����.

	static GameEngineInputLayoutInfo inputLayoutInfo_;
	//���ؽ� �ϳ����� inputLayoutDescs_ �Ѱ����� ������. 

	float4 position_;
	float4 color_;
	float4 texcoord_;
};

