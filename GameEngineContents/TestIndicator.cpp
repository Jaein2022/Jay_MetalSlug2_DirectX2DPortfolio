#include "PreCompile.h"
#include "TestIndicator.h"

bool TestIndicator::isRendering_ = true;
GameEngineTexture* TestIndicator::pixelCollisionTexture_ = nullptr;

TestIndicator::TestIndicator()
{
}

TestIndicator::~TestIndicator()
{
}

TestIndicator* TestIndicator::CreateIndicator(
	const std::string& _name,
	GameEngineActor* _parentActor,
	const float4& _color,
	const float4& _localPos, 
	const float4& _localScale /*= float4(5, 5, 1)*/
)
{
	TestIndicator* newIndicator = new TestIndicator();
	newIndicator->SetName(_name);
	newIndicator->SetParent(_parentActor);
	newIndicator->GetTransform().SetLocalScale(_localScale);
	newIndicator->GetTransform().SetLocalPosition(_localPos);
	newIndicator->color_ = _color;
	newIndicator->Start();
	newIndicator->SetRenderingOrder(5);
	return newIndicator;
}

UINT TestIndicator::GetColorValue_UINT()
{
	float4 thisPos(this->GetTransform().GetWorldPosition().x, this->GetTransform().GetWorldPosition().y);

	return pixelCollisionTexture_->GetPixelToPixelColor(GameEngineWindow::GetScale().HIX() + thisPos.IX(),
		(GameEngineWindow::GetScale().IY() - (thisPos.IY() + GameEngineWindow::GetScale().HIY())
			- (GameEngineWindow::GetScale().IY() - pixelCollisionTexture_->GetScale().IY()))
	).color_;
}

void TestIndicator::Start()
{
	GameEngineRenderer::Start();
	SetPipeLine("Color");

	if (true == shaderResourceHelper_.IsConstantBuffer("ResultColor"))
	{
		this->shaderResourceHelper_.SetConstantBuffer_Link("ResultColor", this->color_);
		GameEngineRenderer::PushRendererToMainCamera();
	}
	else
	{
		MsgBoxAssert("ResultColor: 그런 이름의 상수 버퍼가 없습니다.");
		return;
	}
}

void TestIndicator::Update(float _deltaTime)
{
	float4 test = GetTransform().GetWorldPosition();

	if (0.f <= test.x)
	{
		int i = 0;
	}
}

void TestIndicator::End()
{
}

void TestIndicator::Render(float _deltaTime)
{
	if (true == isRendering_)
	{
		GameEngineDefaultRenderer::Render(_deltaTime);
	}
}
