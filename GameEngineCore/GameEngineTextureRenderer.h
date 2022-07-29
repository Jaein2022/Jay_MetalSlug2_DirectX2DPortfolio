#pragma once
#include "GameEngineDefaultRenderer.h"

enum class PivotMode
{
	Center,		//중심.
	LeftTop,	//좌상단.
	Bot,		//하단중앙.
	Custom,		//임의지점.
	None
};

class FrameAnimation_Desc
{
	//프레임 애니메이션 생성에 필요한 각종 상세정보들을 모아서 저장, 관리하는 클래스.
public:
	std::string textureName_;

	bool isLoop_;		//true == 애니메이션 무한반복. false == 1회반복 후 마지막프레임에서 정지.
	float interval_;	//프레임간 시간간격. 기본값 0.1f

	UINT start_;	//시작 프레임 인덱스.
	UINT curFrame_;	//현재 프레임 인덱스.
	UINT end_;		//마지막 프레임 인덱스.

	float frameTime_;//현재프레임 지난 시간.



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
	//프레임 텍스쳐 및 애니메이션 렌더링 클래스.

	friend class GameEngineTextureRenderer;
	//

	FrameAnimation_Desc desc_;	//애니메이션 생성시 필요한 상세정보 모음.

	GameEngineTextureRenderer* parentRenderer_;	//부모 렌더러.

	GameEngineTexture* atlasTexture_;			//단일 텍스쳐.			
	GameEngineFolderTexture* folderTexture_;	//폴더단위 텍스쳐들 모음.
	//두개가 동시에 사용되는 경우는 없음!

	bool bOnceStart_;
	bool bOnceEnd_;

	std::function<void(const FrameAnimation_Desc&)> start_;		//애니메이션 재생 시작시 호출되는 함수들.
	std::function<void(const FrameAnimation_Desc&)> frame_;		//애니메이션 재생중 매 프레임마다 호출되는 함수들.
	std::function<void(const FrameAnimation_Desc&, float)> time_;	//애니메이션 재생중 일정 시간 후에 호출되는 함수들.
	std::function<void(const FrameAnimation_Desc&)> end_;			//애니메이션 재생 종료시 호출되는 함수들.

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
	//특정 텍스처나 그 텍스처로 만든 애니메이션을 렌더링하는 렌더러.

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
	void SetTexture(const std::string& _textureName);	//폴더텍스처용.
	void SetTexture(GameEngineTexture* _texture);		//폴더텍스처용.
	void SetTexture(const std::string& _textureName, int _index);	//아틀라스텍스처용.
	void SetTexture(GameEngineTexture* _texture, int _index);		//아틀라스텍스처용.

	void SetFrame(int _index);	//애니메이션의 특정 프레임 지정.

	void SetPivot();
	void SetPivot(PivotMode _pivot);
	void SetPivotToVector(const float4& _localPos);

	void SetSamplingMode_Point();
	//필터가 D3D11_FILTER_MIN_MAG_MIP_POINT로 세팅된 샘플러 사용.

	void SetSamplingMode_Linear();
	//필터가 D3D11_FILTER_MIN_MAG_MIP_LINEAR로 세팅된 샘플러 사용.


	void CreateFrameAnimation_AtlasTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc);
	void CreateFrameAnimation_FolderTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc);

	void ChangeFrameAnimation(const std::string& _animationName);

	void ScaleToTexture();	//텍스처 크기에 메쉬 스케일을 자동으로 맞춰주는 함수.

	void CurAnimationReset();	//FrameDataReset()함수와 무슨 차이지? 
	void CurAnimationStartPivotFrame(int _setFrame);	//애니메이션 중 내가 원하는 프레임으로 옮기는 함수.

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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
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
			MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 없습니다.");
			return;
		}

		allAnimations_[uppercaseAnimationName].end_ = std::bind(_function, allAnimations_[uppercaseAnimationName].desc_);
	}


protected:
	void Start() override;
	void Update(float _deltaTime) override;
	void SetTextureRendererSetting();

private:
	void FrameDataReset();	//CurAnimationReset()함수와 무슨 차이지??

private:
	PivotMode pivotMode_;				//현재 사용중인 텍스처의 렌더링 기준점.
	GameEngineTexture* currentTexture_;	//현재 사용중인 텍스처.
	float4 frameData_;					//현재 사용중인 텍스처에서 렌더링되는 범위 정보.

	std::map<std::string, FrameAnimation> allAnimations_;	//<-왜 값형??
	FrameAnimation* currentAnimation_;
};

