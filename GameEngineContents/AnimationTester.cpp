#include "PreCompile.h"
#include "AnimationTester.h"

AnimationTester::AnimationTester(): legRenderer_(nullptr), velocity_(100.f), isMoving_(false)
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
	legRenderer_->GetTransform().SetLocalScale(150, 150, 1);

	if (0 == GameEngineTexture::Find("Tarma_LegAtlas.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Tarma_LegAtlas.png", 5, 6);
	}
	legRenderer_->SetTexture("Tarma_LegAtlas.png");

	FrameAnimation_Desc testAnimationDesc = FrameAnimation_Desc("Tarma_LegAtlas.png", 1, 12, 0.05f, true);

	legRenderer_->CreateFrameAnimation_AtlasTexture("LegTest", testAnimationDesc);
	legRenderer_->ChangeFrameAnimation("LegTest");
}

void AnimationTester::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsPressed("MoveLeft"))
	{
		this->GetTransform().SetWorldLeftMove(velocity_, _deltaTime);
		isMoving_ = true;
	}
	else if (true == GameEngineInput::GetInst()->IsPressed("MoveRight"))
	{
		this->GetTransform().SetWorldRightMove(velocity_, _deltaTime);
		isMoving_ = true;
	}
	else
	{
		isMoving_ = false;
	}


}

void AnimationTester::End()
{
}
