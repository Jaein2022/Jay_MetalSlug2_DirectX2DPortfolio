#pragma once

class UI: public GameEngineActor
{
	//�� Ŭ������ ���� ����: UI
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



	GameEngineUIRenderer* letterRenderer_;
};

