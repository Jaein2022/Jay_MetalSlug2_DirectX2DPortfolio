#include "PreCompile.h"
#include "Arabian.h"
#include "TestLevel.h"

void Arabian::CreateArabianAnimations()
{
	arabianRenderer_ = CreateComponent<GameEngineTextureRenderer>("ArabianRenderer");
	arabianRenderer_->GetTransform().SetLocalScale(600, 600, 1);
	arabianRenderer_->SetPivot(PivotMode::Center);
	arabianRenderer_->GetTransform().SetLocalPosition(arabianRendererLocalPos_);

	if (0 == GameEngineTexture::Find("Rebel_Arabian.png")->GetCutCount())
	{
		GameEngineTexture::Cut("Rebel_Arabian.png", 10, 18);
	}
	arabianRenderer_->SetTexture("Rebel_Arabian.png");
	arabianRenderer_->CreateFrameAnimation_CutTexture("Idling",
		FrameAnimation_Desc("Rebel_Arabian.png", 0, 8, 0.15f, true)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("PreparingToAttack",
		FrameAnimation_Desc("Rebel_Arabian.png", 10, 15, 0.1f, false)
	);
	arabianRenderer_->AnimationBindEnd("PreparingToAttack",
		[this](const FrameAnimation_Desc& _desc)->void 
	{
			currentArabianState_ = ArabianState::Shuffling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Shuffling",
		FrameAnimation_Desc("Rebel_Arabian.png", 20, 25, 0.1f, true)
	);
	arabianRenderer_->AnimationBindEnd("Shuffling",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			shuffleDirection_ = -shuffleDirection_;

			ReactInShuffling();
		}
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("Running",
		FrameAnimation_Desc("Rebel_Arabian.png", 30, 41, 0.05f, true)
	);
	arabianRenderer_->AnimationBindTime("Running",
		std::bind(&Arabian::Run, this)
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Jumping",
		FrameAnimation_Desc("Rebel_Arabian.png", 50, 57, 0.1f, true)
	);
	arabianRenderer_->AnimationBindTime("Jumping",
		std::bind(&Arabian::Jump, this, std::placeholders::_1)
	);
	arabianRenderer_->AnimationBindEnd("Jumping", 
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			currentArabianState_ = ArabianState::Falling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Falling",
		FrameAnimation_Desc("Rebel_Arabian.png", 58, 58, 1.f, true)
	);
	arabianRenderer_->CreateFrameAnimation_CutTexture("FallingToIdling",
		FrameAnimation_Desc("Rebel_Arabian.png", 59, 63, 0.1f, false)
	);
	arabianRenderer_->AnimationBindEnd("FallingToIdling",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			currentArabianState_ = ArabianState::Idling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("JumpingBackward",
		FrameAnimation_Desc("Rebel_Arabian.png", 70, 74, 0.1f, false)
	);
	arabianRenderer_->AnimationBindTime("JumpingBackward",
		std::bind(&Arabian::JumpBackWard, this)
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Turning",
		FrameAnimation_Desc("Rebel_Arabian.png", 80, 83, 0.1f, false)
	);
	arabianRenderer_->AnimationBindEnd("Turning",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			currentArabianState_ = ArabianState::Shuffling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("ThrowingSword&Reloading",
		FrameAnimation_Desc("Rebel_Arabian.png", 90, 107, 0.1f, true)
	);
	arabianRenderer_->AnimationBindFrame("ThrowingSword&Reloading",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			if (7 == _desc.curFrame_)
			{
				ThrowSword();
			}
		}
	);
	arabianRenderer_->AnimationBindEnd("ThrowingSword&Reloading",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			currentArabianState_ = ArabianState::Shuffling;
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("MeleeAttack",
		FrameAnimation_Desc("Rebel_Arabian.png", 110, 117, 0.05f, true)
	);
	arabianRenderer_->AnimationBindFrame("MeleeAttack",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			if (1 == _desc.curFrame_)
			{
				MeleeAttack();
			}
		}
	);
	arabianRenderer_->AnimationBindEnd("MeleeAttack",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			SelectNextState(0, 2, 0);
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("JumpDeath",
		FrameAnimation_Desc("Rebel_Arabian.png", 120, 130, 0.075f, true)
	);
	arabianRenderer_->AnimationBindFrame("JumpDeath",
		std::bind(&Arabian::MoveInJumpDeath, this, std::placeholders::_1)
	);
	arabianRenderer_->AnimationBindEnd("JumpDeath",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			this->Death();
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Death1",
		FrameAnimation_Desc("Rebel_Arabian.png", 140, 151, 0.075f, true)
	);
	arabianRenderer_->AnimationBindEnd("Death1",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			this->Death();
		}
	);

	arabianRenderer_->CreateFrameAnimation_CutTexture("Death2",
		FrameAnimation_Desc("Rebel_Arabian.png", 160, 179, 0.075f, true)
	);
	arabianRenderer_->AnimationBindEnd("Death2",
		[this](const FrameAnimation_Desc& _desc)->void 
		{
			this->Death();
		}
	);


	arabianRenderer_->ChangeFrameAnimation("Idling");
}

void Arabian::CreateArabianStates()
{
	arabianStateManager_.CreateState(
		"Idling",
		[this](float _deltaTime, const StateInfo& _info)->void 
		{
			if (true == isEngaging_)
			{
				currentArabianState_ = ArabianState::Shuffling;
			}
		},
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Idling");
		}
		);

	arabianStateManager_.CreateState(
		"Shuffling",
		std::bind(&Arabian::Shuffle, this),
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Shuffling");
		},
		[this](const StateInfo& _info)->void 
		{
			shuffleDirection_ = 1;
		}
		);
	arabianStateManager_.CreateState(
		"PreparingToAttack",
		[this](float _deltaTime, const StateInfo& _info)->void 
		{

			if (true == arabianCloseCombatCollisionBody_->IsCollision(
				CollisionType::CT_AABB,
				CollisionBodyOrder::Soldier,
				CollisionType::CT_AABB)
				)
			{
				currentArabianState_ = ArabianState::MeleeAttack;
			}
		},
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("PreparingToAttack");
		}
		);

	arabianStateManager_.CreateState(
		"Running",
		[this](float _deltaTime, const StateInfo& _info)->void 
		{

			if (true == arabianCloseCombatCollisionBody_->IsCollision(
				CollisionType::CT_AABB,
				CollisionBodyOrder::Soldier,
				CollisionType::CT_AABB)
				)
			{
				currentArabianState_ = ArabianState::MeleeAttack;
				return;
			}

			if (true == isArabiansDirectionWrong_)
			{
				currentArabianState_ = ArabianState::Turning;
				return;
			}

			if (engagementDistance_ > horizontalDistance_ && chargeDistance_ < horizontalDistance_)
			{
				SelectNextState(0, 4, 1, ArabianState::Running);
			}
		},
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Running");
		}
	);

	arabianStateManager_.CreateState(
		"Jumping",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Jumping");
		}
	);

	arabianStateManager_.CreateState(
		"Falling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Falling");
		}
	);
	arabianStateManager_.CreateState(
		"FallingToIdling",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("FallingToIdling");
		},
		[this](const StateInfo& _info)->void 
		{
			if (_info.prevState_ == allArabianStates_[ArabianState::Running])
			{
				currentArabianState_ = ArabianState::Running;
			}
			else
			{
				currentArabianState_ = ArabianState::Shuffling;
			}
		}

	);
	arabianStateManager_.CreateState(
		"JumpingBackward",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("JumpingBackward");
			if (false == isAirborne_)
			{
				isAirborne_ = true;
				fallingSpeed_ = -2.5f;
			}
		}
	);
	arabianStateManager_.CreateState(
		"Turning",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("Turning");
		},
		[this](const StateInfo& _info)->void 
		{
			if (0 < this->GetTransform().GetWorldScale().x)
			{
				this->GetTransform().PixLocalNegativeX();
				isArabiansDirectionWrong_ = false;
			}
			else if (0 > this->GetTransform().GetWorldScale().x)
			{
				this->GetTransform().PixLocalPositiveX();
				isArabiansDirectionWrong_ = false;
			}
		}
	);

	arabianStateManager_.CreateState(
		"ThrowingSword",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("ThrowingSword&Reloading");
		}
	);
	arabianStateManager_.CreateState(
		"MeleeAttack",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianRenderer_->ChangeFrameAnimation("MeleeAttack");
		}
	);
	arabianStateManager_.CreateState(
		"Dead",
		nullptr,
		[this](const StateInfo& _info)->void 
		{
			arabianLifeCollisionBody_->Off();
			arabianCloseCombatCollisionBody_->Off();

			if (0 == GameEngineRandom::mainRandom_.GetRandomInt(0, 1))
			{
				GameEngineSound::SoundPlayOneshot("Rebel_Death1.mp3");
			}
			else
			{
				GameEngineSound::SoundPlayOneshot("Rebel_Death2.mp3");
			}

			if (true == isAirborne_)
			{
				int deathAnimationSelection = GameEngineRandom::mainRandom_.GetRandomInt(0, 1);

				switch (deathAnimationSelection)
				{
				case 0:
				{
					arabianRenderer_->ChangeFrameAnimation("JumpDeath");
					break;
				}
				case 1:
				{
					arabianRenderer_->ChangeFrameAnimation("Death1");
					break;
				}

				default:
					MsgBoxAssertString(std::to_string(deathAnimationSelection) + ": deathAnimationSelection의 범위가 잘못되었습니다.");
					return;
				}
			}
			else
			{
				int deathAnimationSelection = GameEngineRandom::mainRandom_.GetRandomInt(0, 2);

				switch (deathAnimationSelection)
				{
				case 0:
				{
					arabianRenderer_->ChangeFrameAnimation("JumpDeath");
					break;
				}
				case 1:
				{
					arabianRenderer_->ChangeFrameAnimation("Death1");
					break;
				}
				case 2:
				{
					arabianRenderer_->ChangeFrameAnimation("Death2");
					break;
				}

				default:
					MsgBoxAssertString(std::to_string(deathAnimationSelection) + ": deathAnimationSelection의 범위가 잘못되었습니다.");
					return;
				}
			}
		}
	);

	std::vector<std::pair<ArabianState, std::string_view>> arabianStateEntries(
		magic_enum::enum_entries<ArabianState>().begin(), magic_enum::enum_entries<ArabianState>().end()
	);

	size_t arabianStateCount = arabianStateEntries.size();
	allArabianStates_.reserve(arabianStateCount);

	for (size_t i = 0; i < arabianStateCount; i++)
	{
		if (allArabianStates_.end() != allArabianStates_.find(arabianStateEntries[i].first))
		{
			MsgBoxAssertString(std::string(arabianStateEntries[i].second) + std::string(": 이미 존재하는 스테이트입니다."));
			return;
		}

		allArabianStates_.insert(
			std::make_pair(
				arabianStateEntries[i].first, arabianStateEntries[i].second.data()
			)
		);
	}

	arabianStateManager_.ChangeState(allArabianStates_[currentArabianState_]);
}
