#include "PreCompile.h"
#include "Mission1.h"
#include "Indicator.h"
#include "IndicatorBase.h"
#include "PixelIndicator.h"
#include "Soldier.h"
#include "Arabian.h"
#include "Mission1BG.h"
#include "CamelRider.h"
#include "UI.h"

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
	arabian6_(nullptr),
	arabian7_(nullptr),
	arabian8_(nullptr),
	arabian9_(nullptr),
	arabian10_(nullptr),
	arabian11_(nullptr),
	arabian12_(nullptr),
	arabian13_(nullptr),
	arabian14_(nullptr),
	arabian15_(nullptr),
	arabian16_(nullptr),
	arabian17_(nullptr),
	arabian18_(nullptr),
	arabian19_(nullptr),
	arabian20_(nullptr),
	camelRider_(nullptr),
	isCamelRiderDead_(false),
	isTruckDestroyed_(false),
	ui_(nullptr)
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
		float4(3100, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));
	//시작시 카메라 위치 변경이 필요하면 여기에서.

	soldier_Mission1_ = CreateActor<Soldier>(CollisionBodyOrder::Soldier, "Soldier_Mission1");
	soldier_Mission1_->GetTransform().SetWorldPosition(3000, 200, 0);


	arabian1_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian1");
	arabian2_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian2");
	arabian3_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian3");
	arabian4_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian4");
	arabian5_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian5");	

	
	arabian1_->GetTransform().SetWorldPosition(925, 0, 0);
	arabian2_->GetTransform().SetWorldPosition(1025, 0, 0);
	arabian3_->GetTransform().SetWorldPosition(1600, 0, 0);
	arabian4_->GetTransform().SetWorldPosition(1700, 0, 0);
	arabian5_->GetTransform().SetWorldPosition(1725, 0, 0);


#ifndef _DEBUG
	destFocus_->GetTransform().SetWorldPosition(
		float4(0, 0, GetMainCameraActorTransform().GetWorldPosition().IZ()));

	soldier_Mission1_->GetTransform().SetWorldPosition(
		500 - GameEngineWindow::GetInst()->GetScale().HIX(), 200, 0);

	arabian6_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian6");
	arabian7_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian7");
	arabian8_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian8");
	arabian9_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian9");
	arabian10_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian10");

	arabian6_->GetTransform().SetWorldPosition(1850, 0, 0);
	arabian7_->GetTransform().SetWorldPosition(1925, 0, 0);
	arabian8_->GetTransform().SetWorldPosition(2200, 0, 0);
	arabian9_->GetTransform().SetWorldPosition(2525, 0, 0);
	arabian10_->GetTransform().SetWorldPosition(2600, 0, 0);

	arabian11_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian11");
	arabian12_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian12");
	arabian13_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian13");
	arabian14_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian14");
	arabian15_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian15");
	arabian16_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian6");
	arabian17_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian7");
	arabian18_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian8");
	arabian19_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian9");
	arabian20_ = CreateActor<Arabian>(CollisionBodyOrder::Rebel, "Arabian10");

	arabian11_->GetTransform().SetWorldPosition(3210, 0, 0);
	arabian12_->GetTransform().SetWorldPosition(3250, 0, 0);
	arabian13_->GetTransform().SetWorldPosition(3400, 0, 0);
	arabian14_->GetTransform().SetWorldPosition(3450, 0, 0);
	arabian15_->GetTransform().SetWorldPosition(3550, 0, 0);
	arabian16_->GetTransform().SetWorldPosition(3700, 0, 0);
	arabian17_->GetTransform().SetWorldPosition(3825, 0, 0);
	arabian18_->GetTransform().SetWorldPosition(3850, 0, 0);
	arabian19_->GetTransform().SetWorldPosition(3875, 0, 0);
	arabian20_->GetTransform().SetWorldPosition(3900, 0, 0);

#endif // !_DEBUG


	camelRider_ = CreateActor<CamelRider>(CollisionBodyOrder::Rebel, "CamelRider");
	camelRider_->GetTransform().PixLocalNegativeX();

	camelRider_->GetTransform().SetWorldPosition(mission1BG_->GetPart1RightEnd() - 575.f, 0.f, 0.f);



	ui_ = CreateActor<UI>(CollisionBodyOrder::UI, "UI");
	ui_->GetTransform().SetWorldPosition(0, 0, -5);




	GameEngineSound::SoundPlayOneshot("Mission1_Start.mp3");
	mission1BgmPlayer_ = GameEngineSound::SoundPlayControl("JUDGMENT (Mission 1).mp3", -1);





}

void Mission1::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
		currentFocusPointer_->Off();
		destFocus_->Off();
	}

	if (true == camelRider_->IsDead() && false == isCamelRiderDead_)
	{
		isCamelRiderDead_ = true;
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
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
			if (false == isDestFocusHolding_)
			{
				camelRider_->SelectNextState(0, 0, 0);
			}
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
			
			float4 destFocusWorldPosition = float4::Black;

			destFocusWorldPosition.x = GameEngineMath::LerpTime(
				soldier_Mission1_->GetTransform().GetWorldPosition().x + 75.f,
				destFocus_->GetTransform().GetWorldPosition().x,
				_deltaTime
			);
			destFocusWorldPosition.y = destFocus_->GetTransform().GetWorldPosition().y;
			if (4100.f < destFocus_->GetTransform().GetWorldPosition().x)
			{
				destFocusWorldPosition.y = 60.f;
			}

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
