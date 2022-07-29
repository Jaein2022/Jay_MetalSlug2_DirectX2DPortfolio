#pragma once

class TempLevel: public GameEngineLevel
{
	//이 클래스의 존재 이유:
public:
	TempLevel();
	~TempLevel();

protected:
	TempLevel(const TempLevel& _other) = delete;
	TempLevel(TempLevel&& _other) noexcept = delete;

private:
	TempLevel& operator=(const TempLevel& _other) = delete;
	TempLevel& operator=(const TempLevel&& _other) = delete;


public:	
	void Start() override;
	void Update(float _deltaTime) override;
	void End() override;


private:

};

