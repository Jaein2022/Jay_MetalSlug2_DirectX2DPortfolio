#pragma once

class GameEngineNameObject
{
	//이 클래스의 존재 이유:

public:
	GameEngineNameObject();
	virtual ~GameEngineNameObject();

public:
	inline void SetName(const std::string& _name)
	{
		objectName_ = _name;
	}

	std::string GetNameCopy() const
	{
		return objectName_;
	}

	const std::string& GetNameConstRef()	
	{
		return objectName_;
	}

	const char* GetNameConstPtr()
	{
		return objectName_.c_str();
	}

private:
	std::string objectName_;


};

