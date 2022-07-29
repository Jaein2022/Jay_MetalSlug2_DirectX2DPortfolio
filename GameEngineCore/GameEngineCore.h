#pragma once

class GameEngineLevel;
class GameEngineCore
{
	//이 클래스의 존재 이유:

	friend class GEngine;

protected:
	GameEngineCore();
	~GameEngineCore();


private:
	GameEngineCore(const GameEngineCore& _other) = delete;
	GameEngineCore(GameEngineCore&& _other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _other) = delete;
	GameEngineCore& operator=(const GameEngineCore&& _other) = delete;


public:
	template<typename CoreType>
	static void Start()
	{
		GameEngineDebug::LeakCheckOn();

#ifdef _DEBUG
		int* i = new int(11);	//릭 체크가 이루어지고 있는지를 확인하기 위한 의도적인 릭.
#endif							//릴리즈모드에서는 하지 않는다.

		CoreType core{};
		WindowCreate(core.GetWindowTitle(), &core);


	}



protected:	//컨텐츠코어만 사용해야 하는 함수들.
	virtual std::string GetWindowTitle()
	{
		return "DefaultWindowTitle";
	}

	//컨텐츠 제작자용 함수들.
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;
	virtual void End() = 0;

	template <typename LevelType>
	static GameEngineLevel* CreateLevel(const std::string& _levelName)
	{
		std::string uppercaseLevelName = GameEngineString::ToUpperReturn(_levelName);

		GameEngineLevel* newLevel = new LevelType();
		InitializeLevel(newLevel, uppercaseLevelName);
		return newLevel;
	}

	static GameEngineLevel* FindLevel(const std::string& _levelName)
	{
		std::string uppercaseLevelName = GameEngineString::ToUpperReturn(_levelName);
		std::map<std::string, GameEngineLevel*>::iterator findIter = allLevels_.find(uppercaseLevelName);
		if (allLevels_.end() == findIter)
		{
			return nullptr;
		}
		else
		{
			return findIter->second;
		}
	}

	static bool ChangeLevel(const std::string& _levelName);



protected:
	static void WindowCreate(const std::string& _title, GameEngineCore* _userCore);
	static void CoreStart(GameEngineCore* _userCore);	//프로그램 시작.
	static void CoreUpdate(GameEngineCore* _userCore);	//프로그램 갱신.
	static void CoreEnd(GameEngineCore* _userCore);	//프로그램 종료.

	static void EngineResourceInitialize();
	//사각형, 육면체, 에러텍스쳐 등등, 엔진 수준에서 기본적으로 지원되어야 하는 리소스를 준비하는 함수.

	static void EngineResourceDestroy();	
	//사각형, 육면체, 에러텍스쳐 등등, 엔진 수준에서 기본적으로 지원되어야 하는 리소스를 삭제하는 함수.

	static void InitializeLevel(GameEngineLevel* _level, const std::string& _levelName);
	//헤더 추가하기 싫어서 초기화를 CPP에서 하려고 만든 함수.



private:
	static std::map<std::string, class GameEngineLevel*> allLevels_;
	static GameEngineLevel* currentLevel_;
	static GameEngineLevel* nextLevel_;

};

