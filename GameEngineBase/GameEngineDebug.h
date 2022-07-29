#pragma once

class GameEngineDebug
{
	//이 클래스의 존재 이유:

	GameEngineDebug();
	~GameEngineDebug();
	GameEngineDebug(const GameEngineDebug& _other) = delete;
	GameEngineDebug(GameEngineDebug&& _other) noexcept = delete;
	GameEngineDebug& operator=(const GameEngineDebug& _other) = delete;
	GameEngineDebug& operator=(const GameEngineDebug&& _other) = delete;


public:	
	static void LeakCheckOn();
	static void ConsoleOpen();
	static void OutputString(const std::string& _text);
	static void Destroy();

public:
#define MsgBox(text) MessageBeep(0);	\
MessageBoxA(nullptr, text, "Error", MB_OK);

#define MsgBoxAssert(text)	MessageBeep(0);	\
MessageBoxA(nullptr, text, "Error!", MB_OK);	\
assert(false);

#define MsgBoxAssertString(text) MessageBeep(0);	\
MessageBoxA(nullptr, (text).c_str(), "Error!", MB_OK);	\
assert(false);

private:
	static HANDLE hConsole_;

};

