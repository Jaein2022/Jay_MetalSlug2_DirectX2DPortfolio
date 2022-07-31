#include "PreCompile.h"
#include "TestLevel.h"
#include "AnimationTester.h"

TestLevel::TestLevel(): tester_(nullptr)
{
}

TestLevel::~TestLevel()
{
}

void TestLevel::Start()
{
	tester_ = CreateActor<AnimationTester>(0, "Tester");
}

void TestLevel::Update(float _deltaTime)
{
}

void TestLevel::End()
{
}
