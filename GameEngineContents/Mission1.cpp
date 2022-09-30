#include "PreCompile.h"
#include "Mission1.h"
#include "Indicator.h"
#include "IndicatorBase.h"
#include "PixelIndicator.h"
#include "Soldier.h"
#include "Arabian.h"
#include "Mission1BG.h"

Mission1::Mission1()
	: mission1BG_(nullptr),
	currentFocusPointer_(nullptr),
	destFocus_(nullptr),
	destFocusVelocity_(3.f),
	isDestFocusHolding_(false),
	soldier_Mission1_(nullptr),
	arabian1_(nullptr),
	arabian2_(nullptr),
	arabian3_(nullptr),
	arabian4_(nullptr),
	arabian5_(nullptr),
	isCamelRiderDead_(false),
	isTruckDestroyed_(true)
{
}

Mission1::~Mission1()
{
}

void Mission1::Start()
{
	PixelIndicator::SetPCTexture("Mission1_PCBG.png");

	mission1BG_ = CreateActor<Mission1BG>(CollisionBodyOrder::Background, "Mission1BG");
	mission1BG_->GetTransform().SetWorldPosition(
		-GameEngineWindow::GetScale().HX(),
		-GameEngineWindow::GetScale().HY(),
		40.f);
	//윈도우 왼쪽 아래를 배경액터 위치로 설정.

	currentFocusPointer_ = Indicator::CreateIndicator<Indicator>(
		"CurrentFocusPointer",
		this->GetMainCameraActor(),
		float4::Black,
		float4(0, 0, 5),	//절대 변경하지 말 것.
		float4(15, 15, 1)
		);

	destFocus_ = CreateActor<IndicatorBase>(CollisionBodyOrder::UI, "DestFocus");
	destFocus_->SetPointerColor(float4::Yellow);
	destFocus_->GetTransform().SetWorldPosition(
		float4(3200, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));
	//시작시 카메라 위치 변경이 필요하면 여기에서.

	soldier_Mission1_ = CreateActor<Soldier>(CollisionBodyOrder::Soldier, "Soldier_Mission1");
	soldier_Mission1_->GetTransform().SetWorldPosition(3000, 200, 0);
	
#ifndef _DEBUG
	destFocus_->GetTransform().SetWorldPosition(
		float4(0, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));

	soldier_Mission1_->GetTransform().SetWorldPosition(
		500 - GameEngineWindow::GetInst()->GetScale().HIX(), 200, 0);
#endif // !_DEBUG




	arabian1_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian1");
	arabian2_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian2");
	arabian3_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian3");
	arabian4_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian4");
	arabian5_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian5");

	arabian1_->GetTransform().SetWorldPosition(925, 0, 0);
	arabian2_->GetTransform().SetWorldPosition(1025, 0, 0);
	arabian3_->GetTransform().SetWorldPosition(1600, 0, 0);
	arabian4_->GetTransform().SetWorldPosition(1900, 0, 0);
	arabian5_->GetTransform().SetWorldPosition(1925, 0, 0);


}

void Mission1::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
		currentFocusPointer_->Off();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	if (true == GameEngineInput::GetInst()->IsDown("Test"))
	{
		isCamelRiderDead_ = true;
	}


	UpdateDestFocusMovement(_deltaTime);
	UpdateCameraActorMovement(_deltaTime);
}

void Mission1::End()
{
}

void Mission1::UpdateDestFocusMovement(float _deltaTime)
{
	if (mission1BG_->GetPart1RightEnd() - GameEngineWindow::GetScale().x
		<= destFocus_->GetTransform().GetWorldPosition().x)
	{
		if (true == isCamelRiderDead_)
		{
			isDestFocusHolding_ = false;
		}
		else
		{
			isDestFocusHolding_ = true;
		}
	}

	if (mission1BG_->GetPart2RightEnd() - GameEngineWindow::GetScale().x
		<= destFocus_->GetTransform().GetWorldPosition().x)
	{
		if (true == isTruckDestroyed_)
		{
			isDestFocusHolding_ = false;
		}
		else
		{
			isDestFocusHolding_ = true;
		}
	}

	//지역보스 카메라 고정은 이 위로.

	if (true == isDestFocusHolding_)	//destFocus가 고정이면 모든 이동 무시.
	{
		return;
	}



	if (destFocus_->GetTransform().GetWorldPosition().x
		<= soldier_Mission1_->GetTransform().GetWorldPosition().x + 75.f)
		//destFocus 기본 이동 규칙: 솔저보다 살짝 앞서 나갈것.
	{
		if (PixelIndicator::GetPCTexture()->GetScale().x - GameEngineWindow::GetScale().x
			>= destFocus_->GetTransform().GetWorldPosition().x)		//pc텍스처는 넘어가지 말 것.
		{
			//destFocus_->GetTransform().SetWorldMove(
			//	float4::Right * _deltaTime * destFocusVelocity_ * playSpeed_);
			
			float4 destFocusWorldPosition = float4::Black;

			destFocusWorldPosition.x = GameEngineMath::LerpTime(
				soldier_Mission1_->GetTransform().GetWorldPosition().x + 75.f,
				destFocus_->GetTransform().GetWorldPosition().x,
				_deltaTime
			);
			destFocusWorldPosition.y = destFocus_->GetTransform().GetWorldPosition().y;
			destFocusWorldPosition.z = destFocus_->GetTransform().GetWorldPosition().z;

			destFocus_->GetTransform().SetWorldPosition(destFocusWorldPosition);
		}
	}

	if (3700.f <= destFocus_->GetTransform().GetWorldPosition().x 
		&& 4100.f >= destFocus_->GetTransform().GetWorldPosition().x)
	{
		float4 destFocusWorldPosition = float4::Black;

		destFocusWorldPosition.x = destFocus_->GetTransform().GetWorldPosition().x;
		destFocusWorldPosition.y = GameEngineMath::Lerp(
			0.f,
			60.f,
			(destFocus_->GetTransform().GetWorldPosition().x - 3700.f) * 0.0025f
			//0.0025f == 1 / (4100.f - 3700.f)
		);
		destFocusWorldPosition.z = destFocus_->GetTransform().GetWorldPosition().z;

		destFocus_->GetTransform().SetWorldPosition(destFocusWorldPosition);
	}

}

void Mission1::UpdateCameraActorMovement(float _deltaTime)
{
	GetMainCameraActor()->GetTransform().SetWorldPosition(
		float4::Lerp(
			GetMainCameraActor()->GetTransform().GetWorldPosition(),
			destFocus_->GetTransform().GetWorldPosition(),
			_deltaTime * 10.0f
		)
	);
}
