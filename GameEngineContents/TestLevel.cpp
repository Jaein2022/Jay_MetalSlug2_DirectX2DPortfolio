#include "PreCompile.h"
#include "TestLevel.h"
#include "Player.h"

TestLevel::TestLevel(): testPlayer_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	testPlayer_ = CreateActor<Player>(0, "TestPlayer");
}

void TestLevel::Update(float _deltaTime)
{
}

void TestLevel::End()
{
}
