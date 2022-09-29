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
	//�̹� ������ ���۽ð��� time_point������ �����Ѵ�.


	std::chrono::duration<double> time_span =
		std::chrono::duration_cast<std::chrono::duration<double>>(current - prev_);
	//�̹� ������ ���۽ð� - ���� ������ ���۽ð� = ���� ������ �����ϴµ� �ɸ� �ð� == ��ŸŸ��.
	//��ŸŸ���� duration<double>���·� �����Ѵ�.

	//std::chrono::duration<Rep, Period = std::ratio<1>>: Ư�� ������ �ð� ������ �� ƽ����, 
	// Rep�� �־��� �������� �����ϴ� Ŭ����.
	//ƽ�� ������ Period / 1�� �����̰�, ���ø� �Ű����� Period�� �ƹ� ���� �־����� ������ �⺻���� std::ratio<1>�� �ԷµǾ� �ʴ� 1ƽ���� �����ȴ�. 

	//std::ratio<Num, Denom = 1>: Num / Denom�� ���·� �Է¹��� �м��� ������ ǥ��, ����ϴ� Ŭ����. 

	deltaTime_Double_ = time_span.count();
	deltaTime_Float_ = static_cast<float>(deltaTime_Double_);
	//std::chrono::duration::count(): duration<double>���·� ����� ��ŸŸ���� �� ƽ���� ����Ѵ�. 
	// durationŬ���� �������� �ι�° ���ø� �Ű����� Period�� �ƹ� ���� �־����� �ʾ����Ƿ� �ʴ� 1ƽ���� ���ȴ�.
	//std::chrono::duration<Rep, Period>::count(): duration������ ������ ����� �ð��� �� ƽ���� ��ȯ�ϴ� �Լ�. 

	prev_ = current;


	remainedFPSUpdateInterval_ -= deltaTime_Double_;
	if (DBL_EPSILON < deltaTime_Double_ && true == isUnderFrameLimit_)	
		//��ŸŸ���� �ص��� ���� ���̰ų� �ʴ� ������ ������ �Ѱ�ٸ� ����.
	{
		++loopCount_;
		totalFPS_ += static_cast<int>(1.0 / deltaTime_Double_);
	}

	if (0 >= remainedFPSUpdateInterval_ && 0 < loopCount_)	//1�ʰ� ��� ������ �� ���.
	{
		fps_ = totalFPS_ / loopCount_;

		remainedFPSUpdateInterval_ = 1.0;
		loopCount_ = 0;
		totalFPS_ = 0;

		isUnderFrameLimit_ = true;
	}

	if (-1 == frameLimit_)	//������ ������ �� ������ ���ٴ� �ǹ�.
	{
		isUnderFrameLimit_ = true;
	}
	else if (loopCount_ >= frameLimit_)	//�ʴ� ������ ���� ������ ������ �Ѿ�� ������ ������ �Ǵ�.
	{
		isUnderFrameLimit_ = false;
	}
}
