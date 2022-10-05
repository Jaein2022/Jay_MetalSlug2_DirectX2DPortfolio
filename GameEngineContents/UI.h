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
	GameEngineUIRenderer* capitalLetterRenderer_;
};

