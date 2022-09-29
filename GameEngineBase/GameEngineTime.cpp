#include "PreCompile.h"
#include "GameEngineTime.h"

GameEngineTime* GameEngineTime::inst_ = new GameEngineTime();

GameEngineTime::GameEngineTime() 
	: deltaTime_Double_(0.0),
	deltaTime_Float_(0.00f),
	globalTimeScale_(1.f),
	fps_(0),
	remainedFPSUpdateInterval_(0.0),
	loopCount_(0),
	totalFPS_(0),
	frameLimit_(-1),
	isUnderFrameLimit_(true)
{
}

GameEngineTime::~GameEngineTime()
{
}

void GameEngineTime::Reset()
{
	prev_ = std::chrono::steady_clock::now();

	fps_ = 0;
	remainedFPSUpdateInterval_ = 1.0;
	loopCount_ = 0;
	totalFPS_ = 0;

	isUnderFrameLimit_ = true;

	Update();
}

void GameEngineTime::Update()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	//이번 루프의 시작시간을 time_point형으로 저장한다.


	std::chrono::duration<double> time_span =
		std::chrono::duration_cast<std::chrono::duration<double>>(current - prev_);
	//이번 루프의 시작시간 - 지난 루프의 시작시간 = 지난 루프를 수행하는데 걸린 시간 == 델타타임.
	//델타타임을 duration<double>형태로 저장한다.

	//std::chrono::duration<Rep, Period = std::ratio<1>>: 특정 길이의 시간 간격이 몇 틱인지, 
	// Rep에 넣어준 형식으로 저장하는 클래스.
	//틱간 간격은 Period / 1초 간격이고, 템플릿 매개변수 Period에 아무 값도 넣어주지 않으면 기본값인 std::ratio<1>이 입력되어 초당 1틱으로 설정된다. 

	//std::ratio<Num, Denom = 1>: Num / Denom의 형태로 입력받은 분수나 비율을 표현, 계산하는 클래스. 

	deltaTime_Double_ = time_span.count();
	deltaTime_Float_ = static_cast<float>(deltaTime_Double_);
	//std::chrono::duration::count(): duration<double>형태로 저장된 델타타임이 몇 틱인지 계산한다. 
	// duration클래스 생성자의 두번째 템플릿 매개변수 Period에 아무 값도 넣어주지 않았으므로 초당 1틱으로 계산된다.
	//std::chrono::duration<Rep, Period>::count(): duration형식의 변수에 저장된 시간이 몇 틱인지 반환하는 함수. 

	prev_ = current;


	remainedFPSUpdateInterval_ -= deltaTime_Double_;
	if (DBL_EPSILON < deltaTime_Double_ && true == isUnderFrameLimit_)	
		//델타타임이 극도로 작은 값이거나 초당 프레임 제한을 넘겼다면 무시.
	{
		++loopCount_;
		totalFPS_ += static_cast<int>(1.0 / deltaTime_Double_);
	}

	if (0 >= remainedFPSUpdateInterval_ && 0 < loopCount_)	//1초간 평균 프레임 수 계산.
	{
		fps_ = totalFPS_ / loopCount_;

		remainedFPSUpdateInterval_ = 1.0;
		loopCount_ = 0;
		totalFPS_ = 0;

		isUnderFrameLimit_ = true;
	}

	if (-1 == frameLimit_)	//프레임 제한을 할 생각이 없다는 의미.
	{
		isUnderFrameLimit_ = true;
	}
	else if (loopCount_ >= frameLimit_)	//초당 프레임 수가 프레임 제한을 넘어서면 프레임 제한을 건다.
	{
		isUnderFrameLimit_ = false;
	}
}
