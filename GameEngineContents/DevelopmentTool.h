#pragma once

class Mission1;
class DevelopmentTool: public GameEngineGUIWindow
{
	//�� Ŭ������ ���� ����: �׶��׶� �ʿ��� ���� ���� ���ǵ��� ����.
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

