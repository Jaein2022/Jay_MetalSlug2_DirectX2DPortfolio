#pragma once

enum class ShaderType
{
	VertexShader,
	PixelShader,
	Max
};

class GameEngineShader;
class ShaderResSetter : public GameEngineNameObject
{
	//������ ���̴��鳢�� ������۳� �ٸ� ���̴� ���ҽ����� �����ϴ���, ���ε�����Ʈ ����
	// ���̴� ���ҽ� �����ΰ� �� �� ���� ������ �ٸ� ��쿡 �����ϱ� ���� ����, ���̴����ҽ� �ܺ����� ��������� Ŭ����.

public:
	GameEngineShader* parentShader_;	//�� ���ҽ��� ����� �θ� ���̴�.
	int bindPoint_;		//�ش� ���̴����ҽ��� ���ε�����Ʈ(�������� ��� ��ȣ).
	ShaderType parentShaderType_;	//�� ���ҽ��� ����� �θ� ���̴��� ����.
	std::function<void()> settingFunction_;	//���̴��� ���� ���ҽ����� �����ϴ� �Լ�.
	std::function<void()> resetFunction_;	//���̴��� ���� ���ҽ����� �����ϴ� �Լ�.

protected:
	ShaderResSetter()
		: parentShader_(nullptr),
		bindPoint_(-1),
		parentShaderType_(ShaderType::Max),
		settingFunction_(nullptr),
		resetFunction_(nullptr)
	{
	}

	~ShaderResSetter() {}
};

class GameEngineConstantBuffer;
class GameEngineConstantBufferSetter : public ShaderResSetter
{
	//���̴����� �ʿ�� �ϴ� ������ ������۰� ������ �ϴ� ���� ���� Ŭ����.
	//�������� n��->���������������ΰ� Ʈ�������� ���� n�� �ʿ�. ->������ Ʈ������ �����͸� GPU�� ������ ������۵� n�� �ʿ�.
	// ->n���� ������ ���������ο� ���� 2(+@)*n���� ���̴����� 2(+@)*n���� ������۸� �ʿ�� �Ѵ�.
	// �� 2(+@)*n���� ������۵��� ���� ���̴��� �����ؾ� �ϴ� ����, ������۵��� ������ ���̴�, 
	// ������۵��� �����ϴµ� �ʿ��� �̸��� �� ũ�� ���� �����Ѵ�.


	friend class GameEngineShader;

public:
	GameEngineConstantBuffer* constantBuffer_;
	//

	//Map()�Լ��� ���� GPU�� ������, ������ ������۵��� ���� �������� �ּҰ�. 
	const void* settingDataToGPU_;

	UINT byteWidth_;	//������� ��ü ũ��.

	std::vector<char> originalData_;
	// �ƿ� �ڱ� �޸𸮷� �����??

	void Setting() const;

	//private:	<-������ �� ���⿣ ���� �ʹ� ���������� ������ ������ �ܺο��� ����ؼ� �ȵȴ�.
	// ���⼭ �ܺδ� �� ���ҽ��� ���� ����� GameEngineShader�� �ڽ� Ŭ������� ���̴��� ����� GameEngineRenderingPipeLine,
	// ������ ������������ ����� GameEngineDefaultRendererŬ������ �� �ڽ�Ŭ�������� �����ϰ�� �� �ܺ��̴�.

	GameEngineConstantBufferSetter()
		: constantBuffer_(nullptr),
		settingDataToGPU_(nullptr),
		byteWidth_(-1)
	{
	}

};

class GameEngineTexture;
class GameEngineTextureSetter : public ShaderResSetter
{
	//�� Ŭ������ ������??

	friend class GameEngineShader;

public:
	GameEngineTexture* texture_;

	void Setting() const;
	void Reset() const;

	//private:	<-������ �� ���⿣ ���� �ʹ� ���������� ������ ������ �ܺο��� ����ؼ� �ȵȴ�.
	// ���⼭ �ܺδ� �� ���ҽ��� ���� ����� GameEngineShader�� �ڽ� Ŭ������� ���̴��� ����� GameEngineRenderingPipeLine,
	// ������ ������������ ����� GameEngineDefaultRendererŬ������ �� �ڽ�Ŭ�������� �����ϰ�� �� �ܺ��̴�.

	GameEngineTextureSetter() : texture_(nullptr)
	{
	}
};

class GameEngineSampler;
class GameEngineSamplerSetter : public ShaderResSetter
{



public:
	GameEngineSampler* sampler_;

	void Setting() const;

	//private:	<-������ �� ���⿣ ���� �ʹ� ���������� ������ ������ �ܺο��� ����ؼ� �ȵȴ�.
	// ���⼭ �ܺδ� �� ���ҽ��� ���� ����� GameEngineShader�� �ڽ� Ŭ������� ���̴��� ����� GameEngineRenderingPipeLine,
	// ������ ������������ ����� GameEngineDefaultRendererŬ������ �� �ڽ�Ŭ�������� �����ϰ�� �� �ܺ��̴�.

	GameEngineSamplerSetter() : sampler_(nullptr)
	{
	}
};

class GameEngineStructuredBuffer;
class GameEngineStructuredBufferSetter : public ShaderResSetter
{
public:
	void Setting() const;

	GameEngineStructuredBuffer* structuredBuffer_;

	GameEngineStructuredBufferSetter() : structuredBuffer_(nullptr)
	{
	}
};


class GameEngineShader
{
	//�� �����ӿ�ũ �� ��� ���̴��� ���̴����� ����ϴ� ���ҽ����� �����ϴ� Ŭ����.
	friend class GameEngineShaderResourceHelper;

public:
	GameEngineShader();
	virtual ~GameEngineShader();

protected:
	GameEngineShader(const GameEngineShader& _other) = delete;
	GameEngineShader(GameEngineShader&& _other) noexcept = delete;

private:
	GameEngineShader& operator=(const GameEngineShader& _other) = delete;
	GameEngineShader& operator=(const GameEngineShader&& _other) = delete;


public:
	static void AutoCompile(const std::string& _path);

public:
	GameEngineConstantBufferSetter& GetConstantBufferSetter(const std::string& _name);


protected:
	void CreateVersion(const std::string& _shaderType, UINT _versionHigh, UINT _versionLow);
	void CompileHLSLCode(const std::string& _path);	//ShaderCompile()�Լ��� ��ü�ϴ�, ���̴� Ŭ���� ���� HLSL�ڵ� ������ �Լ�.
	void ShaderResCheck();	//���� �Է��� ���̴��ڵ�� ���� ���̴��ڵ尡 ������ Ȯ���ϴ� �Լ�.
	//��� �۵��� ������ �����̶� ������ ������ �� ��Ƴ� ���� �����Ƿ� �� �Լ��� ���� ������ �̸� �����Ѵ�.

protected:
	std::string entryPoint_;	//HLSL �ڵ��� �������Լ� �̸�.
	std::string shaderVersion_;	//HLSL �ڵ��� ��� ������ �����Ϸ��� ����.
	ID3DBlob* binaryCode_;	//HLSL �ڵ带 �������� �����(����Ʈ�ڵ�).
	ShaderType shaderType_;		//�� ���̴��� ����.

	std::map<std::string, GameEngineConstantBufferSetter> constantBufferMap_;
	std::map<std::string, GameEngineTextureSetter> textureMap_;
	std::map<std::string, GameEngineSamplerSetter> samplerMap_;
	std::map<std::string, GameEngineStructuredBufferSetter> structuredBufferMap_;

	//���̴����ҽ����͵��� �������� ������ ������??
	//->���̴� ���ҽ� ������ �پ����� �ʾƼ� �������� �����ص� ���� �����̳ʵ��� ���� �ʿ䰡 ����, 
	// ������ ���ҽ����� �и��ؼ� ���� �����ϰ� ������ �� �ִٴ� ������ �̿��� �� �ֱ� ����.

protected:
	inline void SetEntrtyPoint(const std::string& _entryPoint)
	{
		entryPoint_ = _entryPoint;
	}


};

