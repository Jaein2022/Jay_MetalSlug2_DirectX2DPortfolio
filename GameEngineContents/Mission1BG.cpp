#include "PreCompile.h"
#include "Mission1BG.h"

Mission1BG::Mission1BG()
	: part1_FirstBG_(nullptr),
	part1_FirstBG_Footstep_(nullptr),
	part1_SecondBG_(nullptr),
	part1_ThirdBG_1_(nullptr),
	part1_ThirdBG_2_(nullptr),
	part1_FourthBG_(nullptr),
	part1_Sign_(nullptr),
	part1_SteppableObject_(nullptr)
{
}

Mission1BG::~Mission1BG()
{
}

void Mission1BG::Start()
{
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);

	part1_FirstBG_ = CreateComponent<GameEngineTextureRenderer>("Part1_FirstBG");
	part1_FirstBG_->SetTexture("Mission1_Part1_FirstBG.png");
	part1_FirstBG_->SetPivot(PivotMode::LeftBot);
	part1_FirstBG_->ScaleToTexture();
	part1_FirstBG_->GetTransform().SetLocalPosition(float4::Zero);

	//part1_FirstBG_Footstep_ = CreateComponent<GameEngineTextureRenderer>("Part1_FirstBG_Footstep");
	//part1_FirstBG_Footstep_->SetTexture("Mission1_Part1_FirstBG_Footstep.png");
	//part1_FirstBG_Footstep_->SetPivot(PivotMode::LeftBot);
	//part1_FirstBG_Footstep_->ScaleToTexture();
	//part1_FirstBG_Footstep_->GetTransform().SetLocalPosition(float4::Zero);

	part1_SecondBG_ = CreateComponent<GameEngineTextureRenderer>("Part1_SecondBG");
	part1_SecondBG_->SetTexture("Mission1_Part1_SecondBG.png");
	part1_SecondBG_->SetPivot(PivotMode::LeftBot);
	part1_SecondBG_->ScaleToTexture();
	part1_SecondBG_->GetTransform().SetLocalPosition(0, 250, 5);

	part1_ThirdBG_1_ = CreateComponent<GameEngineTextureRenderer>("Part1_ThirdBG_1");
	part1_ThirdBG_1_->SetTexture("Mission1_Part1_ThirdBG.png");
	part1_ThirdBG_1_->SetPivot(PivotMode::LeftBot);
	part1_ThirdBG_1_->ScaleToTexture();
	part1_ThirdBG_1_->GetTransform().SetLocalPosition(0, 445, 10);

	part1_ThirdBG_2_ = CreateComponent<GameEngineTextureRenderer>("Part1_ThirdBG_2");
	part1_ThirdBG_2_->SetTexture("Mission1_Part1_ThirdBG.png");
	part1_ThirdBG_2_->SetPivot(PivotMode::LeftBot);
	part1_ThirdBG_2_->ScaleToTexture();
	part1_ThirdBG_2_->GetTransform().SetLocalPosition(2000, 445, 10);

	part1_FourthBG_ = CreateComponent<GameEngineTextureRenderer>("Part1_FourthBG");
	part1_FourthBG_->SetTexture("Mission1_Part1_FourthBG.png");	
	part1_FourthBG_->SetPivot(PivotMode::LeftBot);
	part1_FourthBG_->ScaleToTexture();
	part1_FourthBG_->GetTransform().SetLocalPosition(
		0,
		GameEngineWindow::GetScale().IY() - 384,
		20);


	part1_Sign_ = CreateComponent<GameEngineTextureRenderer>("Part1_Sign");
	part1_Sign_->SetTexture("Mission1_Part1_Sign.png");
	part1_Sign_->SetPivot(PivotMode::LeftBot);
	part1_Sign_->ScaleToTexture();
	part1_Sign_->GetTransform().SetLocalPosition(2800, 545, -5);


	part1_SteppableObject_ = CreateComponent<GameEngineTextureRenderer>("Part1_SteppableObject");
	part1_SteppableObject_->SetTexture("Mission1_Part1_SteppableObject.png");
	part1_SteppableObject_->SetPivot(PivotMode::LeftBot);
	part1_SteppableObject_->ScaleToTexture();
	part1_SteppableObject_->GetTransform().SetLocalPosition(2200, 170, -5);


}

void Mission1BG::Update(float _deltaTime)
{
}

void Mission1BG::End()
{
}
