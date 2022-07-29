#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class GameEngineGUIWindow : public GameEngineNameObject
{
	//이 클래스의 존재 이유는??

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
	//ImGui 생성 및 관리 클래스.

	friend class GameEngineCore;
	//Destroy()함수를 오직 GameEngineCore클래스에서만 쓰게 하기 위해서 private로 막고 friend를 건다.

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

