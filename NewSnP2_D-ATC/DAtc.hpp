// JR�����{ D-ATC
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_DATC_HPP_INCLUDED
#define ATS_DATC_HPP_INCLUDED

#include "stdafx.h"

#define DATC_DECELERATION 14.4F // �����萔 (�����x[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define DATC_OFFSET 0.5F // �ԏ�q�I�t�Z�b�g[m]

#define ATCMODE_DATC 1 // 0:10�^ATC 1:D-ATC

class CDatc
{
public:
	int EmergencyNotch; // ���m�b�`
	int *BrakeNotch; // �u���[�L�m�b�`
	int *Time; // ���ݎ���
	float *TrainSpeed; // ���x�v�̑��x[km/h]
	int *DeltaT; // �t���[������[ms/frame]

	int AtcBrake; // ATC�u���[�L

	int DAtc; // ATC�d��
	int IsDAtc; // D-ATC
	int EmgDriving; // ATC���^�]
	int AtcEmg; // ATC���
	int AtcSvc; // ATC��p
	int AtcApproach; // �p�^�[���ڋ�

	int Signal[2]; // ATC����
	int Arrow[32]; // ATC���x
	int SlideArrow; // ATC�X���C�h���x
	int IsOrp; // ORP
	int OrpNeedle; // ORP���x

	int SigFall; // �O���\��

	int AtcDing; // ATC�P�Ńx��
	int VoiceAts; // ATS�ؑ։���
	int VoiceAtc; // ATC�ؑ։���

	CDatc()
	{
	}

	virtual ~CDatc()
	{
	}

	// Initalize�Ŏ��s���܂�
	void initialize(void)
	{
		DAtc = 1; // D-ATC
		EmgDriving = 0; // ATC���^�]
		AtcApproach = 0; // �p�^�[���ڋ�
		m_falling = 0; // ATC�~���\��

		ResetIndicator(); // ATC�\�����Z�b�g

		m_deltaApproach = 0; // �O��p�^�[���ڋ�
		m_atcDing = ATS_SOUND_STOP; // ATC�P�Ńx��
		m_voiceAts = ATS_SOUND_STOP; // ATS�ؑ։���
		m_voiceAtc = ATS_SOUND_STOP; // ATC�ؑ։���
	}

	// Elapse�Ŏ��s���܂�
	void execute(void)
	{
		if(IsDAtc == 0) // �d�����؂ł���
		{
			AtcBrake = 0; // ATC�u���[�L
		}
		else // �d�������ł���
		{
			float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
			float def = speed / 3600 * *DeltaT; // 1�t���[���œ���������(��Βl)[m]

			if(EmgDriving == 1) // ATC���^�]�ł���
			{
				AtcBrake = speed > 26.0f ? 1 : 0; // 25km/h����

			}
			else // ATC���^�]�łȂ�
			{
				// ATC�u���[�L
				if(m_signal < 10 || m_signal > 42) // ���M��
				{
					AtcBrake = 2;
				}
				else if(m_signal == 10) // 01�M��
				{
					AtcBrake = 1;
				}
				else if(*TrainSpeed > itov(m_signal) + 2.0f) // �L���ȃR�[�h
				{
					AtcBrake = 1;
				}
				else
				{
					AtcBrake = 0;
				}

#if (ATCMODE_DATC)
				if(m_falling == 1) // ATC�M���~��
				{
					if(m_distance > 0){m_distance -= def;} // �~���܂ł̋���
					float limit = (pow(speed, 2.0f) - pow(itov(m_target), 2.0f)) / DATC_DECELERATION;
					if(limit >= m_distance){AtcBrake = 1;} // �p�^�[������
					
					m_deltaApproach = AtcApproach; // �O��p�^�[���ڋ�

					if(itov(m_target) <= speed && limit + (speed / 3.6 * 5) >= m_distance)
					{
						AtcApproach = 1; // �p�^�[���ڋ�
					}
					else
					{
						AtcApproach = 0; // �p�^�[���ڋ�
					}

					// �ڋߏ�ԕς����
					if(m_deltaApproach != AtcApproach)
					{
						m_atcDing = ATS_SOUND_PLAY; // ATC�P�Ńx��
					}
				}
#endif
			}
		}

		ResetIndicator(); // ATC�\�����Z�b�g
		if(IsDAtc == 1){RunIndicator();} // ATC�\����

		AtcEmg = AtcBrake == 2 ? 1 : 0; // ATC���
		AtcSvc = AtcBrake > 0 ? 1 : 0; // ATC��p

		AtcDing = m_atcDing; // ATC�P�Ńx��
		m_atcDing = ATS_SOUND_CONTINUE;
		VoiceAts = m_voiceAts; // ATS�ؑ։���
		m_voiceAts = ATS_SOUND_CONTINUE;
		VoiceAtc = m_voiceAtc; // ATC�ؑ։���
		m_voiceAtc = ATS_SOUND_CONTINUE;
	}

	// SetSignal�Ŏ��s���܂�
	void ChangedSignal(int signal)
	{
		// �M���C���f�b�N�X�݊�
		switch(signal)
		{
		case 100: Exit(); break; // �ؑ�
		case 101: signal = 9; break; // X
		case 102: signal = 10; break; // 0
		case 103: signal = 13; break; // 15
		case 104: signal = 15; break; // 25
		case 105: signal = 19; break; // 45
		case 106: signal = 21; break; // 55
		case 107: signal = 23; break; // 65
		case 108: signal = 25; break; // 75
		case 109: signal = 28; break; // 90
		case 110: signal = 30; break; // 100
		case 111: signal = 32; break; // 110
		case 112: signal = 34; break; // 120
		default: break;
		}

		if(!IsDAtc && signal >= 9 && signal < 37) // ATC�M���̂Ƃ�
		{
			Start(); // �d��������
		}
		else if(IsDAtc && signal < 9) // ATS�M���̂Ƃ�
		{
			Exit(); // �d����؂�
		}

#if (ATCMODE_DATC)
		if(IsDAtc && !EmgDriving && (itov(signal) && signal > m_signal || !m_falling && signal > m_signal)) // D-ATC
#else
		if(IsDAtc && !EmgDriving && (signal >= 9 && signal <= 42 && signal != m_signal)) // 10�^ATC
#endif
		{
			m_atcDing = ATS_SOUND_PLAY; // ATC�P�Ńx��
		}

		m_signal = signal; // �M���̍X�V
		m_falling = 0; // ATC�~���\��
	}

	// D-ATC���Ւf����Ƃ��Ɏ��s���܂�
	void Exit(void)
	{
		if(IsDAtc == 1)
		{
			IsDAtc = 0; // D-ATC
			m_atcDing = ATS_SOUND_PLAY; // ATC�P�Ńx��
			m_voiceAts = ATS_SOUND_PLAY; // ATS�ؑ։���

			EmgDriving = 0; // ATC���^�]
			m_falling = 0; // ATC�~���\��
		}
	}

	// D-ATC�𓊓�����Ƃ��Ɏ��s���܂�
	void Start(void)
	{
		if(IsDAtc == 0)
		{
			IsDAtc = 1; // D-ATC
			m_atcDing = ATS_SOUND_PLAY; // ATC�P�Ńx��
			m_voiceAtc = ATS_SOUND_PLAY; // ATC�ؑ։���
		}
	}

	// D-ATC�̔��^�]��ؑւ���Ƃ��Ɏ��s���܂�
	void EmgDrive(void)
	{
		if(IsDAtc == 1 && *BrakeNotch == EmergencyNotch)
		{
			EmgDriving = !EmgDriving ? 1 : 0; // ATC���^�]
			m_atcDing = ATS_SOUND_PLAY; // ATC�P�Ńx��
		}
	}

	// ATC�M���~����ʉ߂������Ɏ��s���܂�
	void PassedAtcFall(int signal, float dist)
	{
		// �M���C���f�b�N�X�݊�
		switch(signal)
		{
		case 100: Exit(); break; // �ؑ�
		case 101: signal = 9; break; // X
		case 102: signal = 10; break; // 0
		case 103: signal = 13; break; // 15
		case 104: signal = 15; break; // 25
		case 105: signal = 19; break; // 45
		case 106: signal = 21; break; // 55
		case 107: signal = 23; break; // 65
		case 108: signal = 25; break; // 75
		case 109: signal = 28; break; // 90
		case 110: signal = 30; break; // 100
		case 111: signal = 32; break; // 110
		case 112: signal = 34; break; // 120
		default: break;
		}

		if(IsDAtc == 1)
		{
			if(itov(signal) && signal < m_signal)
			{
				m_falling = 1; // ATC�~���\��
				m_target = signal; // ATC�~���ڕW
				m_distance = dist; // �~���܂ł̋���
			}
			else
			{
				m_falling = 0; // ATC�~���\��
			}
		}
	}

private:
	int m_signal; // �M������
	int m_falling; // ATC�~���\��
	int m_target; // ATC�~���ڕW
	float m_distance; // �~���܂ł̋���
	int m_deltaApproach; // �O��p�^�[���ڋ�

	int m_atcDing; // ATC�P�Ńx��
	int m_voiceAts; // ATS�ؑ։���
	int m_voiceAtc; // ATC�ؑ։���

	// ATC�\������������
	void ResetIndicator(void)
	{
		// ATC�\���̃��Z�b�g
		for(int i=0;i<1;i++){Signal[i] = 0;} // ATC����
		for(int j=0;j<32;j++){Arrow[j] = 0;} // ATC���x
		SlideArrow = -10; // ATC�X���C�h���x
		SigFall = 0; // �O���\��
		IsOrp = 0; // ORP
		OrpNeedle = 0; // ORP���x
	}

	// ATC�\�������X�V
	void RunIndicator(void)
	{
		if(EmgDriving == 1) // ATC���^�]�ł���
		{
			Signal[0] = 1; // �Ԍ���
			Arrow[6] = 1; // ATC-25
			SlideArrow = 25; // ATC�X���C�h���x
		}
		else // ATC���^�]�łȂ�
		{
			// ATC�M��
			if(m_signal >= 12 && m_signal <= 42){Signal[1] = 1;}
			else{Signal[0] = 1;}

			// ORP�\����
			IsOrp = (m_signal == 11);

#if (ATCMODE_DATC)
			if(m_falling == 1) // ATC�M���~��
			{
				// ATC���x
				double pattern = sqrt((m_distance * DATC_DECELERATION) + pow(itov(m_target), 2.0f));
				Arrow[vtoi(pattern)] = 1;

				// ATC�X���C�h���x
				if(pattern > itov(m_signal)){SlideArrow = itov(m_signal);}
				else if(pattern > itov(m_target)){SlideArrow = pattern;}
				else{SlideArrow = itov(m_target);}
			}
			else // �ʏ�̂Ƃ�
			{
				// ATC���x
				if(m_signal < 10 || m_signal > 36){Arrow[0] = 1;}
				else{Arrow[(m_signal-10)+1] = 1;}

				// ATC�X���C�h���x
				SlideArrow = itov(m_signal);
			}
#else
			// ATC���x
			if(m_signal < 10 || m_signal == 11 || m_signal > 36){Arrow[0] = 1;}
			else{Arrow[(m_signal-10)+1] = 1;}

			// ATC�X���C�h���x
			SlideArrow = itov(m_signal);
#endif
			// ATC�O���\��
			SigFall = m_falling; // �O���\��

			// ORP���x
			OrpNeedle = (m_signal == 11) ? 0 : 0;
		}
	}

	// �C���f�b�N�X��ATC���x�ɕϊ�����
	int itov(int index)
	{
		int speed = 0;

		// ATC-5 ���� ATC-130
		if(index >= 12 && index <= 42)
		{
			speed = (index - 10) * 5;
		}
		else // ����ȊO�͖��M��
		{
			speed = 0;
		}

		return speed;
	}

	// ���x��Arrow�C���f�b�N�X�ɕϊ�����
	int vtoi(int speed)
	{
		int index = 1;

		index += ((speed % 130) - (speed % 5)) / 5;

		return index;
	}

};	// CDatc

#endif	// ATS_DATC_HPP_INCLUDED
