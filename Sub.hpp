// その他機能
// Copyright (C) 2009 Unicorn

#ifndef ATS_SUB_HPP_INCLUDED
#define ATS_SUB_HPP_INCLUDED

class CSub
{
public:
	int EmergencyNotch; // 非常ノッチ
	int *BrakeNotch; // ブレーキノッチ
	int *PowerNotch; // 力行ノッチ
	int *Reverser; // レバーサー
	float *TrainSpeed; // 速度計の速度[km/h]

	int Key; // マスコンキー
	int SpeedMeter; // 速度計([0-5km/h]で反応悪い)

	int KeyOn; // マスコンキー投入
	int KeyOff; // マスコンキー抜取

	int ini_keyEvent; // マスコンキー有効

	CSub(void)
	{
	}

	virtual ~CSub(void)
	{
	}

	void initialize(void)
	{
		SpeedMeter = 0;
		Key = (ini_keyEvent == 0) ? 1 : 0;

		m_keyOn = ATS_SOUND_STOP;
		m_keyOff = ATS_SOUND_STOP;
	}

	void execute(void)
	{
		// 毎フレーム
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]

		KeyOn = m_keyOn; // マスコンキー投入
		m_keyOn = ATS_SOUND_CONTINUE;
		KeyOff = m_keyOff; // マスコンキー抜取
		m_keyOff = ATS_SOUND_CONTINUE;

		// 速度計(反応悪い)
		if(speed < 5.0f)
		{
			SpeedMeter = 0.0f;
		}
		else
		{
			SpeedMeter = speed * 10.0f;
		}
	}

	void KeyEvent(int state)
	{
		if(ini_keyEvent > 0 && *BrakeNotch == EmergencyNotch && *PowerNotch == 0 && *Reverser == 0)
		{
			if(Key == 0)
			{
				// マスコンキー投入
				if(state > 0)
				{
					m_keyOn = ATS_SOUND_PLAY;
				}
			}
			else
			{
				// マスコンキー抜取
				if(state == 0)
				{
					m_keyOff = ATS_SOUND_PLAY;
				}
			}

			Key = (state > 0);
		}
	}

private:
	int m_keyOn; // マスコンキー投入
	int m_keyOff; // マスコンキー抜取
};

#endif // ATS_SUB_HPP_INCLUDED
