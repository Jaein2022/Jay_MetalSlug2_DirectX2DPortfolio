#include "PreCompile.h"
#include "TestPointer.h"

bool TestPointer::isRendering_ = true;
GameEngineTexture* TestPointer::pixelCollisionTexture_ = nullptr;

TestPointer::TestPointer()
{
}

TestPointer::~TestPointer()
{
}

UINT TestPointer::GetColorValue_UINT()
{
	float4 thisPos(this->GetTransform().GetWorldPosition().x, this->GetTransform().GetWorldPosition().y);

	return pixelCollisionTexture_->GetPixelToPixelColor(
		GameEngineWindow::GetScale().HIX() + thisPos.IX(),
		(GameEngineWindow::GetScale().IY() - (thisPos.IY() + GameEngineWindow::GetScale().HIY())
			- (GameEngineWindow::GetScale().IY() - pixelCollisionTexture_->GetScale().IY()))
	).color_;
}

void TestPointer::Start()
{
	//GameEngineRenderer::Start();

	SetPipeLine("Color");

	if (true == shaderResourceHelper_.IsConstantBuffer("ResultColor"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link("ResultColor", this->color_);
		GameEngineRenderer::PushRendererToMainCamera();
		//나중에 제3의 카메라에 등록해볼 것.
	}
	else
	{
		MsgBoxAssert("ResultColor: 그런 이름의 상수 버퍼가 없습니다.");
		return;
	}
}

void TestPointer::Update(float _deltaTime)
{


}

void TestPointer::End()
{
}

void TestPointer::Render(float _deltaTime)
{
	if (true == isRendering_)
	{
		GameEngineDefaultRenderer::Render(_deltaTime);
	}
}
