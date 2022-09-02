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

	//firstBGRenderer_->SetPivot(PivotMode::Custom);
	//firstBGRenderer_->GetTransform().SetLocalPosition(
	//	float4(
	//		firstBGRenderer_->GetTransform().GetLocalScale().HX(),
	//		firstBGRenderer_->GetTransform().GetLocalScale().HY(),
	//		0.f
	//	)
	//);

	pixelCollisionBGRenderer_ = CreateComponent<GameEngineTextureRenderer>("PixelCollisionBGRenderer");
	pixelCollisionBGRenderer_->SetTexture("TestBG_PC.png");
	pixelCollisionBGRenderer_->ScaleToTexture();
	pixelCollisionBGRenderer_->GetTransform().SetLocalPosition(float4(0, 0, 10));
	pixelCollisionBGRenderer_->SetPivot(PivotMode::LeftBot);
	//pixelCollisionBGRenderer_->SetRenderingOrder(2);
	//pixelCollisionBGRenderer_->DebugOn();
	pixelCollisionBGRenderer_->Off();		//이렇게 해야 콜리전들이 보임. 이유는 불명.
	
	//pixelCollisionBGRenderer_->SetPivot(PivotMode::Custom);
	//pixelCollisionBGRenderer_->GetTransform().SetLocalPosition(
	//	float4(
	//		pixelCollisionBGRenderer_->GetTransform().GetWorldScale().HX(),
	//		pixelCollisionBGRenderer_->GetTransform().GetWorldScale().HY(),
	//		30.f
	//	)
	//);

	

}

void TestBackground::Update(float _deltaTime)
{
}

void TestBackground::End()
{
}
