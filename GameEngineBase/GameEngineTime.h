#pragma once

class GameEngineTime
{
	//�� Ŭ������ ���� ����:

	static GameEngineTime* inst_;
private:

	GameEngineTime();
	~GameEngineTime();

	GameEngineTime(const GameEngineTime& _other) = delete;
	GameEngineTime(GameEngineTime&& _other) noexcept = delete;
	GameEngineTime& operator=(const GameEngineTime& _other) = delete;
	GameEngineTime& operator=(GameEngineTime&& _other) = delete;


public:
	void Reset();			//�ð� ���� �غ� �Լ�.
	void Update();			//��ŸŸ�� ���� �Լ�.

public:
	static GameEngineTime* GetInst()
	{
		return inst_;
	}

	static void Destroy()
	{
		//�������� �� �ϰ� ������ �������� �̱����� ����� ����: ���� ������ �� ������� �����ϱ� ���ؼ�.
		//�̱��� ����'��' ���ϴ� Ÿ�ֿ̹� �Ѵ�.
		if (nullptr != inst_)
		{
			delete inst_;
			inst_ = nullptr;
		}
	}

	static float GetDeltaTime()
	{
		return inst_->deltaTime_;
	}

	static float GetDeltaTime(int _index)
	{
		return inst_->deltaTime_ * inst_->GetTimeScale(_index);
	}
	template <typename EnumType>
	void GetDeltaTime(EnumType _type)
	{
		return GetDeltaTime(static_cast<int>(_type));
	}

	void SetTimeScale(int _index, float _timeScale)
	{
		timeScale_[_index] = _timeScale;
	}
	template <typename EnumType>
	void SetTimeScale(EnumType _type, float _timeScale)
	{
		SetTimeScale(static_cast<int>(_type), _timeScale);
	}

	float GetTimeScale(int _index)
	{
		if (timeScale_.end() == timeScale_.find(_index))
		{
			timeScale_[_index] = 1.00f;
		}

		return timeScale_[_index];
	}

private:

	std::chrono::steady_clock::time_point prev_;	//���� ���� ���� �ð�.

	//std::chrono::time_point: epoch��� ������ ���س���, Ư�� �������κ��� 
	// ����ڰ� ������ �������� �ð� ������ �󸶳� �Ǵ����� duration�̶�� Ŭ������ ǥ���ϰ� ����ϴ� Ŭ����.

	//std::chrono::system_clock�� ��쿡��, ���н� Ÿ���̶�� �θ��� �׷����� 1970�� 1�� 1�� 00�� 00�� 00�ʰ� 
	// epoch�̸�, �ý��� �ð� ������ ���� �ð��� �����ؼ� duration�� ������ �� ���� �ִ�.
	//std::chrono::steady_clock�� ��쿡��, �ý��� ���� ������ epoch�� �Ǹ�, 
	// �ܹ���(monotonic)���θ� �ð��� �귯���Ƿ� ���� �������� �ʰ�, duration�� ������ ���� ���� ����.


	//double deltaTimeD_;				//��ŸŸ��: ���� ������ �ѹ� ���� �� �� �ɸ� �ð�.
	float deltaTime_;				//��ŸŸ��: ���� ������ �ѹ� ���� �� �� �ɸ� �ð�.
	//��ǻ���� ������ �������� ��ŸŸ�� ���� �۾����Ƿ�, ��ǻ�� ���� ������ ���� ���� �ٸ� �ӵ��� ������ �����Ѵٰ� �ص� 
	//���ɿ� �ݺ���ؼ� ��ŸŸ�� ���� �۰� ������, �׷� ���� ��ǻ�� ���� ������� ���� �ð� ���� ���� ����ӵ��� �����ְ� �ȴ�.

	std::map<int, float> timeScale_;

};

