#include "PreCompile.h"
#include "GameEngineVertexes.h"
#include "GameEngineDevice.h"
#include "GameEngineVertexShader.h"

GameEngineInputLayoutInfo GameEngineVertex::inputLayoutInfo_;
unsigned int ConvertFormatToByteScale(DXGI_FORMAT _format);

void GameEngineInputLayoutInfo::AddInputLayout(
	const char* _semanticName,
	DXGI_FORMAT _format,
	unsigned int _index /*= -1*/,
	unsigned int _inputSlot /*= 0*/,
	unsigned int _instanceDataStepRate /*= 0*/,
	D3D11_INPUT_CLASSIFICATION _inputSlotClass /*= D3D11_INPUT_PER_VERTEX_DATA*/
)
{
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc = { 0 };

	int semanticIndex = _index;

	if (-1 == semanticIndex)
		//_index�� �⺻���� -1�̶�� ����ڰ� �ε����� ���� �Ű澲�� �ʰڴٴ� �ǹ��̹Ƿ� �ڵ����� ó���ǰ� �Ѵ�.
	{
		if (semanticIndexData_.end() == semanticIndexData_.find(_semanticName))	//���� �ø�ƽ������ ���ٸ�
		{
			semanticIndexData_[_semanticName] = -1;	//value�� �ε������� -1 ����.
		}
		semanticIndex = ++semanticIndexData_[_semanticName];
		//�ʿ� ����� �ø�ƽ������ �ε����� 1 ������Ű�鼭 semanticIndex�� �����Ѵ�.
	}

	//typedef struct D3D11_INPUT_ELEMENT_DESC		��ǲ���̾ƿ��� �����Ҷ� �ʿ��� ���μ��� ������ ��� ����ü.
	//{
	//	LPCSTR SemanticName;						�� ��ǲ���̾ƿ��� ����Ű�� ����Ÿ�� �ø�ƽ�� �̸�. ��ҹ��� ���� ����.
	//	UINT SemanticIndex;							�� ��ǲ���̾ƿ��� ����Ű�� ����Ÿ�� �ø�ƽ�� ��ȣ.  COLOR[n]�� n.
	//	DXGI_FORMAT Format;							�� ��ǲ���̾ƿ��� ����Ű�� ����Ÿ�� ����ŸŸ��. 
	//	UINT InputSlot;								������ ���������ο� ���ؽ����۸� ����� ���� ��ȣ. 0~15���� ����.
	//	UINT AlignedByteOffset;						�� ��ǲ���̾ƿ��� ����Ű�� ����Ÿ�� ������(�޸� ���������������� ����Ʈ �Ÿ�).
	//	D3D11_INPUT_CLASSIFICATION InputSlotClass;	�� ��ǲ���̾ƿ����� ��ϵǴ°��� �ܼ� ���ؽ� ��������, �ν��Ͻ� ����Ÿ�� �����ϴ����� �˷��ִ� ��.
	//	UINT InstanceDataStepRate;					���� InputSlotClass���� D3D11_INPUT_PER_INSTANCE_DATA�� ������������ �ʿ��� ���̶�� �Ѵ�.
	//} 	D3D11_INPUT_ELEMENT_DESC;

	inputLayoutDesc.SemanticName = _semanticName;
	inputLayoutDesc.SemanticIndex = semanticIndex;
	inputLayoutDesc.Format = _format;
	inputLayoutDesc.InputSlot = _inputSlot;
	inputLayoutDesc.AlignedByteOffset = offset_;
	inputLayoutDesc.InputSlotClass = _inputSlotClass;
	inputLayoutDesc.InstanceDataStepRate = _instanceDataStepRate;

	offset_ += ConvertFormatToByteScale(inputLayoutDesc.Format);

	descs_.push_back(inputLayoutDesc);


}

unsigned int ConvertFormatToByteScale(DXGI_FORMAT _format)
{
	switch (_format)
	{
	case DXGI_FORMAT_UNKNOWN:
		MsgBoxAssert("������ �������� �ʾҽ��ϴ�.");
		return -1;

	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;


	default:
		MsgBoxAssert("ũ�⸦ ���� å������ ���� �����Դϴ�.");
		return -1;
	}
}