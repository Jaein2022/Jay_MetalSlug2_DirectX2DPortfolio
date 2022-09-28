#include "PreCompile.h"
#include "PixelIndicator.h"

GameEngineTexture* PixelIndicator::pcTexture_ = nullptr;

PixelIndicator::PixelIndicator()
{
}

PixelIndicator::~PixelIndicator()
{
}

UINT PixelIndicator::GetColorValue_UINT()
{
	float4 thisPos(this->GetTransform().GetWorldPosition().x, this->GetTransform().GetWorldPosition().y);

	return pcTexture_->GetPixelToPixelColor(
		GameEngineWindow::GetScale().HIX() + thisPos.IX(),
		(GameEngineWindow::GetScale().IY() - (thisPos.IY() + GameEngineWindow::GetScale().HIY())
			- (GameEngineWindow::GetScale().IY() - pcTexture_->GetScale().IY()))
	).color_;
}
