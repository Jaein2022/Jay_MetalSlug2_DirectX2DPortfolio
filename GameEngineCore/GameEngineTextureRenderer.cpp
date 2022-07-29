#include "PreCompile.h"
#include "GameEngineTextureRenderer.h"
#include "GameEngineTexture.h"
#include "GameEngineFolderTexture.h"

void FrameAnimation::Reset()
{
	desc_.frameTime_ = 0.f;
	desc_.curFrame_ = desc_.start_;
}

void FrameAnimation::Update(float _deltaTime)
{
	desc_.frameTime_ += _deltaTime;

	if (nullptr != time_)
	{
		this->time_(desc_, _deltaTime);
	}

	if (false == bOnceStart_
		&& desc_.curFrame_ == desc_.start_)
	{
		if (nullptr != start_)
		{
			this->start_(desc_);
		}

		bOnceStart_ = true;
		bOnceEnd_ = false;
	}

	if (desc_.interval_ <= desc_.frameTime_)
	{
		++desc_.curFrame_;
		if (nullptr != frame_)
		{
			frame_(desc_);
		}

		if (desc_.curFrame_ > desc_.end_)
		{
			if (false == bOnceEnd_ && nullptr != end_)
			{
				end_(desc_);
				bOnceStart_ = false;
				bOnceEnd_ = true;
			}

			if (true == desc_.isLoop_)
			{
				desc_.curFrame_ = desc_.start_;
			}
			else
			{
				desc_.curFrame_ = desc_.end_;
			}
		}

		if (nullptr != atlasTexture_)
		{
			parentRenderer_->currentTexture_ = atlasTexture_;
			parentRenderer_->SetTexture(atlasTexture_, desc_.curFrame_);
			parentRenderer_->SetPivot();
		}
		else if (nullptr != folderTexture_)
		{
			parentRenderer_->FrameDataReset();
			parentRenderer_->currentTexture_ = folderTexture_->GetTexture(desc_.curFrame_);
			parentRenderer_->SetTexture(folderTexture_->GetTexture(desc_.curFrame_));
			parentRenderer_->SetPivot();
		}
		else
		{
			MsgBoxAssert("텍스쳐가 준비되지 않았습니다.");
			return;
		}

		desc_.frameTime_ -= desc_.interval_;
	}

}

GameEngineTextureRenderer::GameEngineTextureRenderer()
	: currentTexture_(nullptr),
	currentAnimation_(nullptr),
	pivotMode_(PivotMode::Custom)
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

void GameEngineTextureRenderer::CreateFrameAnimation_AtlasTexture(const std::string& _animationName, const FrameAnimation_Desc& _desc)
{
	std::string uppercaseAnimationName = GameEngineString::ToUpperReturn(_animationName);

	if (allAnimations_.end() != allAnimations_.find(uppercaseAnimationName))
	{
		MsgBoxAssertString(_animationName + ": 같은 이름의 애니메이션이 이미 존재합니다.");
		return;
	}

	FrameAnimation& newAnimation = allAnimations_[uppercaseAnimationName];	//생성과 동시에 삽입.
	newAnimation.desc_ = _desc;
	newAnimation.parentRenderer_ = this;
	newAnimation.atlasTexture_ = GameEngineTexture::Find(newAnimation.desc_.textureName_);
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
	newAnimation.desc_ = _desc;
	newAnimation.parentRenderer_ = this;
	newAnimation.atlasTexture_ = nullptr;
	newAnimation.folderTexture_ = GameEngineFolderTexture::Find(newAnimation.desc_.textureName_);


	if (-1 == newAnimation.desc_.start_)
	{
		newAnimation.desc_.start_ = 0;
	}

	if (-1 == newAnimation.desc_.end_)
	{
		newAnimation.desc_.end_ = static_cast<UINT>(newAnimation.folderTexture_->GetTextureCount()) - 1;
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
	else
	{
		this->currentAnimation_ = &allAnimations_[uppercaseAnimationName];
		this->currentAnimation_->Reset();
		if (nullptr != currentAnimation_->atlasTexture_)
		{
			SetTexture(currentAnimation_->atlasTexture_, currentAnimation_->desc_.curFrame_);
		}
		else if (nullptr != currentAnimation_->folderTexture_)
		{
			SetTexture(currentAnimation_->folderTexture_->GetTexture(currentAnimation_->desc_.curFrame_));
		}
	}
}

void GameEngineTextureRenderer::ScaleToTexture()
{
	this->GetTransform().SetLocalScale(currentTexture_->GetScale());
}

void GameEngineTextureRenderer::CurAnimationReset()
{
	CurAnimationStartPivotFrame(currentAnimation_->desc_.start_);
}

void GameEngineTextureRenderer::CurAnimationStartPivotFrame(int _setFrame)
{
	currentAnimation_->desc_.curFrame_ += currentAnimation_->desc_.start_ + _setFrame;
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

	this->shaderResources_.SetConstantBufferLink("AtlasData", frameData_);
}

void GameEngineTextureRenderer::FrameDataReset()
{
	this->frameData_.posX = 0.f;
	this->frameData_.posY = 0.f;
	this->frameData_.sizeX = 1.f;
	this->frameData_.sizeY = 1.f;
}
