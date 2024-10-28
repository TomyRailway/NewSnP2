// ���͓S�� ATS
// Copyright (C) TomyRailway 2024

#ifndef ATS_MSAF_HPP_INCLUDED
#define ATS_MSAF_HPP_INCLUDED

#include "stdafx.h"
#include "atsplugin.h"

#define ATSMSAF_DECELERATION 19.5F // �����萔 (�����x[km/h/s] x 7.2)

#define SIGNAL_R 0 // 15km/h Limit
#define SIGNAL_YY 1 // 45km/h Limit
#define SIGNAL_Y 2 // 65km/h Limit
#define SIGNAL_YG 3 // 65km/h Limit
#define SIGNAL_G 4 // Freerun

#define SPEED_R 15+1 // R signal
#define SPEED_YY 25+1 // YY signal
#define SPEED_Y 45+1 // Y signal
#define SPEED_YG 65+1 // YG signal

#define LIMIT_F 0 // No Limit
#define LIMIT_65 1 // 65km/h Limit
#define LIMIT_45 2 // 45km/h Limit

#define ATS_FREE 0 // ATS Free
#define ATS_SVC 1 // ATS Service Brake
#define ATS_EMG 2 // ATS Emergency Brake

#define MAX_SPEED 120+1 // Max Speed

class CMsaf
{
private:
	int m_signal; // �M������
	int m_stepMs; // ������MS�_�̏��
	int m_preSignal; // �������̐M������
	int m_limit; // �Ȑ��ƍ��̏��
	int m_loop; // R�����p���[�v�̏��
	int m_halt; // ��ԃp�^�[��
	double m_dist; // ��~�ʒu�܂ł̋���
	int m_sps; // ��_�Ԓʉߎ��ԑ���

	int m_result_sig; // �M���ƍ��̌���
	int m_result_lim; // �Ȑ��ƍ��̌���

	int m_haltChime; // ��ԃ`���C��
	int m_passChime; // �ʉ߃`���C��

public:
	int *Time; // ���ݎ���[s]
	float *TrainSpeed; // ���x�v�̑��x[km/h]
	int *DeltaT; // �t���[������[flame/ms]
	int *BrakeNotch; // �u���[�L�m�b�`
	int EmgNotch; // ���m�b�`
	int SvcNotch; // ��p�ő�m�b�`

	int AtsBrake; // ATS�u���[�L

	int Ats; // ATS�d��
	int Normal; // ATS����
	int Action; // ATS����

	int AtsBell; // ATS�x��
	int HaltChime; // ��ԃ`���C��
	int PassChime; // �ʉ߃`���C��

	// Initalize�Ŏ��s���܂�
	void initialize(void)
	{
		AtsBrake = 0; // ATS�u���[�L

		Ats = 0; // ATS�d��
		Action = 0; // ATS����

		m_stepMs = 0; // ������MS�_�̏��
		m_limit = 0; // �Ȑ��ƍ��̏��
		m_loop = 0; // R�����p���[�v�̏��
		m_halt = 0; // ��ԃp�^�[��
		m_dist = 0; // ��~�ʒu�܂ł̋���
		m_sps = 0; // ��_�Ԓʉߎ��ԑ���

		m_result_sig = 0; // �M���ƍ��̌���
		m_result_lim = 0; // �Ȑ��ƍ��̌���

		m_haltChime = ATS_SOUND_STOP; // ��ԃ`���C��
		m_passChime = ATS_SOUND_STOP; // �ʉ߃`���C��
	}

	// Elapse�Ŏ��s���܂�
	void execute(void)
	{
		float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
		float def = speed / 3600 * *DeltaT; // 1�t���[���œ���������(��Βl)[m]

		// �M���ɂ��ƍ�
		switch(m_signal)
		{
		case SIGNAL_G: // �t���[����
			if(!m_stepMs || (m_stepMs && m_preSignal > SIGNAL_Y)) // ������Y���D�ʂ̂Ƃ�
			{
				break;
			}
		case SIGNAL_YG: // 65
			if(speed > SPEED_YG){m_result_sig = ATS_SVC;}
			else{m_result_sig = ATS_FREE;}
			break;
		case SIGNAL_Y: // 45
			if(!m_stepMs || (m_stepMs && m_preSignal > SIGNAL_R)) // ������R���D�ʂ̂Ƃ�
			{
				if(speed > SPEED_Y){m_result_sig = ATS_SVC;}
				else{m_result_sig = ATS_FREE;}
				break;
			}
		case SIGNAL_YY: // 25
			if(!m_stepMs) // MS�_�łȂ�
			{
				if(speed > SPEED_YY){m_result_sig = ATS_SVC;}
				else{m_result_sig = ATS_FREE;}
				break;
			}
		case SIGNAL_R: // 15
		default:
			if(m_loop) // R�����p���[�v�ł���
			{
				if(speed > 0){m_result_sig = ATS_EMG;}
			}
			else
			{	
				if(speed > SPEED_R){m_result_sig = ATS_EMG;}
			}
			break;
		}

		// �Ȑ��ɂ��ƍ�
		switch(m_limit)
		{
		case LIMIT_65:
			if(speed > 65+1){m_result_lim = ATS_SVC;}
			else{m_result_lim = ATS_FREE;}
			break;
		case LIMIT_45:
			if(speed > 45+1){m_result_lim = ATS_SVC;}
			else{m_result_lim = ATS_FREE;}
			break;
		case LIMIT_F:
		default:
			m_result_lim = ATS_FREE;
			break;
		}

		// ��ԃp�^�[���ɂ��ƍ�
		if(m_halt)
		{
			m_dist -= def; // ���t���[�����Z
			float limit = pow(speed, 2.0f) / ATSMSAF_DECELERATION; // ��ԃp�^�[��

			if(limit >= m_dist && m_dist > 0) // P��������
			{
				m_result_lim = ATS_EMG;
			}
		}

		// ATS�u���[�L�Ɍ��ʂ��X�V
		AtsBrake = max(m_result_sig, m_result_lim);

		// �\�������X�V
		Ats = 1; // ATS�d��
		Normal = (AtsBrake == 0); // ATS����
		Action = (AtsBrake > 0); // ATS����

		// ATS�x��
		AtsBell = AtsBrake ? ATS_SOUND_PLAYLOOPING : ATS_SOUND_STOP;

		HaltChime = m_haltChime; // ��ԃ`���C��
		m_haltChime = ATS_SOUND_CONTINUE;
		PassChime = m_passChime; // �ʉ߃`���C��
		m_passChime = ATS_SOUND_CONTINUE;
	}

	// ATS���A�X�C�b�`�Ŏ��s���܂�
	void reset(void)
	{
		if(*BrakeNotch >= SvcNotch && *TrainSpeed == 0)
		{
			AtsBrake = 0;

			// �ƍ��̌��ʂ����Z�b�g����
			m_result_sig = 0;
			m_result_lim = 0;
		}
	}

	// DoorOpen�Ŏ��s���܂�
	void DoorOpening()
	{
		m_halt = 0;
		m_haltChime = ATS_SOUND_STOP;
	}

	// SetSignal�Ŏ��s���܂�
	void ChangedSignal(int signal)
	{
		m_signal = signal; // �M������
		m_preSignal = m_signal;
		m_stepMs = 0;
		m_loop = 0;
	}

	// ���SMS�_��ʉ߂������Ɏ��s���܂�
	void PassedMs(int signal)
	{
		m_preSignal = signal;
		m_stepMs = 1;
	}

	// ���S�Ȑ����x�ƍ���ʉ߂������Ɏ��s���܂�
	void PassedCl(int data)
	{
		m_limit = data;
	}

	// ���SR�������[�v��ʉ߂������Ɏ��s���܂�
	void PassedLoop(int data)
	{
		m_loop = data > 0;
	}

	// ���S��_�Ԓʉߎ��ԑ��Ƃ�ʉ߂������Ɏ��s���܂�
	void PassedSps(int data)
	{
		if(*TrainSpeed > 0)
		{
			if(data > 0)
			{
				if(m_sps > 0 && m_sps > *Time)
				{
					AtsBrake = ATS_EMG;
				}
				m_sps = 0;
			}
			else
			{
				m_sps = *Time + 500;
			}
		}
	}

	// ���S��ʉߖh�~��ʉ߂������Ɏ��s���܂�
	void PassedSpp(int is_halt, int dist)
	{
		if(is_halt == 1) // ��ԉw
		{
			m_halt = 1;
			m_dist = dist > 0 ? (dist) : -(dist);
			m_haltChime = ATS_SOUND_PLAY;
		}
		else if(is_halt == 0) // �ʉ߉w
		{
			m_passChime = ATS_SOUND_PLAY;
		}
	}

};	// CMsaf

#endif	// ATS_MSAF_HPP_INCLUDED
