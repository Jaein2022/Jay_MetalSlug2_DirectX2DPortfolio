#include "PreCompile.h"
#include "GameEngineShaderResourceHelper.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h" 

GameEngineShaderResourceHelper::GameEngineShaderResourceHelper()
{
}

GameEngineShaderResourceHelper::~GameEngineShaderResourceHelper()
{
}

void GameEngineShaderResourceHelper::ResourceCheck(GameEngineRenderingPipeLine* _pipeLine)
{
	ShaderCheck(_pipeLine->GetVertexShader());
	ShaderCheck(_pipeLine->GetPixelShader());
}

bool GameEngineShaderResourceHelper::IsConstantBuffer(const std::string& _name)
{
	std::string uppercaseCBufferSetterName = GameEngineString::ToUpperReturn(_name);

	if (constantBufferSetterMap_.end() == constantBufferSetterMap_.find(uppercaseCBufferSetterName))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool GameEngineShaderResourceHelper::IsTexture(const std::string& _name)
{
	const std::string uppercaseTextureSetterName = GameEngineString::ToUpperReturn(_name);

	if (textureSetterMap_.end() == textureSetterMap_.find(uppercaseTextureSetterName))
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool GameEngineShaderResourceHelper::IsSampler(const std::string& _name)
{
	std::string uppercaseSamplerName = GameEngineString::ToUpperReturn(_name);

	if (samplerSetterMap_.end() == samplerSetterMap_.find(uppercaseSamplerName))
	{
		return false;
	}
	else
	{
		return true;
	}

}

void GameEngineShaderResourceHelper::SetConstantBuffer_Link(
	const std::string& _name,
	const void* _data,
	UINT _size
)
{
	if (false == IsConstantBuffer(_name))
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ������� ���Ͱ� �������� �ʽ��ϴ�.");
		return;
	}

	if (16 > _size)
	{
		MsgBoxAssert("������۴� �ּ� 16����Ʈ �̻��� ũ�⸦ ������ �մϴ�.");
		return;
	}

	std::string uppercaseCBufferSetterName = GameEngineString::ToUpperReturn(_name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator nameStartIter =
		constantBufferSetterMap_.lower_bound(uppercaseCBufferSetterName);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator nameEndIter =
		constantBufferSetterMap_.upper_bound(uppercaseCBufferSetterName);


	for (; nameStartIter != nameEndIter; ++nameStartIter)
	{
		//Ʈ�������� �ٲ�� ����� ������ �ڵ� ���ŵȴ�.

		nameStartIter->second.setData_ = _data;
		nameStartIter->second.size_ = _size;
		//��������
	}

}

void GameEngineShaderResourceHelper::SetConstantBuffer_New(const std::string& _name, const void* _data, UINT _size)
{
	if (false == IsConstantBuffer(_name))
	{
		MsgBoxAssertString(_name + ": �׷� �̸��� ������۰� �������� �ʽ��ϴ�.");
		return;
	}

	if (16 > _size)
	{
		MsgBoxAssert("������۴� �ּ� 16����Ʈ �̻��� ũ�⸦ ������ �մϴ�.");
		return;
	}

	std::string uppercaseCBufferSetterName = GameEngineString::ToUpperReturn(_name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator nameStartIter =
		constantBufferSetterMap_.lower_bound(uppercaseCBufferSetterName);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator nameEndIter =
		constantBufferSetterMap_.upper_bound(uppercaseCBufferSetterName);

	for (; nameStartIter != nameEndIter; ++nameStartIter)
	{
		//Ʈ�������� �ٲ��?? �ٲ�� ��??

		if (0 == nameStartIter->second.originalData_.size()
			|| nameStartIter->second.originalData_.size() != _size)
		{
			nameStartIter->second.originalData_.resize(_size);
		}

		nameStartIter->second.setData_ = &nameStartIter->second.originalData_[0];

		memcpy_s(
			&nameStartIter->second.originalData_[0],
			_size,
			_data,
			_size
		);	//��������.

		nameStartIter->second.size_ = _size;
	}

}

GameEngineTexture* GameEngineShaderResourceHelper::SetTexture(
	const std::string& _textureSetterName,
	const std::string& _textureName
)
{
	std::string uppercaseTextureSetterName = GameEngineString::ToUpperReturn(_textureSetterName);

	if (false == IsTexture(uppercaseTextureSetterName))
	{
		MsgBoxAssertString(_textureSetterName + ": �̷� �̸��� �ؽ��� ���Ͱ� �������� �ʽ��ϴ�.");
		return nullptr;
	}

	return SetTexture(_textureSetterName, GameEngineTexture::Find(_textureName));
}

GameEngineTexture* GameEngineShaderResourceHelper::SetTexture(
	const std::string& _textureSetterName,
	GameEngineTexture* _texture
)
{
	std::string uppercaseTextureSetterName = GameEngineString::ToUpperReturn(_textureSetterName);

	if (false == IsTexture(uppercaseTextureSetterName))
	{
		MsgBoxAssertString(_textureSetterName + ": �̷� �̸��� �ؽ��� ���Ͱ� �����ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator nameStartIter
		= textureSetterMap_.lower_bound(uppercaseTextureSetterName);

	std::multimap<std::string, GameEngineTextureSetter>::iterator nameEndIter
		= textureSetterMap_.upper_bound(uppercaseTextureSetterName);

	for (; nameStartIter != nameEndIter; ++nameStartIter)
	{
		BindTexture(nameStartIter->second, _texture);
	}

	return _texture;
}

GameEngineSampler* GameEngineShaderResourceHelper::SetSampler(const std::string& _samplerSetterName, GameEngineSampler* _sampler)
{
	std::string uppercaseSamplerSetterName = GameEngineString::ToUpperReturn(_samplerSetterName);

	if (false == IsSampler(_samplerSetterName))
	{
		MsgBoxAssertString(_samplerSetterName + ": �̷� �̸��� ���÷� ���Ͱ� �������� �ʽ��ϴ�.");
		return nullptr;
	}

	std::multimap<std::string, GameEngineSamplerSetter>::iterator nameStartIter
		= samplerSetterMap_.lower_bound(uppercaseSamplerSetterName);

	std::multimap<std::string, GameEngineSamplerSetter>::iterator nameEndIter
		= samplerSetterMap_.upper_bound(uppercaseSamplerSetterName);

	for (; nameStartIter != nameEndIter; ++nameStartIter)
	{
		BindSampler(nameStartIter->second, _sampler);
	}

	return _sampler;
}

GameEngineSampler* GameEngineShaderResourceHelper::SetSampler(const std::string& _samplerSetterName, const std::string& _samplerName)
{
	std::string uppercaseSamplerSetterName = GameEngineString::ToUpperReturn(_samplerSetterName);

	if (false == IsSampler(_samplerSetterName))
	{
		MsgBoxAssertString(_samplerSetterName + ": �̷� �̸��� ���÷� ���Ͱ� �������� �ʽ��ϴ�.");
		return nullptr;
	}


	return SetSampler(_samplerSetterName, GameEngineSampler::Find(_samplerName));

}

void GameEngineShaderResourceHelper::AllResourcesSetting()
{
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& cBufferSetter
		: constantBufferSetterMap_)
	{
		cBufferSetter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& textureSetter
		: textureSetterMap_)
	{
		textureSetter.second.Setting();
	}

	for (const std::pair<std::string, GameEngineSamplerSetter>& samplerSetter
		: samplerSetterMap_)
	{
		samplerSetter.second.Setting();
	}
}

void GameEngineShaderResourceHelper::ShaderCheck(GameEngineShader* _shader)
{
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& setterPair
		: _shader->constantBufferMap_)
	{
		std::multimap<std::string, GameEngineConstantBufferSetter>::iterator insertIter =
			constantBufferSetterMap_.insert(std::make_pair(setterPair.first, setterPair.second));

		BindConstantBuffer(insertIter->second, setterPair.second.constantBuffer_);
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& setterPair
		: _shader->textureMap_)
	{
		std::multimap<std::string, GameEngineTextureSetter>::iterator insertIter
			= textureSetterMap_.insert(std::make_pair(setterPair.first, setterPair.second));


		BindTexture(insertIter->second, setterPair.second.texture_);
	}


	for (const std::pair<std::string, GameEngineSamplerSetter>& setterPair
		: _shader->samplerMap_)
	{
		std::multimap<std::string, GameEngineSamplerSetter>::iterator insertIter
			= samplerSetterMap_.insert(std::make_pair(setterPair.first, setterPair.second));

		BindSampler(insertIter->second, setterPair.second.sampler_);
	}
}

void GameEngineShaderResourceHelper::BindConstantBuffer(GameEngineConstantBufferSetter& _cBufferSetter, GameEngineConstantBuffer* _cBuffer)
{
	_cBufferSetter.constantBuffer_ = _cBuffer;

	if (nullptr == _cBufferSetter.constantBuffer_)
	{
		MsgBoxAssert("������۰� �������� �ʽ��ϴ�.");
		return;
	}


	switch (_cBufferSetter.parentShaderType_)
	{
	case ShaderType::VertexShader:
	{
		_cBufferSetter.settingFunction_ = std::bind(
			&GameEngineConstantBuffer::VSSetting,
			_cBufferSetter.constantBuffer_,
			_cBufferSetter.bindPoint_
		);
		break;
	}

	case ShaderType::PixelShader:
	{
		_cBufferSetter.settingFunction_ = std::bind(
			&GameEngineConstantBuffer::PSSetting,
			_cBufferSetter.constantBuffer_,
			_cBufferSetter.bindPoint_
		);
		break;
	}

	default:
		MsgBoxAssert("���� �غ���� ���� ���̴� Ÿ���Դϴ�.");
		return;
	}
}

void GameEngineShaderResourceHelper::BindTexture(GameEngineTextureSetter& _textureSetter, GameEngineTexture* _texture)
{
	_textureSetter.texture_ = _texture;


	if (nullptr == _textureSetter.texture_)
	{
		MsgBoxAssert("�ؽ�ó�� �������� �ʽ��ϴ�.");
		return;
	}

	switch (_textureSetter.parentShaderType_)
	{
	case ShaderType::VertexShader:
	{
		_textureSetter.settingFunction_ = std::bind(
			&GameEngineTexture::VSSetting,
			_textureSetter.texture_,
			_textureSetter.bindPoint_
		);
		break;
	}

	case ShaderType::PixelShader:
	{
		_textureSetter.settingFunction_ = std::bind(
			&GameEngineTexture::PSSetting,
			_textureSetter.texture_,
			_textureSetter.bindPoint_
		);
		break;
	}

	default:
		MsgBoxAssert("���� �غ���� ���� ���̴� Ÿ���Դϴ�.");
		return;
	}


}

void GameEngineShaderResourceHelper::BindSampler(GameEngineSamplerSetter& _samplerSetter, GameEngineSampler* _sampler)
{
	_samplerSetter.sampler_ = _sampler;

	if (nullptr == _samplerSetter.sampler_)
	{
		MsgBoxAssert("���÷��� �������� �ʽ��ϴ�.");
		return;
	}

	switch (_samplerSetter.parentShaderType_)
	{
	case ShaderType::VertexShader:
	{
		_samplerSetter.settingFunction_ = std::bind(
			&GameEngineSampler::VSSetting,
			_samplerSetter.sampler_,
			_samplerSetter.bindPoint_
		);
		break;
	}

	case ShaderType::PixelShader:
	{
		_samplerSetter.settingFunction_ = std::bind(
			&GameEngineSampler::PSSetting,
			_samplerSetter.sampler_,
			_samplerSetter.bindPoint_
		);
		break;
	}

	default:
		MsgBoxAssert("���� �غ���� ���� ���̴� Ÿ���Դϴ�.");
		return;
	}

}
