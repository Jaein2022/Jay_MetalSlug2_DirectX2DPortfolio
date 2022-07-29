#include "PreCompile.h"
#include "GEngine.h"

GEngine::GEngine()
{
}

GEngine::~GEngine()
{
}

void GEngine::ChangeLevel(const std::string& _levelName)
{
	GameEngineCore::ChangeLevel(_levelName);
}
