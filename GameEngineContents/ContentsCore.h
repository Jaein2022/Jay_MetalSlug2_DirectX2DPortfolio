#pragma once
#include <GameEngineCore\CoreMinimal.h>

class ContentsCore : public GameEngineCore
{

public:

	ContentsCore();
	~ContentsCore();

	ContentsCore(const ContentsCore& _other) = delete;
	ContentsCore(ContentsCore&& _other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _other) = delete;
	ContentsCore& operator=(ContentsCore&& _other) = delete;

public:

public:
	float4 StartWindowSize() override { return float4{ 1280, 720 }; };
	float4 StartWindowPosition() override { return { 100, 100 }; };

	std::string GetWindowTitle() override
	{
		return "Jay_MetalSlug2_DirectX2DPortFolio";
	}

	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:
	 

private:
	void LoadContentsResource();	//길어지면 전용 cpp파일로 분리.

};