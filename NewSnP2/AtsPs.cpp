// JR�����{ ATS-Ps
// Copyright (C) 2009 Unicorn

#ifndef ATS_ATSPS_HPP_INCLUDED
#define ATS_ATSPS_HPP_INCLUDED

#include "stdafx.h"
#include "atsplugin.h"
#include "AtsPs.h"

#define ATSPS_DECELERATION 19.5F // �����萔 (�����x[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
 #define ATSPS_OFFSET 0.5F // �ԏ�q�I�t�Z�b�g[m]

class CAtsps
{
public:
	int EmergencyNotch; // ���m�b�`
	int *BrakeNotch; // �u���[�L�m�b�`
	float *TrainSpeed; // ���x�v�̑��x[km/h]
	int *DeltaT; // �t���[������[ms/frame]

	int AtsBrake; // ATS�u���[�L

	int AtsPs; // Ps�d��
	int IsAtsPs; // ATS-Ps
	int PatternStart; // �p�^�[������
	int PatternApproach; // �p�^�[���ڋ�
	int BrakeTurnOff; // �u���[�L�J��
	int BrakeTurnOn; // �u���[�L����
	int TrainSpeedLED; // ��ԑ��xLED
	int PatternSpeedLED; // �p�^�[�����xLED
	int BrakeDown; // �̏�

	int PatternBegin; // Ps�p�^�[������
	int PatternEnd; // Ps�p�^�[���I��
	int Chime; // Ps�`���C��

	CAtsps(void)
	{
	}

	virtual ~CAtsps()
	{
	}

	// Initalize�Ŏ��s���܂�
	void initialize(void)
	{
		Init();
	}

	/*
    // ATS-SN�ɐؑ�
    void CAtsps::Exit()
    {
	    if(AtspLamp){blowBell(false);}
	    init();
    }

    

	*/

	// Elapse�Ŏ��s���܂�
	void execute(void)
	{
		if(IsAtsPs == 0) // �d�����؂ł���
		{
			AtsBrake = 0; // ATS�u���[�L
		}
		else // �d�������ł���
		{
		}

		PatternBegin = m_patternBegin; // Ps�p�^�[������
		m_patternBegin = ATS_SOUND_CONTINUE;
		PatternEnd = m_patternEnd; // Ps�p�^�[���I��
		m_patternEnd = ATS_SOUND_CONTINUE;
		Chime = m_chime; // Ps�`���C��
		m_chime = ATS_SOUND_CONTINUE;
	}

	// SetSignal�Ŏ��s���܂�
	void ChangedSignal(int signal)
	{
		// !Power�Ȃ�AStart(); // �d��������
	}

	// ATS-Ps���Ւf����Ƃ��Ɏ��s���܂�
	void Exit(void)
	{
		if(IsAtsPs == 1)
		{
			IsAtsPs = 0; // ATS-Ps
		}
	}

	// ATS-Ps�𓊓�����Ƃ��Ɏ��s���܂�
	void Start(void)
	{
		if(IsAtsPs == 0)
		{
			IsAtsPs = 1; // ATS-Ps
		}
	}

private:
	int m_patternBegin; // Ps�p�^�[������
	int m_patternEnd; // Ps�p�^�[���I��
	int m_chime; // Ps�`���C��

	void Init(void)
	{
		AtsPs = 1; // Ps�d��

		m_patternBegin = ATS_SOUND_STOP; // Ps�p�^�[������
		m_patternEnd = ATS_SOUND_STOP; // Ps�p�^�[���I��
		m_chime = ATS_SOUND_STOP; // Ps�`���C��
	}

};	// CAtsps

#endif	// ATS_ATSPS_HPP_INCLUDED
