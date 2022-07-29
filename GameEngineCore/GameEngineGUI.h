#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class GameEngineGUIWindow : public GameEngineNameObject
{
	//�� Ŭ������ ���� ������??

	friend class GameEngineGUI;

	//bool isOpened_ = false;


	void Begin();
	void End();


protected:
	virtual void Initialize(class GameEngineLevel* _level) = 0;
	virtual void OnGUI(class GameEngineLevel* _level, float _deltaTime) = 0;
};

class GameEngineGUI
{
	//ImGui ���� �� ���� Ŭ����.

	friend class GameEngineCore;
	//Destroy()�Լ��� ���� GameEngineCoreŬ���������� ���� �ϱ� ���ؼ� private�� ���� friend�� �Ǵ�.

	static std::list<GameEngineGUIWindow*> windows_;


public:
	GameEngineGUI();
	~GameEngineGUI();

protected:
	GameEngineGUI(const GameEngineGUI& _other) = delete;
	GameEngineGUI(GameEngineGUI&& _other) noexcept = delete;

private:
	GameEngineGUI& operator=(const GameEngineGUI& _other) = delete;
	GameEngineGUI& operator=(const GameEngineGUI&& _other) = delete;

public:
	static void Render(class GameEngineLevel* _level, float _deltaTime);


public:
	template<typename GUIWindowType>
	static GameEngineGUIWindow* CreateGUIWindow(const std::string& _name, GameEngineLevel* _level)
	{
		GUIWindowType* newWindow = new GUIWindowType();
		GameEngineGUIWindow* initWindow = newWindow;	//
		initWindow->SetName(_name);
		initWindow->Initialize(_level);
		windows_.push_back(newWindow);
		return newWindow;
	}

private:
	static void Initialize();
	static void Destroy();

};

