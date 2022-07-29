#pragma once
#include "GameEngineDefaultRenderer.h"

enum class PivotMode
{
	Center,		//�߽�.
	LeftTop,	//�»��.
	Bot,		//�ϴ��߾�.
	Custom,		//��������.
	None
};

class FrameAnimation_Desc
{
	//������ �ִϸ��̼� ������ �ʿ��� ���� ���������� ��Ƽ� ����, �����ϴ� Ŭ����.
public:
	std::string textureName_;

	bool isLoop_;		//true == �ִϸ��̼� ���ѹݺ�. false == 1ȸ�ݺ� �� �����������ӿ��� ����.
	float interval_;	//�����Ӱ� �ð�����. �⺻�� 0.1f

	UINT start_;	//���� ������ �ε���.
	UINT curFrame_;	//���� ������ �ε���.
	UINT end_;		//������ ������ �ε���.

	float frameTime_;//���������� ���� �ð�.



	FrameAnimation_Desc()
		: isLoop_(false),
		interval_(1.f),
		start_(-1),
		curFrame_(-1),
		end_(-1),
		frameTime_(0.f)
	{
	}

	FrameAnimation_Desc(const std::string& _textureName, UINT _start, UINT _end, float _interval, bool _isLoop = true)
		: textureName_(_textureName),
		isLoop_(_isLoop),
		interval_(_interval),
		start_(_start),
		curFrame_(_start),
		end_(_end),
		frameTime_(0.f)
	{
	}

	FrameAnimation_Desc(const std::string& _textureName, float _interval, bool _isLoop = true)
		: textureName_(_textureName),
		isLoop_(_isLoop),
		interval_(_interval),
		start_(-1),
		curFrame_(0),
		end_(-1),
		frameTime_(0.f)
	{
	}


};

class GameEngineFolderTexture;
class FrameAnimation
{
	//������ �ؽ��� �� �ִϸ��̼� ������ Ŭ����.

	friend class GameEngineTextureRenderer;
	//

	FrameAnimation_Desc desc_;	//�ִϸ��̼� ������ �ʿ��� ������ ����.

	GameEngineTextureRenderer* parentRenderer_;	//�θ� ������.

	GameEngineTexture* atlasTexture_;			//���� �ؽ���.			
	GameEngineFolderTexture* folderTexture_;	//�������� �ؽ��ĵ� ����.
	//�ΰ��� ���ÿ� ���Ǵ� ���� ����!

	bool bOnceStart_;
	bool bOnceEnd_;

	std::function<void(const FrameAnimation_Desc&)> start_;		//�ִϸ��̼� ��� ���۽� ȣ��Ǵ� �Լ���.
	std::function<void(const FrameAnimation_Desc&)> frame_;		//�ִϸ��̼� ����� �� �����Ӹ��� ȣ��Ǵ� �Լ���.
	std::function<void(const FrameAnimation_Desc&, float)> time_;	//�ִϸ��̼� ����� ���� �ð� �Ŀ� ȣ��Ǵ� �Լ���.
	std::function<void(const FrameAnimation_Desc&)> end_;			//�ִϸ��̼� ��� ����� ȣ��Ǵ� �Լ���.

	void Reset();
	void Update(float _deltaTime);

public:
	FrameAnimation()
		: parentRenderer_(nullptr),
		atlasTexture_(nullptr),
		folderTexture_(nullptr),
		bOnceStart_(false),
		bOnceEnd_(false)
	{
	}

};

class GameEngineTexture;
class GameEngineTextureRenderer : public GameEngineDefaultRenderer
{
	//Ư�� �ؽ�ó�� �� �ؽ�ó�� ���� �ִϸ��̼��� �������ϴ� ������.

	friend FrameAnimation;
	//


public:
	GameEngineTextureRenderer();
	~GameEngineTextureRenderer();

protected:
	GameEngineTextureRenderer(const GameEngineTextureRenderer& _other) = delete;
	GameEngineTextureRenderer(GameEngineTextureRenderer&& _other) noexcept = delete;

private:
	GameEngineTextureRenderer& operator=(const GameEngineTextureRenderer& _other) = delete;
	GameEngineTextureRenderer& operator=(const GameEngineTextureRenderer&& _other) = delete;


public:
	void SetTexture(const std::string& _textureName);	//�����ؽ�ó��.
	void SetTexture(GameEngineTexture* _texture);		//�����ؽ�ó��.
	void SetTexture(const std::string& _textureName, int _index);	//��Ʋ���ؽ�ó��.
	void SetTexture(GameEngineTexture* _texture, int _index);		//��Ʋ���ؽ�ó��.

	void SetFrame(int _index);	//�ִϸ��̼��� Ư�� ������ ����.

	void SetPivot();
	void SetPivot(PivotMode _pivot);
	void SetPivotToVector(const float4& _localPos);

	void SetSamplingMode_Point();
	//���Ͱ� D3D11_FILTER_MIN_MAG_MIP_POINT�� ���õ� ���÷� ���.

	void SetSamplingMode_Linear();
	//���Ͱ� D3D11_FILTER_MIN_MAG_MIP_LINEAR�� ���õ� ���÷� ���.


	void CreateFrameAnimation_AtlasTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc);
	void CreateFrameAnimation_FolderTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc);

	void ChangeFrameAnimation(const std::string& _animationName);

	void ScaleToTexture();	//�ؽ�ó ũ�⿡ �޽� �������� �ڵ����� �����ִ� �Լ�.

	void CurAnimationReset();	//FrameDataReset()�Լ��� ���� ������? 
	void CurAnimationStartPivotFrame(int _setFrame);	//�ִϸ��̼� �� ���� ���ϴ� ���������� �ű�� �Լ�.

	GameEngineTexture* GetCurrentTexture() const;

public:
	template<typename ObjectType>
	void AnimationBindStart(
		const std::string& _animationName,
		void(ObjectType::* _function)(const FrameAnimation_Desc&), ObjectType* _this)
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].start_ = std::bind(_function, _this, allAnimations_[uppercaseAnimationName].desc_);
	}

	template<typename ObjectType>
	void AnimationBindFrame(
		const std::string& _animationName,
		void(ObjectType::* _function)(const FrameAnimation_Desc&), ObjectType* _this)
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].frame_ = std::bind(_function, _this, allAnimations_[uppercaseAnimationName].desc_);
	}

	template<typename ObjectType>
	void AnimationBindTime(
		const std::string& _animationName,
		void(ObjectType::* _function)(const FrameAnimation_Desc&), ObjectType* _this)
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].time_ = std::bind(_function, _this, allAnimations_[uppercaseAnimationName].desc_);
	}

	template<typename ObjectType>
	void AnimationBindEnd(
		const std::string& _animationName,
		void(ObjectType::* _function)(const FrameAnimation_Desc&), ObjectType* _this)
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].end_ = std::bind(_function, _this, allAnimations_[uppercaseAnimationName].desc_);
	}

	void AnimationBindStart(
		const std::string& _animationName,
		void(*_function)(const FrameAnimation_Desc&))
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].start_ = std::bind(_function, allAnimations_[uppercaseAnimationName].desc_);
	}

	void AnimationBindFrame(
		const std::string& _animationName,
		void(*_function)(const FrameAnimation_Desc&))
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].frame_ = std::bind(_function, allAnimations_[uppercaseAnimationName].desc_);
	}

	void AnimationBindTime(
		const std::string& _animationName,
		void(*_function)(const FrameAnimation_Desc&))
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].time_ = std::bind(_function, allAnimations_[uppercaseAnimationName].desc_);
	}

	void AnimationBindEnd(
		const std::string& _animationName,
		void(*_function)(const FrameAnimation_Desc&))
	{
		std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

		if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
		{
			MsgBoxAssertString(_animationName + ": �׷� �̸��� �ִϸ��̼��� �����ϴ�.");
			return;
		}

		allAnimations_[uppercaseAnimationName].end_ = std::bind(_function, allAnimations_[uppercaseAnimationName].desc_);
	}


protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void SetTextureRendererSetting();

private:
	void FrameDataReset();	//CurAnimationReset()�Լ��� ���� ������??

private:
	PivotMode pivotMode_;				//���� ������� �ؽ�ó�� ������ ������.
	GameEngineTexture* currentTexture_;	//���� ������� �ؽ�ó.
	float4 frameData_;					//���� ������� �ؽ�ó���� �������Ǵ� ���� ����.

	std::map<std::string, FrameAnimation> allAnimations_;	//<-�� ����??
	FrameAnimation* currentAnimation_;
};

