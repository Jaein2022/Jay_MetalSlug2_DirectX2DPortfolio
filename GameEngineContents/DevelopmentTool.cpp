#include "PreCompile.h"
#include "DevelopmentTool.h"
#include "Mission1.h"
#include "Truck.h"

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
	if (true == ImGui::Button("On/Off Switch"))
	{
		if (true == mission1_->troopTruck_->truckTarpRenderer_->IsUpdate())
		{
			mission1_->troopTruck_->truckTarpRenderer_->Off();
		}
		else
		{
			mission1_->troopTruck_->truckTarpRenderer_->On();
		}
	}
}
