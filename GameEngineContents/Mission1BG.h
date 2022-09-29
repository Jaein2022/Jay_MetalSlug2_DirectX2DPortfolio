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


private:
	GameEngineTextureRenderer* part1_FirstBG_;
	GameEngineTextureRenderer* part1_FirstBG_Footstep_;
	GameEngineTextureRenderer* part1_SecondBG_;
	GameEngineTextureRenderer* part1_ThirdBG_1_;
	GameEngineTextureRenderer* part1_ThirdBG_2_;
	GameEngineTextureRenderer* part1_FourthBG_;
	GameEngineTextureRenderer* part1_Sign_;
	GameEngineTextureRenderer* part1_SteppableObject_;



};

