#pragma once

class GameEngineConstantBuffer : public GameEngineNameObject
{
	//��� ���� ���� �� ���� Ŭ����.

	//��� ����(Constant buffer): ���ؽ����̴� �� �ȼ����̴����� ���� ������� ��� ���� ����. 
	// ���⼭ ����� ���ӿ�������������̽� Ŭ������ �ִ� RenderStart()�Լ��� ȣ���� ����, 
	// RenderEnd()�Լ��� ȣ��ɶ�����, �׷��ϱ� ������������������ ����Ǵ� ���� ������ �ʴ� ������ ����Ű�� ���̴�.
	// �Ѹ����, ������ GPU�� �Ѱ��ְ��� �ϴ� ����Ÿ ����.
	//��� ���۸� ����ϴ� ������ ���̴����� �Ź� ���Ǵ� ������� cpp ���Ͽ��� �������־�� �Ѵٰ� ������ ��,
	// ���������� �ϳ��ϳ� ���� ������ �ִ� �ͺ��� �Ѳ����� ��Ƶξ��ٰ� �����ؼ� �ű�� ���� ĳ���� ������ ������ ȿ���� �� ����.
	// ���� �ϳ��� ����ü�� ���� �������� �ϱ� ���� ��� ���۶�� ������ ����Ѵ�.
	//��� ������ �޸� ������, ũ�Ⱑ �ݵ�� �ּ� �ϵ���� �Ҵ� ũ��(16 ����Ʈ)�� ������� �Ѵ�.

	//������۴� �̸��� �ݵ�� �־�� �Ѵ�. �׷��� ������ۿ��� ���ʿ��� unnamedRes_�� ���� ���ӿ��������� ��ӹ��� �ʰ� 
	//������ ������ ������ �Ѵ�.

	//������� ��ü�� ������� ���� ������ �򰥸��� �� ��.


public:
	GameEngineConstantBuffer();
	~GameEngineConstantBuffer();

protected:
	GameEngineConstantBuffer(const GameEngineConstantBuffer& _other) = delete;
	GameEngineConstantBuffer(GameEngineConstantBuffer&& _other) noexcept = delete;

private:
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer& _other) = delete;
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer&& _other) = delete;


public:
	static GameEngineConstantBuffer* Create(
		const std::string& _name,	//������۴� �̸��� �ݵ�� �־�� �Ѵ�. �׷��� �ٸ� ���ҽ� �����Լ��ʹ� �ٸ��� �̸� �⺻���� ����.
		const D3D11_SHADER_BUFFER_DESC& _desc,
		ID3D11ShaderReflectionConstantBuffer* _cBufferPtr
	);
	static GameEngineConstantBuffer* Find(const std::string& _name, int _byteSize);
	static GameEngineConstantBuffer* CreateAndFind(
		const std::string& _name,
		const D3D11_SHADER_BUFFER_DESC& _desc,
		ID3D11ShaderReflectionConstantBuffer* _cBufferPtr
	);



	//�� �������� �ٲ�� Ʈ������ ������ ���� ������۰� ���� ������ �׷���ī���� �޸𸮷� �����ϴ� �Լ�.
	void ChangeData(const void* _data, size_t _size) const;		
	//�������� ��������� ����Ÿ������ ���⿡�� �Ѵ�.
	//��, ��� �Լ��� ȣ���ϹǷ� �ּ������� ����� ��.

	//������۸� ������ ������������ �������̴��� �����ϴ� �Լ�.
	void VSSetting(int _bindPoint);

	//������۸� ������ ������������ �ȼ����̴��� �����ϴ� �Լ�.
	void PSSetting(int _bindPoint);

	static void ResourceDestroy();



private:
	static GameEngineConstantBuffer* CreateNamedRes(const std::string& _name, int _byteSize);
	static GameEngineConstantBuffer* CreateRes(const std::string& _name);
	void CreateConstantBuffer(
		const D3D11_SHADER_BUFFER_DESC& _desc,
		ID3D11ShaderReflectionConstantBuffer* _cBufferPtr
	);

private:
	static std::map<std::string, std::map<int, GameEngineConstantBuffer*>> namedRes_;
	//������۸� �̸��� ����Ʈ ũ��α��� �����ϱ� ���� ���� �� ���� ���.
	//�׷��� ���� ���̴����� �Ȱ��� ������۸� ���� ��쿡�� ������ ������ �ʴ´�.

	//typedef struct _D3D11_SHADER_BUFFER_DESC
	//{
	//	LPCSTR                  Name;		������� �̸�.
	//	D3D_CBUFFER_TYPE        Type;		��������� ����.
	//	UINT                    Variables;	��������� ������� ����.
	//	UINT                    Size;		������� ũ��.
	//	UINT                    uFlags;		������۰� ����� ������ �����ϴ� �÷���. 
	//} D3D11_SHADER_BUFFER_DESC;
	D3D11_SHADER_BUFFER_DESC shaderBufferDesc_;
	D3D11_BUFFER_DESC cBufferDesc_;


	ID3D11Buffer* constantBuffer_;


};
