// JR�e�� EB���u
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_EB_HPP_INCLUDED
#define ATS_EB_HPP_INCLUDED

#include "stdafx.h"
#include <cmath> // fabsf �̂��߂ɕK�v

class CEb
{
private:
	int m_tmrPhase1;   // 60�b�^�C�}�[
	int m_tmrPhase2;   // 5�b�^�C�}�[
	int m_ebSwitch;    // EB�X�C�b�`
	int m_iniEBEnabled; // ini�œǂݍ���EB�d���i0=OFF, 1=ON�j

public:
	int EmergencyNotch;   // ���m�b�`
	int* Time;            // ���ݎ����~���b�i�|�C���^�ŎQ�Ɓj
	float* TrainSpeed;    // ��ԑ��x [km/h]
	int Emergency;        // ��퐧�����
	int Action;           // EB�쓮���
	int RedLamp;          // �ԃ����v�_��
	int Bell;             // ATS�x��
	int Buzzer;           // EB�u�U�[
	int EbSwitch;         // EB�X�C�b�`��

	CEb() = default;
	virtual ~CEb() = default;

	// �������֐��iini����󂯎����EB��Ԃ������Ŏw��j
	void initialize(int ebEnabled)
	{
		m_iniEBEnabled = ebEnabled;
		Emergency = 0;
		Action = 0;
		RedLamp = 0;
		Bell = ATS_SOUND_STOP;
		Buzzer = ATS_SOUND_STOP;
		EbSwitch = ATS_SOUND_STOP;
		m_tmrPhase1 = *Time + 60000;
		m_tmrPhase2 = 0;
	}

	// ���C�������iElapse�ŌĂяo���j
	void execute()
	{
		if (m_iniEBEnabled == 0) return; // EB�d��OFF�Ȃ疳����

		RedLamp = 0;
		Buzzer = ATS_SOUND_STOP;
		Bell = ATS_SOUND_STOP;

		if (*TrainSpeed == 0)
		{
			ResetPhase1(); // �Î~���͏�����
		}

		if (std::fabs(*TrainSpeed) > 15 && *Time > m_tmrPhase1)
		{
			Action = 1;
			m_tmrPhase1 = *Time + 60000;
			if (m_tmrPhase2 == 0) m_tmrPhase2 = *Time + 5000;
		}

		if (Action == 1 && *Time > m_tmrPhase2)
		{
			Emergency = 1;
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
		}

		if (Action == 1)
		{
			Buzzer = ATS_SOUND_PLAYLOOPING;
		}

		if (Emergency == 1)
		{
			RedLamp = (*Time % 750) / 375;
			Bell = ATS_SOUND_PLAYLOOPING;
		}

		EbSwitch = m_ebSwitch;
		m_ebSwitch = ATS_SOUND_CONTINUE;
	}

	// ��~���ȂǂŃ^�C�}�[��������
	void ResetPhase1(int is_sw = 0)
	{
		if (Emergency != 1)
		{
			Action = 0;
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
		}
		if (is_sw == 1)
		{
			m_ebSwitch = ATS_SOUND_PLAY;
		}
	}

	// Brake�ݒ�ŌĂяo���F���u���[�L���A����
	void ResetPhase2(int notch)
	{
		if (notch == EmergencyNotch && *TrainSpeed == 0)
		{
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
			Action = 0;
			Emergency = 0;
		}
	}
};

#endif // ATS_EB_HPP_INCLUDED
