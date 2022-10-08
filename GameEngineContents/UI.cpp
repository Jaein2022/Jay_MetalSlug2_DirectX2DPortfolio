#include "PreCompile.h"
#include "UI.h"

UI::UI()
	: upperStandardPoint_(0, 380, 0),
	letterSize_(32.f),
	slugDurablitiy_EmptyRenderer_(nullptr),
	remainingRedeployUIRenderer_(nullptr),
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
	munitionStatusWindowRenderer_(nullptr),
	bulletCount1stDigitRenderer_(nullptr),
	bulletCount2ndDigitRenderer_(nullptr),
	bulletCount3rdDigitRenderer_(nullptr),
	infinityMarkRenderer_(nullptr),
	bulletCount_(15),
	bombCount1stDigitRenderer_(nullptr),
	bombCount2ndDigitRenderer_(nullptr),
	munitionStatusUI_MulColor_(1.9f, 1.8f, 1.0f, 1.f),
	munitionStatusUI_PlusColor_(-0.175f, -0.5f, -0.5f, 0.f),
	bombCount_(15),
	remainingPlayTime1stDigitRenderer_(nullptr),
	remainingPlayTime2ndDigitRenderer_(nullptr),
	remainingPlayTime_(55),
	insertCoinRenderer_(nullptr),
	flickeringInterval_InsertCoin_On_(3.f),
	flickeringInterval_InsertCoin_Off_(1.f),
	remainingFlickeringTime_(3.f),
	credits__0Renderer_(nullptr),
	creditsRenderer_(nullptr),
	creditsCountRenderer_(nullptr),
	creditsCount_(0)
{
}

UI::~UI()
{
}

void UI::Start()
{
	this->GetTransform().SetWorldScale(1, 1, 1);
	this->GetTransform().SetLocalScale(1, 1, 1);


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


	remainingRedeployUIRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingRedeployUIRenderer"); 
	remainingRedeployUIRenderer_->SetTexture("1UP=.png");
	remainingRedeployUIRenderer_->SetPivot(PivotMode::Top);
	remainingRedeployUIRenderer_->GetPixelData().mulColor_ = remainingRedeployUI_MulColor_;
	remainingRedeployUIRenderer_->GetPixelData().plusColor_ = remainingRedeployUI_PlusColor_;
	remainingRedeployUIRenderer_->GetTransform().SetLocalScale(letterSize_ * 4.f, letterSize_, 1.f);
	remainingRedeployUIRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (460.f + letterSize_) ,
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

	score1stDigitRenderer_->Off();
	score2ndDigitRenderer_->Off();
	score3rdDigitRenderer_->Off();
	score4thDigitRenderer_->Off();
	score5thDigitRenderer_->Off();



	munitionStatusWindowRenderer_ = CreateComponent<GameEngineUIRenderer>("MunitionStatusWindowRenderer");
	munitionStatusWindowRenderer_->SetTexture("MunitionStatus.png");
	munitionStatusWindowRenderer_->SetPivot(PivotMode::Center);
	munitionStatusWindowRenderer_->GetTransform().SetLocalScale(248, 68, 1);
	munitionStatusWindowRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - 200.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);


	bulletCount1stDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("");
	bulletCount1stDigitRenderer_->SetTexture("Numbers.png", bulletCount_ % 10);
	bulletCount1stDigitRenderer_->SetPivot(PivotMode::Top);
	bulletCount1stDigitRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	bulletCount1stDigitRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	bulletCount1stDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	bulletCount1stDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (250.f - letterSize_),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	bulletCount2ndDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("");
	bulletCount2ndDigitRenderer_->SetTexture("Numbers.png", (bulletCount_ % 100) / 10);
	bulletCount2ndDigitRenderer_->SetPivot(PivotMode::Top);
	bulletCount2ndDigitRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	bulletCount2ndDigitRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	bulletCount2ndDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	bulletCount2ndDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - 250.f,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	bulletCount3rdDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("");
	bulletCount3rdDigitRenderer_->SetTexture("Numbers.png", (bulletCount_ % 1000) / 100);
	bulletCount3rdDigitRenderer_->SetPivot(PivotMode::Top);
	bulletCount3rdDigitRenderer_->GetPixelData().mulColor_ = munitionStatusUI_MulColor_;
	bulletCount3rdDigitRenderer_->GetPixelData().plusColor_ = munitionStatusUI_PlusColor_;
	bulletCount3rdDigitRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	bulletCount3rdDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - (250.f + letterSize_),
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
	infinityMarkRenderer_->Off();




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
	bombCount2ndDigitRenderer_->Off();
	
	if (0 == GameEngineTexture::Find("TimeNumbers.png")->GetCutCount())
	{
		GameEngineTexture::Cut("TimeNumbers.png", 10, 1);
	}


	remainingPlayTime1stDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingTime1stDigitRenderer");
	remainingPlayTime1stDigitRenderer_->SetTexture("TimeNumbers.png", remainingPlayTime_ % 10);
	remainingPlayTime1stDigitRenderer_->SetPivot(PivotMode::Center);
	remainingPlayTime1stDigitRenderer_->GetTransform().SetLocalScale(letterSize_ * 2.f, letterSize_ * 2.f, 1.f);
	remainingPlayTime1stDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x - letterSize_,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);
	remainingPlayTime1stDigitRenderer_->Off();

	remainingPlayTime2ndDigitRenderer_ = CreateComponent<GameEngineUIRenderer>("RemainingTime2ndDigitRenderer");
	remainingPlayTime2ndDigitRenderer_->SetTexture("TimeNumbers.png", remainingPlayTime_ % 100 / 10);
	remainingPlayTime2ndDigitRenderer_->SetPivot(PivotMode::Center);
	remainingPlayTime2ndDigitRenderer_->GetTransform().SetLocalScale(letterSize_ * 2.f, letterSize_ * 2.f, 1.f);
	remainingPlayTime2ndDigitRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + letterSize_,
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);
	remainingPlayTime2ndDigitRenderer_->Off();

	insertCoinRenderer_ = CreateComponent<GameEngineUIRenderer>("InsertCoinRenderer");
	insertCoinRenderer_->SetTexture("InsertCoin.png");
	insertCoinRenderer_->SetPivot(PivotMode::Center);
	insertCoinRenderer_->GetTransform().SetLocalScale(letterSize_ * 11.f, letterSize_ * 2.f, 1.f);
	insertCoinRenderer_->GetTransform().SetLocalPosition(
		upperStandardPoint_.x + (200.f + (letterSize_ * 5.f)),
		upperStandardPoint_.y,
		upperStandardPoint_.z
	);

	credits__0Renderer_ = CreateComponent<GameEngineUIRenderer>("Credits__0Renderer");
	credits__0Renderer_->SetTexture("Credits__0.png");
	credits__0Renderer_->SetPivot(PivotMode::Center);
	credits__0Renderer_->GetTransform().SetLocalScale(letterSize_ * 9.f, letterSize_, 1.f);
	credits__0Renderer_->GetTransform().SetLocalPosition(
		200.f + (letterSize_ * 5.f),
		-GameEngineWindow::GetScale().HY() + (letterSize_ * 0.5f),
		0.f
	);

	creditsRenderer_ = CreateComponent<GameEngineUIRenderer>("CreditsRenderer");
	creditsRenderer_->SetTexture("Credits.png");
	creditsRenderer_->SetPivot(PivotMode::Center);
	creditsRenderer_->GetTransform().SetLocalScale(letterSize_ * 6.f, letterSize_, 1.f);
	creditsRenderer_->GetTransform().SetLocalPosition(
		-400.f,
		-GameEngineWindow::GetScale().HY() + (letterSize_ * 0.5f),
		0.f
	);

	creditsCountRenderer_ = CreateComponent<GameEngineUIRenderer>("CreditsCountRenderer");
	creditsCountRenderer_->SetTexture("Numbers.png", creditsCount_);
	creditsCountRenderer_->SetPivot(PivotMode::Center);
	creditsCountRenderer_->GetTransform().SetLocalScale(letterSize_, letterSize_, 1.f);
	creditsCountRenderer_->GetTransform().SetLocalPosition(
		-(400.f - (letterSize_ * 5.f)),
		-GameEngineWindow::GetScale().HY() + (letterSize_ * 0.5f),
		0.f
	);
}

void UI::Update(float _deltaTime)
{
	Flicker_InsertCoinRenderer(_deltaTime);

	UpdateUIRenderers();


}

void UI::End()
{
}

void UI::Flicker_InsertCoinRenderer(float _deltaTime)
{
	if (0 >= remainingFlickeringTime_)
	{
		insertCoinRenderer_->OnOffSwitch();

		if (true == insertCoinRenderer_->IsUpdate())
		{
			remainingFlickeringTime_ = flickeringInterval_InsertCoin_On_;
		}
		else
		{
			remainingFlickeringTime_ = flickeringInterval_InsertCoin_Off_;
		}
	}
	else
	{
		remainingFlickeringTime_ -= _deltaTime;
	}
}

void UI::UpdateUIRenderers()
{


	if (10 > score_)
	{
		score1stDigitRenderer_->On();
		score2ndDigitRenderer_->Off();
		score3rdDigitRenderer_->Off();
		score4thDigitRenderer_->Off();
		score5thDigitRenderer_->Off();
	}
	else if (10 <= score_ && 100 > score_)
	{
		score1stDigitRenderer_->On();
		score2ndDigitRenderer_->On();
		score3rdDigitRenderer_->Off();
		score4thDigitRenderer_->Off();
		score5thDigitRenderer_->Off();
	}	
	else if (100 <= score_ && 1000 > score_)
	{
		score1stDigitRenderer_->On();
		score2ndDigitRenderer_->On();
		score3rdDigitRenderer_->On();
		score4thDigitRenderer_->Off();
		score5thDigitRenderer_->Off();
	}	
	else if (1000 <= score_ && 10000 > score_)
	{
		score1stDigitRenderer_->On();
		score2ndDigitRenderer_->On();
		score3rdDigitRenderer_->On();
		score4thDigitRenderer_->On();
		score5thDigitRenderer_->Off();
	}	
	else if (10000 <= score_ && 100000 > score_)
	{
		score1stDigitRenderer_->On();
		score2ndDigitRenderer_->On();
		score3rdDigitRenderer_->On();
		score4thDigitRenderer_->On();
		score5thDigitRenderer_->On();
	}
	else
	{
		MsgBoxAssert("더이상의 점수를 표시할 수 없습니다. 추가 스코어렌더러를 만드세요.");
		return;
	}



	if (10 > remainingPlayTime_)
	{
		remainingPlayTime1stDigitRenderer_->On();
		remainingPlayTime2ndDigitRenderer_->Off();
	}
	else if (10 <= remainingPlayTime_ && 100 > remainingPlayTime_)
	{
		remainingPlayTime1stDigitRenderer_->On();
		remainingPlayTime2ndDigitRenderer_->On();
	}
	else
	{
		MsgBoxAssert("남은 플레이시간이 뭔가 이상합니다. 확인해보세요.");
		return;
	}

	if (10 > bulletCount_)
	{
		bulletCount1stDigitRenderer_->On();
		bulletCount2ndDigitRenderer_->Off();
		bulletCount3rdDigitRenderer_->Off();
	}
	else if (10 <= bulletCount_ && 100 > bulletCount_)
	{
		bulletCount1stDigitRenderer_->On();
		bulletCount2ndDigitRenderer_->On();
		bulletCount3rdDigitRenderer_->Off();
	}
	else if (100 <= bulletCount_ && 1000 > bulletCount_)
	{
		bulletCount1stDigitRenderer_->On();
		bulletCount2ndDigitRenderer_->On();
		bulletCount3rdDigitRenderer_->On();
	}
	else
	{
		MsgBoxAssert("남은 총알 수가 뭔가 이상합니다. 확인해보세요.");
		return;
	}

	if (10 > bombCount_)
	{
		bombCount1stDigitRenderer_->On();
		bombCount2ndDigitRenderer_->Off();
	}
	else if (10 <= bombCount_ && 100 > bombCount_)
	{
		bombCount1stDigitRenderer_->On();
		bombCount2ndDigitRenderer_->On();
	}
	else
	{
		MsgBoxAssert("폭탄 숫자가 뭔가 잘못되었습니다. 확인해보세요.");
		return;
	}
}
