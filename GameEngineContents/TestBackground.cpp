#include "PreCompile.h"
#include "TestBackground.h"

TestBackground::TestBackground(): firstBGRenderer_(nullptr), pixelCollisionBGRenderer_(nullptr)
{
}

TestBackground::~TestBackground()
{	
	//CreateActor()�� ���� �̻� ������ �ڵ����� �̷������.
}

void TestBackground::Start()
{
	this->GetTransform().SetWorldPosition(
		-GameEngineWindow::GetScale().HX(),
		-GameEngineWindow::GetScale().HY(),
		20.f);
	//������ ���� �Ʒ��� ������ ��ġ�� ����.


	//�׽�ƮBG ���� ũ��: 1176x224.

	firstBGRenderer_ = CreateComponent<GameEngineTextureRenderer>("BaseBGRenderer");
	firstBGRenderer_->SetTexture("TestBg.png");
	firstBGRenderer_->ScaleToTexture();
	firstBGRenderer_->SetPivot(PivotMode::Custom);
	firstBGRenderer_->GetTransform().SetLocalPosition(
		float4(
			firstBGRenderer_->GetTransform().GetWorldScale().HX(),
			firstBGRenderer_->GetTransform().GetWorldScale().HY(),
			0.f
		)
	);
	firstBGRenderer_->Off();

	pixelCollisionBGRenderer_ = CreateComponent<GameEngineTextureRenderer>("PixelCollisionBGRenderer");
	pixelCollisionBGRenderer_->SetTexture("TestBg_PixelCollision.png");
	pixelCollisionBGRenderer_->ScaleToTexture();
	pixelCollisionBGRenderer_->SetPivot(PivotMode::Custom);
	pixelCollisionBGRenderer_->GetTransform().SetLocalPosition(
		float4(
			pixelCollisionBGRenderer_->GetTransform().GetWorldScale().HX(),
			pixelCollisionBGRenderer_->GetTransform().GetWorldScale().HY(),
			30.f
		)
	);

	

}

void TestBackground::Update(float _deltaTime)
{
}

void TestBackground::End()
{
}
