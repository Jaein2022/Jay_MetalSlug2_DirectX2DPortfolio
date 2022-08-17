#include "PreCompile.h"
#include "GameEngineTime.h"

GameEngineTime* GameEngineTime::inst_ = new GameEngineTime();

GameEngineTime::GameEngineTime() : deltaTime_(0.00f), globalScale_(1.f)
{
}

GameEngineTime::~GameEngineTime()
{
}

void GameEngineTime::Reset()
{
	prev_ = std::chrono::steady_clock::now();
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

	deltaTime_ = static_cast<float>(time_span.count());
	//std::chrono::duration::count(): duration<double>���·� ����� ��ŸŸ���� �� ƽ���� ����Ѵ�. 
	// durationŬ���� �������� �ι�° ���ø� �Ű����� Period�� �ƹ� ���� �־����� �ʾ����Ƿ� �ʴ� 1ƽ���� ���ȴ�.
	//std::chrono::duration<Rep, Period>::count(): duration������ ������ ����� �ð��� �� ƽ���� ��ȯ�ϴ� �Լ�. 


	prev_ = current;
}
