#pragma once
#include "GameEngineCamera.h"

namespace GameEngineDebug
{

	void Debug3DInitialize();
	void Debug3DRender();

	void DrawBox(const GameEngineTransform& _transform, const float4& _color);
	void DrawBox(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color);
	void DrawSphere();
}
