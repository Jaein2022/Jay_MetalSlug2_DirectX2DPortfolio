#pragma once

class Mission1BG: public GameEngineActor
{
	//이 클래스의 존재 이유: 미션1 배경.
public:
	Mission1BG();
	~Mission1BG();

protected:
	Mission1BG(const Mission1BG& _other) = delete;
	Mission1BG(Mission1BG&& _other) noexcept = delete;

private:
	Mission1BG& operator=(const Mission1BG& _other) = delete;
	Mission1BG& operator=(const Mission1BG&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


public:
	inline float GetPart1RightEnd()
	{
		return part1_FirstBG_InitPosition_.x + part1_FirstBG_->GetCurrentTexture()->GetScale().x;
	}

	inline float GetPart2RightEnd()
	{
		return part2_FirstBG_InitPosition_.x + part2_FirstBG_->GetCurrentTexture()->GetScale().x;
	}

	inline float GetPart3RightEnd()
	{
		return part3_FirstBG_InitPosition_.x + part3_FirstBG_->GetCurrentTexture()->GetScale().x;
	}

private:
	GameEngineTextureRenderer* part1_FirstBG_;
	const float4 part1_FirstBG_InitPosition_;
	GameEngineTextureRenderer* part1_FirstBG_Footstep_;
	GameEngineTextureRenderer* part1_SecondBG_;
	GameEngineTextureRenderer* part1_ThirdBG_1_;
	GameEngineTextureRenderer* part1_ThirdBG_2_;
	GameEngineTextureRenderer* part1_Sign_;
	GameEngineTextureRenderer* part1_SteppableObject_;
	GameEngineTextureRenderer* skyBG_1_;


	GameEngineTextureRenderer* part2_FirstBG_;
	const float4 part2_FirstBG_InitPosition_;
	GameEngineTextureRenderer* skyBG_2_;


	GameEngineTextureRenderer* part3_FirstBG_;
	const float4 part3_FirstBG_InitPosition_;
	GameEngineTextureRenderer* skyBG_3_;


};

