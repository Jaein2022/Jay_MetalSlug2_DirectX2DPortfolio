#include "PreCompile.h"
#include "UI.h"

UI::UI()
	: upperStandardPoint_(0, 375, 0),
	letterSize_(32.f),
	slugDurablitiy_EmptyRenderer_(nullptr),
	remainingRedeployUI_1_Renderer_(nullptr),
	remainingRedeployUI_U_Renderer_(nullptr),
	remainingRedeployUI_P_Renderer_(nullptr),
	remainingRedeployUI_EqualMarkRenderer_(nullptr),
	remainingRedeployCountRenderer_(nullptr),
	remainingRedeployUI_MulColor_(2.f, 2.f, 2.5f, 1.f),
	remainingRedeployUI_PlusColor_(-0.05f, -0.4f, -1.5f, 0.f),
	letterRenderer_(nullptr)
{
}

UI::~UI()
{
}

void UI::Start()
{
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);

	//탄약 숫자 색깔보정값.
	//mulColor_ = float4(1.9f, 1.8f, 1.0f, 1.f);
	//plusColor_ = float4(-0.175f, -0.5f, -0.5f, 0.f);


	slugDurablitiy_EmptyRenderer_ = CreateComponent<GameEngineUIRenderer>("SlugDurability_EmptyRenderer");
	slugDurablitiy_EmptyRenderer_->SetTexture("UI_SlugDurability_Empty.png");
	slugDurablitiy_EmptyRenderer_->SetPivot(PivotMode::Top);
	slugDurablitiy_EmptyRenderer_->GetTransform().SetLocalScale(212, 32, 1);
	slugDurablitiy_EmptyRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - 460.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	if (0 == GameEngineTexture::Find("Numbers.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Numbers.png", 10, 1);
	}

	if (0 == GameEngineTexture::Find("CapitalLetters.png")->GetCutCount())
	{
		GameEngineTexture::Cut("CapitalLetters.png", 26, 1);
	}



	remainingRedeployUI_1_Renderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployUI_1_Renderer"); 
	remainingRedeployUI_1_Renderer_->SetTexture("Numbers.png", 1);
	remainingRedeployUI_1_Renderer_->SetPivot(PivotMode::Top);
	remainingRedeployUI_1_Renderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployUI_1_Renderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployUI_1_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployUI_1_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f + (letterSize_ * 2.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);

	remainingRedeployUI_U_Renderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployUI_U_Renderer"); 
	remainingRedeployUI_U_Renderer_->SetTexture("CapitalLetters.png", 'U' - 'A');
	remainingRedeployUI_U_Renderer_->SetPivot(PivotMode::Top);
	remainingRedeployUI_U_Renderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployUI_U_Renderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployUI_U_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployUI_U_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f + (letterSize_ * 1.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);

	remainingRedeployUI_P_Renderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployUI_P_Renderer"); 
	remainingRedeployUI_P_Renderer_->SetTexture("CapitalLetters.png", 'P' - 'A');
	remainingRedeployUI_P_Renderer_->SetPivot(PivotMode::Top);
	remainingRedeployUI_P_Renderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployUI_P_Renderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployUI_P_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployUI_P_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f + (letterSize_ * 0.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);

	remainingRedeployUI_EqualMarkRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployUI_EqualMarkRenderer"); 
	remainingRedeployUI_EqualMarkRenderer_->SetTexture("EqualMark.png");
	remainingRedeployUI_EqualMarkRenderer_->SetPivot(PivotMode::Top);
	remainingRedeployUI_EqualMarkRenderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployUI_EqualMarkRenderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployUI_EqualMarkRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployUI_EqualMarkRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 0.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);

	remainingRedeployCountRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployCountRenderer"); 
	remainingRedeployCountRenderer_->SetTexture("Numbers.png", 2);		//상수 2는 솔저 재투입 횟수 받아오는것으로 변경할 것.
	remainingRedeployCountRenderer_->SetPivot(PivotMode::Top);
	remainingRedeployCountRenderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployCountRenderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployCountRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployCountRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 1.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);




	letterRenderer_ = CreateComponent<GameEngineUIRenderer>("letterRenderer_");
	letterRenderer_->SetTexture("V_Stretched_CapitalLetters.png");
	letterRenderer_->GetTransform().SetLocalScale(832, 64, 1);
	letterRenderer_->GetTransform().SetLocalPosition(0, 0, 0);
	letterRenderer_->GetPixelData().mulColor_ = float4(2.f, 2.f, 2.5f, 1.f);
	letterRenderer_->GetPixelData().plusColor_ = float4(-0.05f, -0.4f, -1.5f, 0.f);




}

void UI::Update(float _deltaTime)
{
}

void UI::End()
{
}
