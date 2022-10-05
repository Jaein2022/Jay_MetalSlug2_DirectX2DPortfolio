#include "PreCompile.h"
#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::Start()
{
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);


	capitalLetterRenderer_ = CreateComponent<GameEngineUIRenderer>("capitalLetterRenderer_");
	capitalLetterRenderer_->SetTexture("CapitalLetters.png");
	capitalLetterRenderer_->GetTransform().SetLocalScale(832, 32, 1);
	capitalLetterRenderer_->GetTransform().SetLocalPosition(0, 0, 0);

}

void UI::Update(float _deltaTime)
{
}

void UI::End()
{
}
