#include "PreCompile.h"
#include "GameEngineDebug.h"

HANDLE GameEngineDebug::hConsole_ = nullptr;

GameEngineDebug::GameEngineDebug()
{
}

GameEngineDebug::~GameEngineDebug()
{
}

void GameEngineDebug::LeakCheckOn()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(319);
}

void GameEngineDebug::ConsoleOpen()
{
	if (AllocConsole())
	{
		FILE* file;
		freopen_s(&file, "CONIN$", "rb", stdin);
		freopen_s(&file, "CONOUT$", "wb", stdout);
		freopen_s(&file, "CONOUT$", "wb", stderr);

		hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
		std::cout << "Console Start." << std::endl;
	}
}

void GameEngineDebug::OutputString(const std::string& _text)
{
	OutputDebugStringA((_text + "\n").c_str());
}

void GameEngineDebug::Destroy()
{
	if (nullptr != hConsole_)
	{
		FreeConsole();
		hConsole_ = nullptr;
	}
}
