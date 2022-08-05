#include "PreCompile.h"
#include "GameEngineFontRenderer.h"
#include "GameEngineFont.h"
#include "GameEngineRenderTarget.h"

GameEngineRenderTarget* GameEngineFontRenderer::fontTarget_ = nullptr;

GameEngineFontRenderer::GameEngineFontRenderer()
	:font_(nullptr),
	fontSize_(20.f),
	fontColor_(float4::White),
	screenPosition_(float4::Zero)
{
}

GameEngineFontRenderer::~GameEngineFontRenderer()
{
}

void GameEngineFontRenderer::SetText(const std::string& _text, const std::string& _font /*= "돋움"*/)
{
	text_ = _text;
	font_ = GameEngineFont::Find(_font);

	if (nullptr == font_)
	{
		MsgBoxAssertString(_font + ": 그런 이름의 폰트가 없습니다.");
		return;
	}
}

void GameEngineFontRenderer::Start()
{
	if (nullptr == fontTarget_)
	{
		fontTarget_ = GameEngineRenderTarget::Create("Font Target");
		fontTarget_->CreateRenderTargetTexture(GameEngineWindow::GetScale(), float4::Red);
	}

	PushRendererToMainCamera();
}

void GameEngineFontRenderer::Render(float _deltaTime)
{
	if (nullptr == font_)
	{
		return;
	}

	font_->FontDraw(text_, fontSize_, screenPosition_, fontColor_);
	GameEngineRenderingPipeLine::AllShaderReset();
}