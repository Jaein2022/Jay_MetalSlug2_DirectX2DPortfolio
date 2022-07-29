#include "PreCompile.h"
#include "GameEngineSound.h"
#include "GameEnginePath.h"
#include "GameEngineDebug.h"
#include "GameEngineString.h"

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif // DEBUG

FMOD::System* soundSystem_ = nullptr;

class SoundSystemCreater
{
    SoundSystemCreater()
    {
        FMOD::System_Create(&soundSystem_);

        if (nullptr == soundSystem_)
        {
            MsgBoxAssert("����ý��� ������ �����߽��ϴ�.");
            return;
        }
        if (FMOD_OK != soundSystem_->init(32, FMOD_DEFAULT, nullptr))
        {
            MsgBoxAssert("����ý��� �ʱ�ȭ�� �����߽��ϴ�.");
            return;
        }
    }
};


GameEngineSound::GameEngineSound() : sound_(nullptr)
{
}

GameEngineSound::~GameEngineSound()
{
}

GameEngineSoundPlayer GameEngineSound::SoundPlayControl(const std::string& _name, unsigned int _loopCount /*= 0*/)
{
    std::string upperCaseName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* findSound = GameEngineSound::FindRessource(upperCaseName);
    if (nullptr == findSound)
    {
        MsgBoxAssertString(_name + ": �׷� �̸��� ���尡 �������� �ʽ��ϴ�.");
        return GameEngineSoundPlayer();
    }

    FMOD::Channel* playControl = nullptr;

    soundSystem_->playSound(findSound->sound_, nullptr, false, &playControl);

    playControl->setLoopCount(_loopCount);

    return GameEngineSoundPlayer(findSound, playControl);
}

void GameEngineSound::SoundPlayOneshot(const std::string& _name, int _loopCount)
{
    std::string upperCaseName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* findSound = GameEngineSound::FindRessource(upperCaseName);
    if (nullptr == findSound)
    {
        MsgBoxAssertString(_name + ": �׷� �̸��� ���尡 �������� �ʽ��ϴ�.");
        return;
    }

    FMOD::Channel* playControl = nullptr;

    soundSystem_->playSound(findSound->sound_, nullptr, false, &playControl);

    playControl->setLoopCount(_loopCount);
}

void GameEngineSound::Update()
{
    if (nullptr != soundSystem_)
    {
        soundSystem_->update();
    }
}

GameEngineSound* GameEngineSound::LoadResource(const GameEngineFile& _path)
{
    return LoadResource(_path.GetFullPath());
}

GameEngineSound* GameEngineSound::LoadResource(const std::string& _path)
{
    GameEnginePath newPath = GameEnginePath(_path);
    return LoadResource(_path, newPath.GetFileName());
}

GameEngineSound* GameEngineSound::LoadResource(const std::string& _path, const std::string& _name)
{
    std::string upperCaseName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* newSound = new GameEngineSound();
    if (false == newSound->Load(_path))
    {
        delete newSound;
        newSound = nullptr;
        MsgBoxAssertString(_name + ": �׷� �̸��� ���尡 �������� �ʽ��ϴ�.");
        return nullptr;
    }

    allResource_.insert(std::make_pair(upperCaseName, newSound));

    return newSound;
}

GameEngineSound* GameEngineSound::FindRessource(const std::string& _name)
{
    std::string upperCaseName = GameEngineString::ToUpperReturn(_name);
    std::map<std::string, GameEngineSound*>::iterator findIter = allResource_.find(upperCaseName);
    if (allResource_.end() == findIter)
    {
        return nullptr;
    }
    else
    {
        return findIter->second;
    }
}

void GameEngineSound::AllResourceDestroy()
{
    for (std::pair<std::string, GameEngineSound*> Res : allResource_)
    {
        delete Res.second;
        Res.second = nullptr;
    }

    allResource_.clear();

    if (nullptr != soundSystem_)
    {
        soundSystem_->release();
        soundSystem_ = nullptr;
    }
}

bool GameEngineSound::Load(const std::string& _path)
{
    if (FMOD_OK != soundSystem_->createSound(_path.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound_))
    {
        MsgBoxAssert("���带 �ҷ����µ� �����߽��ϴ�.");
        return false;
    }

    return true;
}





GameEngineSoundPlayer::GameEngineSoundPlayer(GameEngineSound* _sound, FMOD::Channel* _controlHandle)
    :sound_(_sound), controlHandle_(_controlHandle)
{
}

GameEngineSoundPlayer::GameEngineSoundPlayer() : sound_(nullptr), controlHandle_(nullptr)
{
}

GameEngineSoundPlayer::GameEngineSoundPlayer(const GameEngineSoundPlayer& _other)
    : sound_(_other.sound_), controlHandle_(_other.controlHandle_)
{
}

GameEngineSoundPlayer::~GameEngineSoundPlayer()
{
}

void GameEngineSoundPlayer::Stop()
{
    if (nullptr == controlHandle_)
    {
        MsgBoxAssert("���� ��Ʈ�� ä�ο� ������ �ֽ��ϴ�.");
        return;
    }
    controlHandle_->stop();
}

void GameEngineSoundPlayer::PlaySpeed(float _speed)
{
    if (nullptr == controlHandle_)
    {
        MsgBoxAssert("���� ��Ʈ�� ä�ο� ������ �ֽ��ϴ�.");
        return;
    }
    controlHandle_->setPitch(_speed);
}

void GameEngineSoundPlayer::Volume(float _value)
{
    if (nullptr == controlHandle_)
    {
        MsgBoxAssert("���� ��Ʈ�� ä�ο� ������ �ֽ��ϴ�.");
        return;
    }
    controlHandle_->setVolume(_value);
}
