#include "PreCompile.h"
#include "GameEngineStateManager.h"

GameEngineStateManager::GameEngineStateManager(): currentState_(nullptr)
{
}

GameEngineStateManager::~GameEngineStateManager()
{
}

void GameEngineStateManager::Update(float _deltaTime)
{
	currentState_->info_.stateTime_ += _deltaTime;
	currentState_->StateUpdate(_deltaTime);
}