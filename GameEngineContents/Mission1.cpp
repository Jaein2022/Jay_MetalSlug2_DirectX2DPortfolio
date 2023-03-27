#include "PreCompile.h"
#include "Mission1.h"
#include "Indicator.h"
#include "IndicatorBase.h"
#include "PixelIndicator.h"
#include "Soldier.h"
#include "Arabian.h"
#include "Mission1BG.h"
#include "CamelRider.h"
#include "Truck.h"
#include "UI.h"
#include "DevelopmentTool.h"
#include "Berserker.h"

Mission1::Mission1()
	: mission1BG_(nullptr),
	currentFocusPointer_(nullptr),
	destFocus_(nullptr),
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
	troopTruck_(nullptr),
	isTruckDestroyed_(false),
	berserker1_(nullptr),
	ui_(nullptr)
{
}

Mission1::~Mission1()
{
}

void Mission1::Start()
{
	PixelIndicator::SetPCTexture("Mission1_PCBG.png");

	mission1BG_ = CreateActor<Mission1BG>(ObjectOrder::Background, "Mission1BG");
	mission1BG_->GetTransform().SetWorldPosition(
		-GameEngineWindow::GetScale().HX(),
		-GameEngineWindow::GetScale().HY(),
		40.f
	);
	//윈도우 왼쪽 아래를 배경액터 위치로 설정.

	currentFocusPointer_ = Indicator::CreateIndicator<Indicator>(
		"CurrentFocusPointer",
		this->GetMainCameraActor(),
		float4::Black,
		float4(0, 0, 5),	//절대 변경하지 말 것.
		float4(15, 15, 1)
	);

	destFocus_ = CreateActor<IndicatorBase>(ObjectOrder::UI, "DestFocus");
	destFocus_->SetPointerColor(float4::Yellow);
	destFocus_->GetTransform().SetWorldPosition(
		float4(00, 0, GetMainCameraActorTransform().GetWorldPosition().IZ())
	);
	//시작시 카메라 위치 변경이 필요하면 여기에서.

	soldier_Mission1_ = CreateActor<Soldier>(ObjectOrder::Soldier, "Soldier_Mission1");
	soldier_Mission1_->GetTransform().SetWorldPosition(500 - GameEngineWindow::GetInst()->GetScale().HIX(), 200, 0);


	arabian1_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian1");
	arabian2_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian2");
	arabian3_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian3");
	arabian4_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian4");
	arabian5_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian5");	

	
	arabian1_->GetTransform().SetWorldPosition(925, 0, 0);
	arabian1_->SetRecognitionDistance(750);

	arabian2_->GetTransform().SetWorldPosition(1050, 0, 0);
	arabian2_->SetRecognitionDistance(550);

	arabian3_->GetTransform().SetWorldPosition(1600, -200, 0);
	arabian3_->SetRecognitionDistance(400);

	arabian4_->GetTransform().SetWorldPosition(1700, -200, 0);
	arabian4_->SetRecognitionDistance(400);

	arabian5_->GetTransform().SetWorldPosition(1725, -200, 0);
	arabian5_->SetRecognitionDistance(400);



	camelRider_ = CreateActor<CamelRider>(ObjectOrder::Rebel, "CamelRider");
	camelRider_->GetTransform().PixLocalNegativeX();

	camelRider_->GetTransform().SetWorldPosition(mission1BG_->GetPart1RightEnd() - 575.f, 0.f, 0.f);


	troopTruck_ = CreateActor<Truck>(ObjectOrder::RebelMachine, "TroopTruck");
	troopTruck_->GetTransform().SetWorldPosition(6250, 0, 0);


	berserker1_ = CreateActor <Berserker>(ObjectOrder::Rebel, "Berserker1");
	berserker1_->GetTransform().SetWorldPosition(5100, 0, 0);


	ui_ = CreateActor<UI>(ObjectOrder::UI, "UI");
	ui_->GetTransform().SetWorldPosition(0, 0, -5);


	mission1BgmPlayer_ = GameEngineSound::SoundPlayControl("JUDGMENT (Mission 1).mp3", -1);

	GameEngineSound::SoundPlayOneshot("Mission1_Start.mp3");


#ifndef _DEBUG

	this->GetMainCameraActor()->GetTransform().SetWorldPosition(0, 0, -100);

	destFocus_->GetTransform().SetWorldPosition(
		float4(0, 0, -100)
	);

	soldier_Mission1_->GetTransform().SetWorldPosition(
		500 - GameEngineWindow::GetInst()->GetScale().HIX(),
		200,
		0
	);

	arabian6_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian6");
	arabian7_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian7");
	arabian8_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian8");
	arabian9_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian9");
	arabian10_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian10");

	arabian6_->GetTransform().SetWorldPosition(1850, 0, 0);
	arabian7_->GetTransform().SetWorldPosition(1925, 0, 0);
	arabian8_->GetTransform().SetWorldPosition(2200, 0, 0);
	arabian9_->GetTransform().SetWorldPosition(2525, 0, 0);
	arabian10_->GetTransform().SetWorldPosition(2600, 0, 0);

	arabian6_->SetRecognitionDistance(610);
	arabian7_->SetRecognitionDistance(610);
	arabian8_->SetRecognitionDistance(610);
	arabian9_->SetRecognitionDistance(610);
	arabian10_->SetRecognitionDistance(610);

	arabian11_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian11");
	arabian12_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian12");
	arabian13_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian13");
	arabian14_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian14");
	arabian15_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian15");
	arabian16_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian16");
	arabian17_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian17");
	arabian18_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian18");
	arabian19_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian19");
	arabian20_ = CreateActor<Arabian>(ObjectOrder::Rebel, "Arabian20");

	arabian11_->GetTransform().SetWorldPosition(3450, 0, 0);
	arabian12_->GetTransform().SetWorldPosition(3500, 0, 0);
	arabian13_->GetTransform().SetWorldPosition(3550, 0, 0);
	arabian14_->GetTransform().SetWorldPosition(3600, 0, 0);
	arabian15_->GetTransform().SetWorldPosition(3650, 0, 0);
	arabian16_->GetTransform().SetWorldPosition(3700, 0, 0);
	arabian17_->GetTransform().SetWorldPosition(3750, 0, 0);
	arabian18_->GetTransform().SetWorldPosition(3800, 0, 0);
	arabian19_->GetTransform().SetWorldPosition(3850, 0, 0);
	arabian20_->GetTransform().SetWorldPosition(3900, 0, 0);

	arabian11_->SetRecognitionDistance(605);
	arabian12_->SetRecognitionDistance(605);
	arabian13_->SetRecognitionDistance(605);
	arabian14_->SetRecognitionDistance(605);
	arabian15_->SetRecognitionDistance(605);
	arabian16_->SetRecognitionDistance(605);
	arabian17_->SetRecognitionDistance(605);
	arabian18_->SetRecognitionDistance(605);
	arabian19_->SetRecognitionDistance(605);
	arabian20_->SetRecognitionDistance(605);

#endif // !_DEBUG

	GameEngineGUI::CreateGUIWindow<DevelopmentTool>("Development Tool", this);

}

void Mission1::Update(float _deltaTime)
{
	if (true == GameEngineInput::GetInst()->IsDown("FreeCameraOnOff"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
		currentFocusPointer_->Off();
		destFocus_->Off();
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

void Mission1::Reset()
{
	destFocus_->GetTransform().SetWorldPosition(
		float4(0, 0, GetMainCameraActorTransform().GetWorldPosition().IZ())
	);
	isDestFocusHolding_ = false;

	soldier_Mission1_->GetTransform().SetWorldPosition(
		500 - GameEngineWindow::GetInst()->GetScale().HIX(),
		200,
		0
	);

	arabian1_->GetTransform().SetWorldPosition(925, 0, 0);
	arabian1_->ResetRebelState();

	arabian2_->GetTransform().SetWorldPosition(1050, 0, 0);
	arabian2_->ResetRebelState();

	arabian3_->GetTransform().SetWorldPosition(1600, -200, 0);
	arabian3_->ResetRebelState();

	arabian4_->GetTransform().SetWorldPosition(1700, -200, 0);
	arabian4_->ResetRebelState();

	arabian5_->GetTransform().SetWorldPosition(1725, -200, 0);
	arabian5_->ResetRebelState();

	camelRider_->GetTransform().PixLocalNegativeX();
	camelRider_->GetTransform().SetWorldPosition(mission1BG_->GetPart1RightEnd() - 575.f, 0.f, 0.f);
	camelRider_->ResetRebelState();
	isCamelRiderDead_ = false;

	troopTruck_->GetTransform().SetWorldPosition(6250, 0, 0);
	troopTruck_->ResetRebelState();
	isTruckDestroyed_ = false;

	//berserker1_->GetTransform().SetWorldPosition(5100, 0, 0);
	//berserker1_->ResetRebelState();


#ifndef _DEBUG

	arabian6_->GetTransform().SetWorldPosition(1850, 0, 0);
	arabian7_->GetTransform().SetWorldPosition(1925, 0, 0);
	arabian8_->GetTransform().SetWorldPosition(2200, 0, 0);
	arabian9_->GetTransform().SetWorldPosition(2525, 0, 0);
	arabian10_->GetTransform().SetWorldPosition(2600, 0, 0);

	arabian6_->ResetRebelState();
	arabian7_->ResetRebelState();
	arabian8_->ResetRebelState();
	arabian9_->ResetRebelState();
	arabian10_->ResetRebelState();

	arabian11_->GetTransform().SetWorldPosition(3450, 0, 0);
	arabian12_->GetTransform().SetWorldPosition(3500, 0, 0);
	arabian13_->GetTransform().SetWorldPosition(3550, 0, 0);
	arabian14_->GetTransform().SetWorldPosition(3600, 0, 0);
	arabian15_->GetTransform().SetWorldPosition(3650, 0, 0);
	arabian16_->GetTransform().SetWorldPosition(3700, 0, 0);
	arabian17_->GetTransform().SetWorldPosition(3750, 0, 0);
	arabian18_->GetTransform().SetWorldPosition(3800, 0, 0);
	arabian19_->GetTransform().SetWorldPosition(3850, 0, 0);
	arabian20_->GetTransform().SetWorldPosition(3900, 0, 0);

	arabian11_->ResetRebelState();
	arabian12_->ResetRebelState();
	arabian13_->ResetRebelState();
	arabian14_->ResetRebelState();
	arabian15_->ResetRebelState();
	arabian16_->ResetRebelState();
	arabian17_->ResetRebelState();
	arabian18_->ResetRebelState();
	arabian19_->ResetRebelState();
	arabian20_->ResetRebelState();

#endif // !_DEBUG

	mission1BgmPlayer_.Stop();
	mission1BgmPlayer_ = GameEngineSound::SoundPlayControl("JUDGMENT (Mission 1).mp3", -1);
	
	GameEngineSound::SoundPlayOneshot("Mission1_Start.mp3");

}

void Mission1::UpdateDestFocusMovement(float _deltaTime)
{
	if (mission1BG_->GetPart1RightEnd() - GameEngineWindow::GetScale().x - 5.f
		<= destFocus_->GetTransform().GetWorldPosition().x 
		&& false == isCamelRiderDead_)
	{
		if (false == camelRider_->IsUpdate())
		{
			isDestFocusHolding_ = false;
			isCamelRiderDead_ = true;
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
		<= destFocus_->GetTransform().GetWorldPosition().x
		&& false == isTruckDestroyed_)
	{
		if (true == troopTruck_->IsDestroyed())
		{
			soldier_Mission1_->SetSoldierWorldPosLimit(-1.f);
			isDestFocusHolding_ = false;
			isTruckDestroyed_ = true;
		}
		else
		{
			if (false == isDestFocusHolding_)
			{
				troopTruck_->SetTruckState(TruckState::Driving);
			}
			soldier_Mission1_->SetSoldierWorldPosLimit(troopTruck_->GetTransform().GetWorldPosition().x - 325.f);
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
