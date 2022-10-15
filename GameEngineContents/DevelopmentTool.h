#pragma once

class Mission1;
class DevelopmentTool: public GameEngineGUIWindow
{
	//이 클래스의 존재 이유: 그때그때 필요한 각종 개발 편의도구 모음.
public:
	DevelopmentTool();
	~DevelopmentTool();

protected:
	DevelopmentTool(const DevelopmentTool& _other) = delete;
	DevelopmentTool(DevelopmentTool&& _other) noexcept = delete;

private:
	DevelopmentTool& operator=(const DevelopmentTool& _other) = delete;
	DevelopmentTool& operator=(const DevelopmentTool&& _other) = delete;


public:	
	void Initialize(class GameEngineLevel* _level) override;
	void OnGUI(class GameEngineLevel* _level, float _deltaTime) override;


private:
	Mission1* mission1_;

};

