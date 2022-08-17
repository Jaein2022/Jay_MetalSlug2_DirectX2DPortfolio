#pragma once

class GameEngineCamera;
class GameEngineTexture;
namespace GameEngineDebug
{

	void Debug3DInitialize();
	void Debug3DRender();//디버그 관련 정보를 실제로 렌더링하는 함수.

	void DrawBox(const GameEngineTransform& _transform, const float4& _color);
	void DrawBox(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color);
	
	void DrawSphere(const GameEngineTransform& _transform, const float4& _color);
	void DrawSphere(const GameEngineTransform& _transform, GameEngineCamera* _camera, const float4& _color);

	void DrawTexture(
		const std::string& _textureName,
		const float4& _position,
		const float4& _rotation = float4::Zero,
		const float4& _scale = float4::Zero);
	void DrawTexture(
		const std::string& _textureName,
		GameEngineCamera* _camera,
		const float4& _position,
		const float4& _rotation = float4::Zero,
		const float4& _scale = float4::Zero);

	void DrawTexture(
		GameEngineTexture* _texture,
		const float4& _position,
		const float4& _rotation = float4::Zero,
		const float4& _scale = float4::Zero);
	void DrawTexture(
		GameEngineTexture* _texture,
		GameEngineCamera* _camera,
		const float4& _position,
		const float4& _rotation = float4::Zero,
		const float4& _scale = float4::Zero);


}
