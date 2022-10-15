#include "PreCompile.h"
#include "DevelopmentTool.h"
#include "Mission1.h"
#include "Truck.h"
#include "Berserker.h"

DevelopmentTool::DevelopmentTool(): mission1_(nullptr)
{
}

DevelopmentTool::~DevelopmentTool()
{
}

void DevelopmentTool::Initialize(GameEngineLevel* _level)
{
	mission1_ = reinterpret_cast<Mission1*>(_level);
}

void DevelopmentTool::OnGUI(GameEngineLevel* _level, float _deltaTime)
{

	if (true == ImGui::Button("Waiting_SittingDown"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::Waiting_SittingDown);
	}	
	
	if (true == ImGui::Button("Waiting_Standing"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::Waiting_Standing);
	}	
	
	if (true == ImGui::Button("Idling"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::Idling);
	}	
	
	if (true == ImGui::Button("Running"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::Running);
	}	
	
	if (true == ImGui::Button("Turning"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::Turning);
	}	
	
	if (true == ImGui::Button("MeleeAttack"))
	{
		this->mission1_->berserker1_->SetBerserkerState(BerserkerState::MeleeAttack);
	}	
}
