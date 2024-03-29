#include "PreCompile.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"

GameEngineFile::GameEngineFile(): filePtr_(nullptr)
{
}

GameEngineFile::GameEngineFile(const char* _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const std::string& _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const std::filesystem::path& _path) : filePtr_(nullptr)
{
	path_ = _path;
}

GameEngineFile::GameEngineFile(const GameEngineFile& _other) : filePtr_(nullptr)
{
	path_ = _other.path_;
}

GameEngineFile::GameEngineFile(GameEngineFile&& _other) noexcept : filePtr_(nullptr)
{
	path_ = _other.path_;
}

GameEngineFile::~GameEngineFile()
{
}

void GameEngineFile::Open(OpenMode _openMode, FileMode _fileMode)
{
	std::string mode = "";

	// "wb": 쓴다 바이너리.
	// "wt": 쓴다 텍스트.

	// "rb": 읽는다 바이너리.
	// "rt": 읽는다 텍스트.


	switch (_openMode)
	{
	case OpenMode::Read:
		mode += "r";
		break;

	case OpenMode::Write:
		mode += "w";
		break;

	default:
		MsgBoxAssert("불가능한 방식입니다.");
		return;
	}

	switch (_fileMode)
	{
	case FileMode::Binary:
		mode += "b";
		break;

	case FileMode::Text:
		mode += "t";
		break;

	default:
		MsgBoxAssert("불가능한 방식입니다.");
		return;
	}

	int openResult = fopen_s(	//지정한 경로의 파일을 열고 파일핸들을 받아오는 함수.
		&filePtr_,				//열린 파일에 대한 파일핸들의 주소값.
		path_.string().c_str(),	//경로가 포함된 파일 이름.
		mode.c_str()			//파일 오픈 모드. 
	);

	if (0 != openResult)
	{
		MsgBoxAssert("파일을 정상적으로 여는데 실패했습니다.");
		return;
	}
}

void GameEngineFile::Close()
{
	if (nullptr != filePtr_)
	{
		fclose(filePtr_);
		filePtr_ = nullptr;
	}
}

void GameEngineFile::Read(void* _readData, size_t _dataSize, size_t _readSize)
{
	fread_s(
		_readData,
		_dataSize,
		_readSize,
		1,			//읽는 횟수.
		filePtr_
	);
}

void GameEngineFile::Write(void* _writeData, size_t _writeSize)
{
	fwrite(
		_writeData,
		_writeSize,
		1,			//쓰는 횟수.
		filePtr_
	);
}

std::string GameEngineFile::GetString()
{
	std::string allString;

	uintmax_t fileSize = GetFileSize();

	allString.resize(fileSize);	//unsigned long long == uintmax_t == size_t.

	Read(&allString[0], fileSize, fileSize);

	return allString;
}

uintmax_t GameEngineFile::GetFileSize() const
{
	return std::filesystem::file_size(this->path_);
}

uintmax_t GameEngineFile::GetFileSize(const std::filesystem::path& _path)
{
	return std::filesystem::file_size(_path);
}
