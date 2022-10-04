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
public:
    SoundSystemCreater()
    {
        FMOD::System_Create(&soundSystem_);

        if (nullptr == soundSystem_)
        {
            MsgBoxAssert("사운드시스템 생성에 실패했습니다.");
            return;
        }
        if (FMOD_OK != soundSystem_->init(32, FMOD_DEFAULT, nullptr))
        {
            MsgBoxAssert("사운드시스템 초기화에 실패했습니다.");
            return;
        }
    }
};

SoundSystemCreater CreateInst = SoundSystemCreater();

std::map<std::string, GameEngineSound*> GameEngineSound::allSounds_;


GameEngineSound::GameEngineSound() : sound_(nullptr)
{
}

GameEngineSound::~GameEngineSound()
{
}

GameEngineSoundPlayer GameEngineSound::SoundPlayControl(const std::string& _name, unsigned int _loopCount /*= 0*/)
{
    std::string uppercaseResourceName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* findSound = GameEngineSound::Find(uppercaseResourceName);
    if (nullptr == findSound)
    {
        MsgBoxAssertString(_name + ": 그런 이름의 사운드가 존재하지 않습니다.");
        return GameEngineSoundPlayer();
    }

    FMOD::Channel* playControl = nullptr;

    soundSystem_->playSound(findSound->sound_, nullptr, false, &playControl);

    playControl->setLoopCount(_loopCount);

    return GameEngineSoundPlayer(findSound, playControl);
}

void GameEngineSound::SoundPlayOneshot(const std::string& _name, int _loopCount)
{
    std::string uppercaseResourceName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* findSound = GameEngineSound::Find(uppercaseResourceName);
    if (nullptr == findSound)
    {
        MsgBoxAssertString(_name + ": 그런 이름의 사운드가 존재하지 않습니다.");
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

GameEngineSound* GameEngineSound::Load(const GameEngineFile& _path)
{
    return Load(_path.GetFullPath());
}

GameEngineSound* GameEngineSound::Load(const std::string& _path)
{
    GameEnginePath newPath = GameEnginePath(_path);
    return Load(_path, newPath.GetFileName());
}

GameEngineSound* GameEngineSound::Load(const std::string& _path, const std::string& _name)
{
    std::string uppercaseResourceName = GameEngineString::ToUpperReturn(_name);
    GameEngineSound* newSound = new GameEngineSound();
    if (false == newSound->LoadSound(_path))
    {
        delete newSound;
        newSound = nullptr;
        MsgBoxAssertString(_name + ": 그런 이름의 사운드가 존재하지 않습니다.");
        return nullptr;
    }

    allSounds_.insert(std::make_pair(uppercaseResourceName, newSound));

    return newSound;
}

GameEngineSound* GameEngineSound::Find(const std::string& _name)
{
    std::string uppercaseResourceName = GameEngineString::ToUpperReturn(_name);
    std::map<std::string, GameEngineSound*>::iterator findIter = allSounds_.find(uppercaseResourceName);
    if (allSounds_.end() == findIter)
    {
        return nullptr;
    }
    else
    {
        return findIter->second;
    }
}

void GameEngineSound::ResourceDestroy()
{
    for (std::pair<std::string, GameEngineSound*> soundPair : allSounds_)
    {
        delete soundPair.second;
        soundPair.second = nullptr;
    }

    allSounds_.clear();

    if (nullptr != soundSystem_)
    {
        soundSystem_->release();
        soundSystem_ = nullptr;
    }
}

bool GameEngineSound::LoadSound(const std::string& _path)
{
    if (FMOD_OK != soundSystem_->createSound(_path.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound_))
    {
        MsgBoxAssert("사운드를 불러오는데 실패했습니다.");
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
        MsgBoxAssert("사운드 컨트롤 채널에 문제가 있습니다.");
        return;
    }
    controlHandle_->stop();
}

void GameEngineSoundPlayer::PlaySpeed(float _speed)
{
    if (nullptr == controlHandle_)
    {
        MsgBoxAssert("사운드 컨트롤 채널에 문제가 있습니다.");
        return;
    }
    controlHandle_->setPitch(_speed);
}

void GameEngineSoundPlayer::Volume(float _value)
{
    if (nullptr == controlHandle_)
    {
        MsgBoxAssert("사운드 컨트롤 채널에 문제가 있습니다.");
        return;
    }
    controlHandle_->setVolume(_value);
}
