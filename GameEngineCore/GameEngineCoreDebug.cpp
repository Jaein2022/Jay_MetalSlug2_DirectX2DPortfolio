#include "PreCompile.h"
#include "GameEngineCoreDebug.h"
#include "GameEngineCore.h"
#include "GameEngineCamera.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResourceHelper.h"
#include "GameEngineTexture.h"


namespace GameEngineDebug
{
	enum class DebugRenderType
	{
		Sphere,
		Box,
	};

	struct DebugInfo
	{
		int type_[4];
		float4 color_;

	public:
		DebugInfo(DebugRenderType _type, const float4& _color)
			: type_{ static_cast<int>(_type) },
			color_(_color)
		{
		}
	};

	struct DebugRenderData
	{
		DebugInfo info_;
		TransformData data_;
		GameEngineTexture* texture_;

		DebugRenderData(const DebugInfo& _info, const TransformData& _data, GameEngineTexture* _texture = nullptr)
			: info_(_info),
			data_(_data),
			texture_(_texture)
		{
		}
	};

	std::vector<DebugRenderData> debugData_ = std::vector<DebugRenderData>();
	GameEngineRenderingPipeLine* debugRenderingPipeLine_;
	GameEngineShaderResourceHelper debugShaderResource_;

	GameEngineRenderingPipeLine* debugTextureRenderingPipeLine_;
	GameEngineShaderResourceHelper debugTextureShaderResource_;


	void Debug3DInitialize()
	{
		static bool isOnce = false;

		if (true == isOnce)
		{
			return;
		}

		debugRenderingPipeLine_ = GameEngineRenderingPipeLine::Find("3DDebug");
		debugShaderResource_.ResourceCheck(debugRenderingPipeLine_);

		debugTextureRenderingPipeLine_ = GameEngineRenderingPipeLine::Find("DebugTexture");
		debugTextureShaderResource_.ResourceCheck(debugTextureRenderingPipeLine_);


		isOnce = true;
	}

	void Debug3DRender()
	{
		for (size_t i = 0; i < debugData_.size(); i++)
		{
			if (nullptr == debugData_[i].texture_)
			{
				debugShaderResource_.SetConstantBuffer_Link("TransformData", debugData_[i].data_);
				debugShaderResource_.SetConstantBuffer_Link("DebugInfo", debugData_[i].info_);
				debugShaderResource_.AllResourcesSetting();
				debugRenderingPipeLine_->Rendering();
			}
			else
			{
				debugTextureShaderResource_.SetConstantBuffer_Link("TransformData", debugData_[i].data_);
				debugTextureShaderResource_.SetConstantBuffer_Link("DebugInfo", debugData_[i].info_);
				debugTextureShaderResource_.SetTexture("Tex", debugData_[i].texture_);
				debugTextureShaderResource_.AllResourcesSetting();
				debugTextureRenderingPipeLine_->Rendering();
			}
		}
		debugData_.clear();
	}

	void GameEngineDebug::DrawBox(const GameEngineTransform& _transform, const float4& _color)
	{
		DrawBox(_transform, GameEngineCore::GetCurrentLevel()->GetMainCamera(), _color);
	}

	void GameEngineDebug::DrawBox(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color)
	{
		static GameEngineTransform debugTransform;

		debugTransform.Copy(_transform);
		debugTransform.SetViewMatrix(_camera->GetViewMatrix());
		debugTransform.SetProjectionMatrix(_camera->GetProjectionMatrix());
		debugTransform.CalculateWorldViewProjection();

		debugData_.push_back(DebugRenderData(DebugInfo(DebugRenderType::Box, _color), debugTransform.GetTransformData()));
	}

	void DrawSphere(const GameEngineTransform& _transform, const float4& _color)
	{
		DrawSphere(_transform, GameEngineCore::GetCurrentLevel()->GetMainCamera(), _color);
	}

	void DrawSphere(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color)
	{
		static GameEngineTransform debugTransform;

		debugTransform.Copy(_transform);
		debugTransform.SetViewMatrix(_camera->GetViewMatrix());
		debugTransform.SetProjectionMatrix(_camera->GetProjectionMatrix());
		debugTransform.CalculateWorldViewProjection();

		debugData_.push_back(
			DebugRenderData{ DebugInfo(DebugRenderType::Sphere, _color), debugTransform.GetTransformData() });
	}

	void DrawTexture(
		const std::string& _textureName,
		const float4& _position,
		const float4& _rotation /*= float4::Zero*/,
		const float4& _scale /*= float4::Zero*/
	)
	{
		DrawTexture(_textureName, GameEngineCore::GetCurrentLevel()->GetMainCamera(), _position, _rotation, _scale);
	}

	void DrawTexture(
		const std::string& _textureName,
		GameEngineCamera* _camera,
		const float4& _position,
		const float4& _rotation /*= float4::Zero*/,
		const float4& _scale /*= float4::Zero*/
	)
	{
		static GameEngineTransform debugTransform;

		GameEngineTexture* findTexture = GameEngineTexture::Find(_textureName);
		if (nullptr == findTexture)
		{
			MsgBoxAssertString(_textureName + ": �׷� �̸��� �ؽ�ó�� �������� �ʽ��ϴ�.");
			return;
		}

		debugTransform.SetLocalScale(_scale);
		debugTransform.SetLocalRotation(_rotation);
		debugTransform.SetLocalPosition(_position);

		if (_scale.CompareInt2D(float4::Zero))
			//ũ�� ������ �⺻���� float4::Zero���, ũ�⼳���� �� ������ ���� ������ �����ϰ� �ؽ�ó ��ü ũ��� �����Ѵ�. 
		{
			debugTransform.SetLocalScale(findTexture->GetScale());
		}

		debugTransform.SetViewMatrix(_camera->GetViewMatrix());
		debugTransform.SetProjectionMatrix(_camera->GetProjectionMatrix());
		debugTransform.CalculateWorldViewProjection();

		debugData_.push_back(
			DebugRenderData(
				DebugInfo(DebugRenderType::Box, float4::White),
				debugTransform.GetTransformData(),
				findTexture));
	}
	void DrawTexture(
		GameEngineTexture* _texture,
		const float4& _position,
		const float4& _rotation /*= float4::Zero*/,
		const float4& _scale /*= float4::Zero*/
	)
	{
		DrawTexture(_texture, GameEngineCore::GetCurrentLevel()->GetMainCamera(), _position, _rotation, _scale);
	}
	void DrawTexture(
		GameEngineTexture* _texture,
		GameEngineCamera* _camera,
		const float4& _position,
		const float4& _rotation /*= float4::Zero*/,
		const float4& _scale /*= float4::Zero*/
	)
	{
		static GameEngineTransform debugTransform;

		if (nullptr == _texture)
		{
			MsgBoxAssert("�������� �ʴ� �ؽ�ó�Դϴ�.");
			return;
		}

		debugTransform.SetLocalScale(_scale);
		debugTransform.SetLocalRotation(_rotation);
		debugTransform.SetLocalPosition(_position);

		if (_scale.CompareInt2D(float4::Zero))
			//ũ�� ������ �⺻���� float4::Zero���, ũ�⼳���� �� ������ ���� ������ �����ϰ� �ؽ�ó ��ü ũ��� �����Ѵ�. 
		{
			debugTransform.SetLocalScale(_texture->GetScale());
		}

		debugTransform.SetViewMatrix(_camera->GetViewMatrix());
		debugTransform.SetProjectionMatrix(_camera->GetProjectionMatrix());
		debugTransform.CalculateWorldViewProjection();

		debugData_.push_back(
			DebugRenderData(
				DebugInfo(DebugRenderType::Box, float4::White),
				debugTransform.GetTransformData(),
				_texture));
	}
}