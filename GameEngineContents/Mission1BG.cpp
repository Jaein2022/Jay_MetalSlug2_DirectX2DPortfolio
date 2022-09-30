#include "PreCompile.h"
#include "Mission1BG.h"

Mission1BG::Mission1BG()
	: part1_FirstBG_(nullptr),
	part1_FirstBG_InitPosition_(float4::Zero),
	part1_FirstBG_Footstep_(nullptr),
	part1_SecondBG_(nullptr),
	part1_ThirdBG_1_(nullptr),
	part1_ThirdBG_2_(nullptr),
	part1_Sign_(nullptr),
	part1_SteppableObject_(nullptr),
	skyBG_1_(nullptr),
	part2_FirstBG_(nullptr),
	part2_FirstBG_InitPosition_(4205, 61, -1),
	skyBG_2_(nullptr),
	part3_FirstBG_(nullptr),
	part3_FirstBG_InitPosition_(6000, 61, 0),
	skyBG_3_(nullptr)
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
	part1_FirstBG_->GetTransform().SetLocalPosition(part1_FirstBG_InitPosition_);

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


	skyBG_1_ = CreateComponent<GameEngineTextureRenderer>("SkyBG_1");
	skyBG_1_->SetTexture("Mission1_SkyBG.png");
	skyBG_1_->SetPivot(PivotMode::LeftBot);
	skyBG_1_->ScaleToTexture();
	skyBG_1_->GetTransform().SetLocalPosition(
		0,
		GameEngineWindow::GetScale().IY() - 384,
		15);


	part2_FirstBG_ = CreateComponent<GameEngineTextureRenderer>("Part2_FirstBG");
	part2_FirstBG_->SetTexture("Mission1_Part2_FirstBG.png");
	part2_FirstBG_->SetPivot(PivotMode::LeftBot);
	part2_FirstBG_->ScaleToTexture();
	part2_FirstBG_->GetTransform().SetLocalPosition(part2_FirstBG_InitPosition_);

	skyBG_2_ = CreateComponent<GameEngineTextureRenderer>("SkyBG_2");
	skyBG_2_->SetTexture("Mission1_SkyBG.png");
	skyBG_2_->SetPivot(PivotMode::LeftBot);
	skyBG_2_->ScaleToTexture();
	skyBG_2_->GetTransform().SetLocalPosition(
		3300,
		GameEngineWindow::GetScale().IY() - 289,
		17);


	part3_FirstBG_ = CreateComponent<GameEngineTextureRenderer>("Part3_FirstBG");
	part3_FirstBG_->SetTexture("Mission1_Part3_FirstBG.png");
	part3_FirstBG_->SetPivot(PivotMode::LeftBot);
	part3_FirstBG_->ScaleToTexture();
	part3_FirstBG_->GetTransform().SetLocalPosition(part3_FirstBG_InitPosition_);

	skyBG_3_ = CreateComponent<GameEngineTextureRenderer>("SkyBG_3");
	skyBG_3_->SetTexture("Mission1_SkyBG.png");
	skyBG_3_->SetPivot(PivotMode::LeftBot);
	skyBG_3_->ScaleToTexture();
	skyBG_3_->GetTransform().SetLocalPosition(
		6800,
		GameEngineWindow::GetScale().IY() - 289,	//모스크포대 배치한 후에 하늘배경 3번 y좌표 재조정.
		17);
}

void Mission1BG::Update(float _deltaTime)
{
}

void Mission1BG::End()
{
}
