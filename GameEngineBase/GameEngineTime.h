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

	static inline double GetDeltaTimeD()
	{
		return inst_->deltaTime_Double_;
	}

	static inline float GetDeltaTimeF()
	{
		if (0.05f <= inst_->deltaTime_Float_)
		{
			inst_->deltaTime_Float_ = 0.05f;
		}

		return inst_->deltaTime_Float_ * inst_->globalTimeScale_;
	}

	static inline float GetDeltaTimeF(int _index)
	{
		return inst_->deltaTime_Float_ * inst_->GetTimeScale(_index);
	}
	template <typename EnumType>
	inline void GetDeltaTimeF(EnumType _type)
	{
		return GetDeltaTimeF(static_cast<int>(_type));
	}

	inline void SetTimeScale(int _index, float _timeScale)
	{
		timeScale_[_index] = _timeScale;
	}
	template <typename EnumType>
	void SetTimeScale(EnumType _type, float _timeScale)
	{
		SetTimeScale(static_cast<int>(_type), _timeScale);
	}

	inline float GetTimeScale(int _index)
	{
		if (timeScale_.end() == timeScale_.find(_index))
		{
			timeScale_[_index] = 1.00f;
		}

		return timeScale_[_index];
	}

	inline void SetGlobalTimeScale(float _globalTimeScale)
	{
		globalTimeScale_ = _globalTimeScale;
	}

	static inline int GetFPS()
	{
		return inst_->averageFPS_;
	}

	static inline void SetFrameLimit(int _frameLimit)
	{
		inst_->frameLimit_ = _frameLimit;
	}

	static inline bool IsUnderFrameLimit()
	{
		return inst_->isUnderFrameLimit_;
	}

private:

	std::chrono::steady_clock::time_point prev_;	//���� ���� ���� �ð�.

	//std::chrono::time_point: epoch��� ������ ���س���, Ư�� �������κ��� 
	// ����ڰ� ������ �������� �ð� ������ �󸶳� �Ǵ����� duration�̶�� Ŭ������ ǥ���ϰ� ����ϴ� Ŭ����.

	//std::chrono::system_clock�� ��쿡��, ���н� Ÿ���̶�� �θ��� �׷����� 1970�� 1�� 1�� 00�� 00�� 00�ʰ� 
	// epoch�̸�, �ý��� �ð� ������ ���� �ð��� �����ؼ� duration�� ������ �� ���� �ִ�.
	//std::chrono::steady_clock�� ��쿡��, �ý��� ���� ������ epoch�� �Ǹ�, 
	// �ܹ���(monotonic)���θ� �ð��� �귯���Ƿ� ���� �������� �ʰ�, duration�� ������ ���� ���� ����.


	double deltaTime_Double_;				//��ŸŸ��: ���� ������ �ѹ� ���� �� �� �ɸ� �ð�.
	float deltaTime_Float_;				//��ŸŸ��: ���� ������ �ѹ� ���� �� �� �ɸ� �ð�.
	//��ǻ���� ������ �������� ��ŸŸ�� ���� �۾����Ƿ�, ��ǻ�� ���� ������ ���� ���� �ٸ� �ӵ��� ������ �����Ѵٰ� �ص� 
	//���ɿ� �ݺ���ؼ� ��ŸŸ�� ���� �۰� ������, �׷� ���� ��ǻ�� ���� ������� ���� �ð� ���� ���� ����ӵ��� �����ְ� �ȴ�.

	std::map<int, float> timeScale_;

	float globalTimeScale_;			//���� ��ü �ӵ� ������ ���.


	int averageFPS_;					//�ʴ� ��� ������ ���� Ƚ��.
	double remainedFPSUpdateInterval_;	//���� ���� FPS ���� �ֱ�.
	int loopCount_;				//1�ʰ� ������ ��ü ���� ��.
	int totalFPS_;			//1�ʰ� ����� FPS�� ����.

	int frameLimit_;
	bool isUnderFrameLimit_;


};

