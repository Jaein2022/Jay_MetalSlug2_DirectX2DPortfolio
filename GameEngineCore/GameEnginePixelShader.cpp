#include "PreCompile.h"
#include "GameEnginePixelShader.h"
#include "GameEngineDevice.h"

GameEnginePixelShader::GameEnginePixelShader() : pixelShader_(nullptr)
{
    shaderType_ = ShaderType::PixelShader;
}

GameEnginePixelShader::~GameEnginePixelShader()
{
    if (nullptr != pixelShader_)
    {
        pixelShader_->Release();
        pixelShader_ = nullptr;
    }
}

void GameEnginePixelShader::Setting()
{
    if (nullptr == pixelShader_)
    {
        MsgBoxAssert("픽셀셰이더가 없습니다.");
        return;
    }

    //두번째 인자는 #include나 #define등 hlsl에서 사용할 헤더나 디파인의 객체를 넣어줄수 있다.
    GameEngineDevice::GetContext()->PSSetShader(//파이프라인에 픽셀셰이더를 세팅하는 함수.
        pixelShader_,                      //픽셀셰이더 포인터.
        nullptr,                           //클래스 인스턴스 인터페이스 배열 주소값. 없으면 NULL.
        0                                  //사용할 클래스 인스턴스 인터페이스 배열의 인덱스.
    );
}

GameEnginePixelShader* GameEnginePixelShader::Load(
    const std::string& _path,
    const std::string& _entryPoint,
    UINT _versionHigh /*= 5*/,
    UINT _versionLow /*= 0*/
)
{
    return Load(_path, GameEnginePath::GetFileName(_path), _entryPoint, _versionHigh, _versionLow);
}

GameEnginePixelShader* GameEnginePixelShader::Load(
    const std::string& _path,
    const std::string& _name,
    const std::string& _entryPoint,
    UINT _versionHigh /*= 5*/,
    UINT _versionLow /*= 0*/
)
{
    GameEnginePixelShader* newRes = CreateNamedRes(_name);
    newRes->CreateVersion("ps", _versionHigh, _versionLow);
    newRes->SetEntrtyPoint(_entryPoint);
    newRes->CompileHLSLCode(_path);
    newRes->CreatePixelShader();
    return newRes;
}

void GameEnginePixelShader::CreatePixelShader()
{
    if (S_OK != GameEngineDevice::GetDevice()->CreatePixelShader(   //픽셀셰이더 생성 함수.
        binaryCodePtr_->GetBufferPointer(), //컴파일된 바이너리코드.
        binaryCodePtr_->GetBufferSize(),    //컴파일된 바이너리코드 크기.
        NULL,                               //??
        &pixelShader_                       //픽셀셰이더 포인터.
    ))
    {
        MsgBoxAssert("픽셀셰이더 생성 실패.");
        return;
    }

    ShaderResCheck();
}