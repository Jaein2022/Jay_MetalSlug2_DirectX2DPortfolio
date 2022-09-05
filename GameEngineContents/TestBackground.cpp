#include "PreCompile.h"
#include "TestBackground.h"

TestBackground::TestBackground(): firstBGRenderer_(nullptr), pixelCollisionBGRenderer_(nullptr)
{
}

TestBackground::~TestBackground()
{	
}

void TestBackground::Start()
{
	this->GetTransform().SetWorldPosition(
		-GameEngineWindow::GetScale().HX(),
		-GameEngineWindow::GetScale().HY(),
		40.f);
	//윈도우 왼쪽 아래를 배경액터 위치로 설정.

	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);

	firstBGRenderer_ = CreateComponent<GameEngineTextureRenderer>("FirstBGRenderer");
	firstBGRenderer_->SetTexture("TestBG.png");
	firstBGRenderer_->ScaleToTexture();
	firstBGRenderer_->GetTransform().SetLocalPosition(float4::Zero);
	firstBGRenderer_->SetPivot(PivotMode::LeftBot);
	//firstBGRenderer_->Off();

	pixelCollisionBGRenderer_ = CreateComponent<GameEngineTextureRenderer>("PixelCollisionBGRenderer");
	pixelCollisionBGRenderer_->SetTexture("TestBG_PC.png");
	pixelCollisionBGRenderer_->ScaleToTexture();
	pixelCollisionBGRenderer_->GetTransform().SetLocalPosition(float4(0, 0, 10));
	pixelCollisionBGRenderer_->SetPivot(PivotMode::LeftBot);
	//pixelCollisionBGRenderer_->SetRenderingOrder(2);
	//pixelCollisionBGRenderer_->DebugOn();
	//pixelCollisionBGRenderer_->Off();		
	

	

}

void TestBackground::Update(float _deltaTime)
{
}

void TestBackground::End()
{
}
