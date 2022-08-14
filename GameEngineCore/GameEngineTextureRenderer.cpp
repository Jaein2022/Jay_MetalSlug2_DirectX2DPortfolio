#include "PreCompile.h"
#include "GameEngineTextureRenderer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"

void FrameAnimation::Reset()
{
	info_.frameTime_ = 0.f;
	info_.curFrame_ = 0;
}

void FrameAnimation::Update(float _deltaTime)
{
	info_.frameTime_ += _deltaTime;

	if (nullptr != time_)
	{
		this->time_(info_, _deltaTime);
	}

	if (false == bOnceStart_
		&& info_.curFrame_ == 0)
	{
		if (nullptr != start_)
		{
			this->start_(info_);
		}

		bOnceStart_ = true;
		bOnceEnd_ = false;
	}

	if (info_.interval_ <= info_.frameTime_)
	{
		if (info_.curFrame_ == info_.frames_.size() - 1 
			&& false == bOnceEnd_ && nullptr != end_)
		{
			end_(info_);
			bOnceStart_ = false;
			bOnceEnd_ = true;
		}

		++info_.curFrame_;
		if (nullptr != frame_)
		{
			frame_(info_);
		}

		if (info_.curFrame_ >= info_.frames_.size())
		{
			if (true == info_.isLoop_)
			{
				info_.curFrame_ = 0;
			}
			else
			{
				info_.curFrame_ = static_cast<UINT>(info_.frames_.size() - 1);
			}
		}

		if (nullptr != cutTexture_)
		{
			parentRenderer_->currentTexture_ = cutTexture_;
			parentRenderer_->SetTexture(cutTexture_, info_.frames_[info_.curFrame_]);		
			parentRenderer_->SetPivot();

			if (0 != cutTexture_->GetCutCount())
			{
				if (ScaleMode::Image == parentRenderer_->scaleMode_)
				{
					parentRenderer_->ScaleToCutTexture(info_.frames_[info_.curFrame_]);
				}
			}
			else
			{
				if (ScaleMode::Image == parentRenderer_->scaleMode_)
				{
					parentRenderer_->ScaleToTexture();
				}
			}
		}
		else if (nullptr != folderTexture_)
		{
			parentRenderer_->FrameDataReset();
			parentRenderer_->currentTexture_ = folderTexture_->GetTexture(info_.frames_[info_.curFrame_]);
			parentRenderer_->SetTexture(folderTexture_->GetTexture(info_.frames_[info_.curFrame_]));
			parentRenderer_->SetPivot();
			if (ScaleMode::Image == parentRenderer_->scaleMode_)
			{
				parentRenderer_->ScaleToTexture();
			}
		}
		else
		{
			MsgBoxAssert("텍스쳐가 준비되지 않았습니다.");
			return;
		}

		info_.frameTime_ -= info_.interval_;
	}

}

GameEngineTextureRenderer::GameEngineTextureRenderer()
	: currentTexture_(nullptr),
	currentAnimation_(nullptr),
	pivotMode_(PivotMode::Center),
	scaleMode_(ScaleMode::Image),
	scaleRatio_(1.f)
{
}

GameEngineTextureRenderer::~GameEngineTextureRenderer()
{
}

void GameEngineTextureRenderer::SetTexture(const std::string& _textureName)
{
	this->currentTexture_ = this->shaderResources_.SetTexture("Tex", _textureName);
}

void GameEngineTextureRenderer::SetTexture(GameEngineTexture* _texture)
{
	currentTexture_ = _texture;
	this->shaderResources_.SetTexture("Tex", _texture);
}

void GameEngineTextureRenderer::SetTexture(const std::string& _textureName, int _index)
{
	this->SetTexture(_textureName);
	this->SetFrame(_index);
}

void GameEngineTextureRenderer::SetTexture(GameEngineTexture* _texture, int _index)
{
	if (nullptr == _texture)
	{
		MsgBoxAssert("존재하지 않는 텍스쳐입니다.");
		return;
	}

	SetTexture(_texture);
	SetFrame(_index);
}

void GameEngineTextureRenderer::SetFrame(int _index)
{
	this->frameData_ = currentTexture_->GetFrameData(_index);
}

void GameEngineTextureRenderer::SetPivot()
{
	SetPivot(pivotMode_);
}

void GameEngineTextureRenderer::SetPivot(PivotMode _pivot)
{
	switch (_pivot)
	{
	case PivotMode::Center:
		SetPivotToVector(float4::Zero);
		break;


	case PivotMode::LeftTop:
		SetPivotToVector(
			float4(this->GetTransform().GetWorldScale().HX(), -this->GetTransform().GetWorldScale().HY())
		);
		break;


	case PivotMode::Bot:
		SetPivotToVector(
			float4(0.f, this->GetTransform().GetWorldScale().HY())
		);
		break;

	case PivotMode::Custom:
		//_pivot == Custom일때는 아무것도 하지 않는다.
		break;



	default:
		MsgBoxAssert("피봇모드가 없습니다.");
		break;
	}

	pivotMode_ = _pivot;
}

void GameEngineTextureRenderer::SetPivotToVector(const float4& _localPos)
{
	this->GetTransform().SetLocalPosition(_localPos);
}

void GameEngineTextureRenderer::SetSamplingMode_Point()
{
	this->shaderResources_.SetSampler("Smp", "EngineSampler_Point");
}

void GameEngineTextureRenderer::SetSamplingMode_Linear()
{
	this->shaderResources_.SetSampler("Smp", "EngineSampler_Linear");
}

void GameEngineTextureRenderer::CreateFrameAnimation_CutTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc)
{
	std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

	if (allAnimations_.end() != allAnimations_.find(uppercaseAnimationName))
	{
		MsgBoxAssertString(_animationName + ": 같은 이름의 애니메이션이 이미 존재합니다.");
		return;
	}

	FrameAnimation& newAnimation = allAnimations_[uppercaseAnimationName];	//생성과 동시에 삽입.
	newAnimation.info_ = _desc;
	newAnimation.parentRenderer_ = this;
	newAnimation.cutTexture_ = GameEngineTexture::Find(newAnimation.info_.textureName_);
	newAnimation.folderTexture_ = nullptr;
}

void GameEngineTextureRenderer::CreateFrameAnimation_FolderTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc)
{
	std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

	if (allAnimations_.end() != allAnimations_.find(uppercaseAnimationName))
	{
		MsgBoxAssertString(_animationName + ": 같은 이름의 애니메이션이 이미 존재합니다.");
		return;
	}

	FrameAnimation& newAnimation = allAnimations_[uppercaseAnimationName];	//생성과 동시에 삽입.
	newAnimation.info_ = _desc;
	newAnimation.parentRenderer_ = this;
	newAnimation.cutTexture_ = nullptr;
	newAnimation.folderTexture_ = GameEngineFolderTexture::Find(newAnimation.info_.textureName_);

	if (0 == newAnimation.info_.frames_.size())
	{
		for (UINT textureIndex = 0; textureIndex < newAnimation.folderTexture_->GetTextureCount();
			textureIndex++)
		{
			newAnimation.info_.frames_.push_back(textureIndex);
		}
	}
}

void GameEngineTextureRenderer::ChangeFrameAnimation(const std::string& _animationName)
{
	std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

	if (allAnimations_.end() == allAnimations_.find(uppercaseAnimationName))
	{
		MsgBoxAssertString(_animationName + ": 그런 이름의 애니메이션이 존재하지 않습니다.");
		return;
	}

	if (this->currentAnimation_ != &allAnimations_[uppercaseAnimationName])
	{
		this->currentAnimation_ = &allAnimations_[uppercaseAnimationName];
		this->currentAnimation_->Reset();
		if (nullptr != currentAnimation_->cutTexture_)
		{
			SetTexture(
				currentAnimation_->cutTexture_,
				currentAnimation_->info_.frames_[currentAnimation_->info_.curFrame_]);
			if (ScaleMode::Image == this->scaleMode_)
			{
				ScaleToCutTexture(currentAnimation_->info_.frames_[currentAnimation_->info_.curFrame_]);
			}
		}
		else if (nullptr != currentAnimation_->folderTexture_)
		{
			SetTexture(
				currentAnimation_->folderTexture_->GetTexture(
					currentAnimation_->info_.frames_[currentAnimation_->info_.curFrame_]));
			if (ScaleMode::Image == this->scaleMode_)
			{
				ScaleToTexture();
			}
		}
	}
}

void GameEngineTextureRenderer::ScaleToTexture()
{
	float4 scale = currentTexture_->GetScale();

	if (0 > this->GetTransform().GetLocalScale().x)
	{
		scale.x = -scale.x;
	}

	if (0 > this->GetTransform().GetLocalScale().y)
	{
		scale.y = -scale.y;
	}

	this->GetTransform().SetLocalScale(scale * scaleRatio_);
}

void GameEngineTextureRenderer::ScaleToCutTexture(int _index)
{
	float4 scale = currentTexture_->GetCutScale(_index);

	if (0 > this->GetTransform().GetLocalScale().x)
	{
		scale.x = -scale.x;
	}

	if (0 > this->GetTransform().GetLocalScale().y)
	{
		scale.y = -scale.y;
	}

	this->GetTransform().SetLocalScale(scale * scaleRatio_);
}

void GameEngineTextureRenderer::CurAnimationReset()
{
	CurAnimationSetStartPivotFrame(0);
}

void GameEngineTextureRenderer::CurAnimationSetStartPivotFrame(int _setFrame)
{
	currentAnimation_->info_.curFrame_ = _setFrame;
}

GameEngineTexture* GameEngineTextureRenderer::GetCurrentTexture() const
{
	return currentTexture_;
}

void GameEngineTextureRenderer::Start()
{
	GameEngineDefaultRenderer::Start();

	GameEngineRenderer::PushRendererToMainCamera();

	this->SetTextureRendererSetting();
}

void GameEngineTextureRenderer::Update(float _deltaTime)
{
	if (nullptr != this->currentAnimation_)
	{
		currentAnimation_->Update(_deltaTime);
	}
}

void GameEngineTextureRenderer::SetTextureRendererSetting()
{
	this->SetPipeLine("TextureAtlas");

	frameData_.posX = 0.f;
	frameData_.posY = 0.f;
	frameData_.sizeX = 1.f;
	frameData_.sizeY = 1.f;

	this->shaderResources_.SetConstantBuffer_Link("AtlasData", frameData_);
	this->shaderResources_.SetConstantBuffer_Link("ColorData", colorData_);
}

void GameEngineTextureRenderer::FrameDataReset()
{
	this->frameData_.posX = 0.f;
	this->frameData_.posY = 0.f;
	this->frameData_.sizeX = 1.f;
	this->frameData_.sizeY = 1.f;
}
