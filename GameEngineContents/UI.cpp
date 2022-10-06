#include "PreCompile.h"
#include "UI.h"

UI::UI()
	: upperStandardPoint_(0, 380, 0),
	letterSize_(32.f),
	slugDurablitiy_EmptyRenderer_(nullptr),
	remainingRedeployUI_1_Renderer_(nullptr),
	remainingRedeployUI_U_Renderer_(nullptr),
	remainingRedeployUI_P_Renderer_(nullptr),
	remainingRedeployUI_EqualMarkRenderer_(nullptr),
	remainingRedeployCountRenderer_(nullptr),
	remainingRedeployUI_MulColor_(2.f, 2.f, 2.5f, 1.f),
	remainingRedeployUI_PlusColor_(-0.05f, -0.4f, -1.5f, 0.f),
	remainingRedelployCount_(2),
	score1stDigitRenderer_(nullptr),
	score2ndDigitRenderer_(nullptr),
	score3rdDigitRenderer_(nullptr),
	score4thDigitRenderer_(nullptr),
	score5thDigitRenderer_(nullptr),
	score_(0),
	munitionStatusRenderer_(nullptr),
	infinityMarkRenderer_(nullptr),
	bombCount1stDigitRenderer_(nullptr),
	bombCount2ndDigitRenderer_(nullptr),
	munitionStatusUI_MulColor_(1.9f, 1.8f, 1.0f, 1.f),
	munitionStatusUI_PlusColor_(-0.175f, -0.5f, -0.5f, 0.f),
	bombCount_(10),
	remainingTime1stDigitRenderer_(nullptr),
	remainingTime2ndDigitRenderer_(nullptr),
	insertCoin_I_Renderer_(nullptr),
	insertCoin_N_Renderer_(nullptr),
	insertCoin_S_Renderer_(nullptr),
	insertCoin_E_Renderer_(nullptr),
	insertCoin_R_Renderer_(nullptr),
	insertCoin_T_Renderer_(nullptr),
	insertCoin_C_Renderer_(nullptr),
	insertCoin_O_Renderer_(nullptr),
	insertCoin_I_Renderer2_(nullptr),
	insertCoin_N_Renderer2_(nullptr)
{
}

UI::~UI()
{
}

void UI::Start()
{
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);

	//Åº¾à ¼ýÀÚ »ö±òº¸Á¤°ª.
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
	remainingRedeployCountRenderer_->SetTexture("Numbers.png", remainingRedelployCount_);
	remainingRedeployCountRenderer_->SetPivot(PivotMode::Top);
	remainingRedeployCountRenderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployCountRenderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployCountRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	remainingRedeployCountRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 1.5f)),
		upperStandardPoint_.y - letterSize_,
		upperStandardPoint_.z
	);




	score1stDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("Score1stDigitRenderer");
	score1stDigitRenderer_->SetTexture("Numbers.png", score_ % 10);
	score1stDigitRenderer_->SetPivot(PivotMode::Bot);
	score1stDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	score1stDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 3.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	score2ndDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("Score2ndDigitRenderer");
	score2ndDigitRenderer_->SetTexture("Numbers.png", (score_ % 100) / 10);
	score2ndDigitRenderer_->SetPivot(PivotMode::Bot);
	score2ndDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	score2ndDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 2.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	score3rdDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("Score3rdDigitRenderer");
	score3rdDigitRenderer_->SetTexture("Numbers.png", (score_ % 1000) / 100);
	score3rdDigitRenderer_->SetPivot(PivotMode::Bot);
	score3rdDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	score3rdDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 1.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	score4thDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("Score4thDigitRenderer");
	score4thDigitRenderer_->SetTexture("Numbers.png", (score_ % 10000) / 1000);
	score4thDigitRenderer_->SetPivot(PivotMode::Bot);
	score4thDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	score4thDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f - (letterSize_ * 0.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	score5thDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("Score4thDigitRenderer");
	score5thDigitRenderer_->SetTexture("Numbers.png", (score_ % 100000) / 10000);
	score5thDigitRenderer_->SetPivot(PivotMode::Bot);
	score5thDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	score5thDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f + (letterSize_ * 0.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);


	munitionStatusRenderer_ = CreateComponent<GameEngineUIRenderer>("MunitionStatusRenderer");
	munitionStatusRenderer_->SetTexture("MunitionStatus.png");
	munitionStatusRenderer_->SetPivot(PivotMode::Center);
	munitionStatusRenderer_->GetTransform().SetLocalScale(248, 68, 1);
	munitionStatusRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - 200.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	infinityMarkRenderer_ = CreateComponent<GameEngineUIRenderer>("InfinityMarkRenderer");
	infinityMarkRenderer_->SetTexture("InfinityMark.png");
	infinityMarkRenderer_->SetPivot(PivotMode::Top);
	infinityMarkRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	infinityMarkRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	infinityMarkRenderer_->GetTransform().SetLocalScale(80, 32, 1);
	infinityMarkRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - 250.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	bombCount1stDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("BombCount1stDigitRenderer");
	bombCount1stDigitRenderer_->SetTexture("Numbers.png", bombCount_ % 10);
	bombCount1stDigitRenderer_->SetPivot(PivotMode::Top);
	bombCount1stDigitRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	bombCount1stDigitRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	bombCount1stDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	bombCount1stDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (135.f - (letterSize_ * 0.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	bombCount2ndDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("BombCount2ndDigitRenderer");
	bombCount2ndDigitRenderer_->SetTexture("Numbers.png", (bombCount_ % 100) / 10);
	bombCount2ndDigitRenderer_->SetPivot(PivotMode::Top);
	bombCount2ndDigitRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	bombCount2ndDigitRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	bombCount2ndDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	bombCount2ndDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (135.f + (letterSize_ * 0.5f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);
	
	if (0 == GameEngineTexture::Find("TimeNumbers.png")->GetCutCount())
	{
		GameEngineTexture::Cut("TimeNumbers.png", 10, 1);
	}


	remainingTime1stDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingTime1stDigitRenderer");
	remainingTime1stDigitRenderer_->SetTexture("TimeNumbers.png", 6);
	remainingTime1stDigitRenderer_->SetPivot(PivotMode::Center);
	remainingTime1stDigitRenderer_->GetTransform().SetLocalScale(letterSize_ * 2.f, letterSize_ * 2.f, 1.f);
	remainingTime1stDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - letterSize_,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	remainingTime2ndDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingTime2ndDigitRenderer");
	remainingTime2ndDigitRenderer_->SetTexture("TimeNumbers.png", 0);
	remainingTime2ndDigitRenderer_->SetPivot(PivotMode::Center);
	remainingTime2ndDigitRenderer_->GetTransform().SetLocalScale(letterSize_ * 2.f, letterSize_ * 2.f, 1.f);
	remainingTime2ndDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + letterSize_,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	if (0 == GameEngineTexture::Find("V_Stretched_CapitalLetters.png")->GetCutCount())
	{
		GameEngineTexture::Cut("V_Stretched_CapitalLetters.png", 26, 1);
	}


	insertCoin_I_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_I_Renderer");
	insertCoin_I_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'I' - 'A');
	insertCoin_I_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_I_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_I_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + 200.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_N_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_N_Renderer");
	insertCoin_N_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'N' - 'A');
	insertCoin_N_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_N_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_N_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + letterSize_),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_S_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_S_Renderer");
	insertCoin_S_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'S' - 'A');
	insertCoin_S_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_S_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_S_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 2.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_E_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_E_Renderer");
	insertCoin_E_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'E' - 'A');
	insertCoin_E_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_E_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_E_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 3.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_R_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_R_Renderer");
	insertCoin_R_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'R' - 'A');
	insertCoin_R_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_R_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_R_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 4.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_T_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_T_Renderer");
	insertCoin_T_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'T' - 'A');
	insertCoin_T_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_T_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_T_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 5.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_C_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_C_Renderer");
	insertCoin_C_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'C' - 'A');
	insertCoin_C_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_C_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_C_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 7.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_O_Renderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_O_Renderer");
	insertCoin_O_Renderer_->SetTexture("V_Stretched_CapitalLetters.png", 'O' - 'A');
	insertCoin_O_Renderer_->SetPivot(PivotMode::Center);
	insertCoin_O_Renderer_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_O_Renderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 8.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_I_Renderer2_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_I_Renderer2");
	insertCoin_I_Renderer2_->SetTexture("V_Stretched_CapitalLetters.png", 'I' - 'A');
	insertCoin_I_Renderer2_->SetPivot(PivotMode::Center);
	insertCoin_I_Renderer2_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_I_Renderer2_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 9.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	insertCoin_N_Renderer2_ = CreateComponent<GameEngineUIRenderer>("InsertCoin_N_Renderer2");
	insertCoin_N_Renderer2_->SetTexture("V_Stretched_CapitalLetters.png", 'N' - 'A');
	insertCoin_N_Renderer2_->SetPivot(PivotMode::Center);
	insertCoin_N_Renderer2_->GetTransform().SetLocalScale(letterSize_, letterSize_ * 2.f, 1.f);
	insertCoin_N_Renderer2_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 10.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);


}

void UI::Update(float _deltaTime)
{
}

void UI::End()
{
}
