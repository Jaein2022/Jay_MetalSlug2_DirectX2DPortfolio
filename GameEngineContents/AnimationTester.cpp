#include "PreCompile.h"
#include "AnimationTester.h"

AnimationTester::AnimationTester(): legRenderer_(nullptr), velocity_(100.f)
{
}

AnimationTester::~AnimationTester()
{
}

void AnimationTester::Start()
{

	this->GetTransform().SetWorldPosition(0, 0, 0);


	if (false == GameEngineInput::GetInst()->IsKey("MoveLeft"))
	{
		GameEngineInput::GetInst()->CreateKey("MoveLeft", 'J');
		GameEngineInput::GetInst()->CreateKey("MoveRight", 'L');
		GameEngineInput::GetInst()->CreateKey("LookUp", 'I');
		GameEngineInput::GetInst()->CreateKey("LookDown", 'K');

		GameEngineInput::GetInst()->CreateKey("Attack", 'X');
		GameEngineInput::GetInst()->CreateKey("Jump", 'Z');
		GameEngineInput::GetInst()->CreateKey("Special", 'S');
	}

	legRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	legRenderer_->GetTransform().SetLocalScale(100, 100, 1);




}

void AnimationTester::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPressed("MoveLeft"))
	{
		this->GetTransform().SetWorldLeftMove(velocity_, _deltaTime);
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("MoveRight"))
	{
		this->GetTransform().SetWorldRightMove(velocity_, _deltaTime);
	}


}

void AnimationTester::End()
{
}
