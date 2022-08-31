#include "PreCompile.h"
#include "TestPixelIndicator.h"

GameEngineTexture* TestPixelIndicator::pcTexture_ = nullptr;
const PixelColor TestPixelIndicator::magenta_ = PixelColor(255, 0, 255, 255);

TestPixelIndicator::TestPixelIndicator()
{
}

TestPixelIndicator::~TestPixelIndicator()
{
}

UINT TestPixelIndicator::GetColorValue_UINT()
{
	float4 thisPos(this->GetTransform().GetWorldPosition().x, this->GetTransform().GetWorldPosition().y);

	return pcTexture_->GetPixelToPixelColor(
		GameEngineWindow::GetScale().HIX() + thisPos.IX(),
		(GameEngineWindow::GetScale().IY() - (thisPos.IY() + GameEngineWindow::GetScale().HIY())
			- (GameEngineWindow::GetScale().IY() - pcTexture_->GetScale().IY()))
	).color_;
}
