#pragma once

class Mission1;
class UI: public GameEngineActor
{
	//이 클래스의 존재 이유: UI
public:
	UI();
	~UI();

protected:
	UI(const UI& _other) = delete;
	UI(UI&& _other) noexcept = delete;

private:
	UI& operator=(const UI& _other) = delete;
	UI& operator=(const UI&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;

private:
	void Flicker_InsertCoinRenderer(float _deltaTime);
	void UpdateUIRenderers();

private:
	const float4 upperStandardPoint_;
	const float letterSize_;

	GameEngineUIRenderer* slugDurablitiy_EmptyRenderer_;
	GameEngineUIRenderer* remainingRedeployUIRenderer_;
	GameEngineUIRenderer* remainingRedeployCountRenderer_;
	const float4 remainingRedeployUI_MulColor_;
	const float4 remainingRedeployUI_PlusColor_; 
	int remainingRedelployCount_;

	GameEngineUIRenderer* score1stDigitRenderer_;
	GameEngineUIRenderer* score2ndDigitRenderer_;
	GameEngineUIRenderer* score3rdDigitRenderer_;
	GameEngineUIRenderer* score4thDigitRenderer_;
	GameEngineUIRenderer* score5thDigitRenderer_;
	int score_;

	GameEngineUIRenderer* munitionStatusWindowRenderer_;

	GameEngineUIRenderer* bulletCount1stDigitRenderer_;
	GameEngineUIRenderer* bulletCount2ndDigitRenderer_;
	GameEngineUIRenderer* bulletCount3rdDigitRenderer_;

	GameEngineUIRenderer* infinityMarkRenderer_;
	int bulletCount_;




	GameEngineUIRenderer* bombCount1stDigitRenderer_;
	GameEngineUIRenderer* bombCount2ndDigitRenderer_;
	const float4 munitionStatusUI_MulColor_;
	const float4 munitionStatusUI_PlusColor_;
	int bombCount_;


	GameEngineUIRenderer* remainingPlayTime1stDigitRenderer_;
	GameEngineUIRenderer* remainingPlayTime2ndDigitRenderer_;
	int remainingPlayTime_;

	GameEngineUIRenderer* insertCoinRenderer_;
	const float flickeringInterval_InsertCoin_On_;
	const float flickeringInterval_InsertCoin_Off_;
	float remainingFlickeringTime_;

	GameEngineUIRenderer* credits__0Renderer_;
	GameEngineUIRenderer* creditsRenderer_;
	GameEngineUIRenderer* creditsCountRenderer_;
	int creditsCount_;
};

