// ���̑��@�\
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_SUB_HPP_INCLUDED
#define ATS_SUB_HPP_INCLUDED

class CSub
{
public:
	int EmergencyNotch; // ���m�b�`
	int *BrakeNotch; // �u���[�L�m�b�`
	int *PowerNotch; // �͍s�m�b�`
	int *Reverser; // ���o�[�T�[
	float *TrainSpeed; // ���x�v�̑��x[km/h]

	int Key; // �}�X�R���L�[
	int SpeedMeter; // ���x�v([0-5km/h]�Ŕ�������)

	int KeyOn; // �}�X�R���L�[����
	int KeyOff; // �}�X�R���L�[����

	int ini_keyEvent; // �}�X�R���L�[�L��

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
		// ���t���[��
		float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]

		KeyOn = m_keyOn; // �}�X�R���L�[����
		m_keyOn = ATS_SOUND_CONTINUE;
		KeyOff = m_keyOff; // �}�X�R���L�[����
		m_keyOff = ATS_SOUND_CONTINUE;

		// ���x�v(��������)
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
				// �}�X�R���L�[����
				if(state > 0)
				{
					m_keyOn = ATS_SOUND_PLAY;
				}
			}
			else
			{
				// �}�X�R���L�[����
				if(state == 0)
				{
					m_keyOff = ATS_SOUND_PLAY;
				}
			}

			Key = (state > 0);
		}
	}

private:
	int m_keyOn; // �}�X�R���L�[����
	int m_keyOff; // �}�X�R���L�[����
};

#endif // ATS_SUB_HPP_INCLUDED
