#pragma once

class GameEngineInputLayoutInfo
{
	//인풋레이아웃 생성시 필요한 D3D11_INPUT_ELEMENT_DESC 구조체들과 부가 정보를 한 묶음으로 관리하는 클래스.
	//일반적인 멤버변수를 가지면 안되는 이유가 무엇??


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
	//버텍스 하나마다 가져야 하는 정보들을 하나로 모아 관리하는 클래스.

	static GameEngineInputLayoutInfo inputLayoutInfo_;
	//버텍스 하나마다 inputLayoutDescs_ 한개씩을 가진다. 

	float4 position_;
	float4 color_;
	float4 texcoord_;
};

