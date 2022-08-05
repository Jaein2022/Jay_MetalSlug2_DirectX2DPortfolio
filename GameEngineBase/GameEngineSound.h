#pragma once
#include "GameEngineNameObject.h"
#include "GameEngineFile.h"
#include <GameEngineBase\ThirdParty\FMOD\Include\fmod.hpp>

class GameEngineSoundPlayer;
class GameEngineSound: public GameEngineNameObject
{
	//�� Ŭ������ ���� ����:
public:
	GameEngineSound();
	~GameEngineSound();

protected:
	GameEngineSound(const GameEngineSound& _other) = delete;
	GameEngineSound(GameEngineSound&& _other) noexcept = delete;

private:
	GameEngineSound& operator=(const GameEngineSound& _other) = delete;
	GameEngineSound& operator=(const GameEngineSound&& _other) = delete;


public:	
	static GameEngineSoundPlayer SoundPlayControl(const std::string& _name, unsigned int _loopCount = 0);
	static void SoundPlayOneshot(const std::string& _name, int _loopCount = 0);
	static void Update();

	//������ ���庸�� �� �ٺ��� �����Ƿ� ���ӿ������忡�� ���ӿ��� ���ϵ� ���� �� �ְ� �����Ѵ�.
	static GameEngineSound* LoadResource(const GameEngineFile& _path);
	static GameEngineSound* LoadResource(const std::string& _path);
	static GameEngineSound* LoadResource(const std::string& _path, const std::string& _name);
	static GameEngineSound* FindResources(const std::string& _name);

	static void ResourceDestroy();

protected:
	bool Load(const std::string& _path);


private:
	FMOD::Sound* sound_;

	static std::map<std::string, GameEngineSound*> allResource_;
};

class GameEngineSoundPlayer
{
	friend GameEngineSound;

private:
	GameEngineSoundPlayer(GameEngineSound* _sound, FMOD::Channel* _controlHandle);

public:
	GameEngineSoundPlayer();
	GameEngineSoundPlayer(const GameEngineSoundPlayer& _other);
	~GameEngineSoundPlayer();


public:
	void Stop();
	void PlaySpeed(float _speed);
	void Volume(float _value);



private:

	GameEngineSound* sound_;
	FMOD::Channel* controlHandle_;


};
