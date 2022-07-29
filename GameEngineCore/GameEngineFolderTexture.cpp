#include "PreCompile.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineDevice.h"
#include "GameEngineTexture.h"

GameEngineFolderTexture::GameEngineFolderTexture()
{
}

GameEngineFolderTexture::~GameEngineFolderTexture()
{
	for (size_t i = 0; i < textures_.size(); i++)
	{
		if (nullptr != textures_[i])
		{
			delete textures_[i];
			textures_[i] = nullptr;
		}
	}
	//정식 GameEngineTexture 생성 경로인 Create()나 Load()함수를 통하지 않고 직접 만들었으므로
	//해제도 직접 해줘야 한다.
}

GameEngineFolderTexture* GameEngineFolderTexture::Load(const std::string& _path)
{
	return Load(_path, GameEnginePath::GetFileName(_path));
}

GameEngineFolderTexture* GameEngineFolderTexture::Load(const std::string& _path, const std::string _name)
{
	GameEngineFolderTexture* newFolderTexture = CreateNamedRes(_name);
	newFolderTexture->LoadFolder(_path);
	return newFolderTexture;
}

void GameEngineFolderTexture::LoadFolder(const std::string& _path)
{
	GameEngineDirectory folderTextureDir = _path;
	std::vector<GameEngineFile> allFolderFiles = folderTextureDir.GetAllFiles();

	for (size_t i = 0; i < allFolderFiles.size(); i++)
	{
		GameEngineTexture* newTexture = new GameEngineTexture();
		newTexture->TextureLoad(allFolderFiles[i].GetFullPath());
		textures_.push_back(newTexture);
	}
}
