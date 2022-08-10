#pragma once
#include "GameEngineCamera.h"

namespace GameEngineDebug
{

	void Debug3DInitialize();
	void Debug3DRender();//����� ���� ������ ������ �������ϴ� �Լ�.

	void DrawBox(const GameEngineTransform& _transform, const float4& _color);
	void DrawBox(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color);
	
	void DrawSphere(const GameEngineTransform& _transform, const float4& _color);
	void DrawSphere(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color);

	void DrawTexture(
		const std::string& _textureName,
		const float4& _position,
		const float4& _rotation,
		const float4& _scale = float4::Zero);
	void DrawTexture(
		const std::string& _textureName,
		GameEngineCamera* _camera,
		const float4& _position,
		const float4& _rotation,
		const float4& _scale = float4::Zero);


}