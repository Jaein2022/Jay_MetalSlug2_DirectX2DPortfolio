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
	std::string GetWindowTitle() override
	{
		return "MetalSlug2_DirectX2DPortFolio";
	}

	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:

};