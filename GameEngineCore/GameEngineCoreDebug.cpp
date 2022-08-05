#include "PreCompile.h"
#include "GameEngineCoreDebug.h"
#include "GameEngineCore.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResourceHelper.h"


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

		DebugRenderData(const DebugInfo& _info, const TransformData& _data)
			: info_(_info),
			data_(_data)
		{
		}
	};

	std::vector<DebugRenderData> debugData_ = std::vector<DebugRenderData>();
	GameEngineRenderingPipeLine* debugRenderingPipeLine_;
	GameEngineShaderResourceHelper debugShaderResource_;

	void Debug3DInitialize()
	{
		static bool isOnce = false;

		if (true == isOnce)
		{
			return;
		}

		debugRenderingPipeLine_ = GameEngineRenderingPipeLine::Find("3DDebug");
		debugShaderResource_.ResourceCheck(debugRenderingPipeLine_);

		isOnce = true;
	}

	void Debug3DRender()
	{
		for (size_t i = 0; i < debugData_.size(); i++)
		{
			debugShaderResource_.SetConstantBuffer_Link("TransformData", debugData_[i].data_);
			debugShaderResource_.SetConstantBuffer_Link("DebugInfo", debugData_[i].info_);
			debugShaderResource_.AllResourcesSetting();
			debugRenderingPipeLine_->Rendering();
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

	void GameEngineDebug::DrawSphere()
	{
	}
}