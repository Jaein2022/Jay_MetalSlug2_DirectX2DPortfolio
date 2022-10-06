#pragma once

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
	const float4 upperStandardPoint_;
	const float letterSize_;

	GameEngineUIRenderer* slugDurablitiy_EmptyRenderer_;
	
	GameEngineUIRenderer* remainingRedeployUI_1_Renderer_;
	GameEngineUIRenderer* remainingRedeployUI_U_Renderer_;
	GameEngineUIRenderer* remainingRedeployUI_P_Renderer_;
	GameEngineUIRenderer* remainingRedeployUI_EqualMarkRenderer_;
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

	GameEngineUIRenderer* munitionStatusRenderer_;
	GameEngineUIRenderer* infinityMarkRenderer_;
	GameEngineUIRenderer* bombCount1stDigitRenderer_;
	GameEngineUIRenderer* bombCount2ndDigitRenderer_;
	const float4 munitionStatusUI_MulColor_;
	const float4 munitionStatusUI_PlusColor_;
	int bombCount_;


	GameEngineUIRenderer* remainingTime1stDigitRenderer_;
	GameEngineUIRenderer* remainingTime2ndDigitRenderer_;


	GameEngineUIRenderer* insertCoin_I_Renderer_;
	GameEngineUIRenderer* insertCoin_N_Renderer_;
	GameEngineUIRenderer* insertCoin_S_Renderer_;
	GameEngineUIRenderer* insertCoin_E_Renderer_;
	GameEngineUIRenderer* insertCoin_R_Renderer_;
	GameEngineUIRenderer* insertCoin_T_Renderer_;

	GameEngineUIRenderer* insertCoin_C_Renderer_;
	GameEngineUIRenderer* insertCoin_O_Renderer_;
	GameEngineUIRenderer* insertCoin_I_Renderer2_;
	GameEngineUIRenderer* insertCoin_N_Renderer2_;


};

