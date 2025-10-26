// JR�����{���� TIMS���u ����� �O���X�R�b�N�s�b�g���u
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_TIMS_HPP_INCLUDED
#define ATS_TIMS_HPP_INCLUDED

#define TIMS_DECELERATION 19.5F // �����萔 (�����x[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define TIMS_OFFSET 0.5F // �ԏ�q�I�t�Z�b�g[m]
#define MAX_SECTION_COUNT 10 // ������Ԃ̍ő吔�im_pathSecDist / m_passSecLine �̃T�C�Y�ɍ��킹��j

#define RANDOMIZE_UNITTIMS 30 // TIMS���j�b�g�\���̍X�V�m��
#define RANDOMIZE_UNITLAMP 10 // ���j�b�g�\�����̍X�V�m��
#define UPDATE_SPAN 180000 // �^�s���X�V�̍Œ�Ԋu
#define RELAY_REQUIREMENT 7.5f // ���s���m�����[���삷�鑬�x

class CTims
{
public:
	int EmergencyNotch; // ���m�b�`
	int *BrakeNotch; // �u���[�L�m�b�`
	int *PowerNotch; // �͍s�m�b�`
	int *Reverser; // ���o�[�T�[
	int *Time; // ���ݎ���
	float *TrainSpeed; // ���x�v�̑��x[km/h]
	int *DeltaT; // �t���[������[ms/frame]
	float BcPressure; // �u���[�L�V�����_����[kPa]
	float MrPressure; // ����C�_������[kPa]
	float Current; // �d��[A]
	double Location; // ��Ԉʒu[m]

	int Kind; // ��Ԏ��
	int Number[4]; // ��Ԕԍ�(1000 - 100 - 10 - 1)
	int Charactor; // ��Ԕԍ�(�L��)
	int From; // �w���\��(�n��)
	int Destination; // �w���\��(�s��)
	int This; // �w���\��(���w)
	int Next; // �w���\��(���w)
	int NextBlink; // �w���\���̓_��
	int NextTime[3]; // ��������(���w�A�� - �� - �b)
	int Crawl; // ���s���
	int CrawlLimit; // ���s���x
	int AfterNumber[4]; // ���^�p��Ԕԍ�(1000 - 100 - 10 - 1)
	int AfterChara; // ���^�p�L��
	int AfterTime[2][3]; // ���^�p��������
	int LastStopTime[3]; // �I�_��������(�� - �� - �b)
	int LastStopTrack; // �I�_�����Ԑ�
	int RelayName; // �s�H�I�_�w��
	int TrainArrow; // �s�H�\���
	int Station[10]; // �w���\��
	int PassSta[10]; // �ʉ߉w���\��
	int Arrive[10][3]; // ��������(�� - �� - �b)
	int Leave[10][3]; // �o������(�� - �� - �b)
	int Track[10]; // ���w�����Ԑ�
	int Limit[10]; // �w�ԍō����x
	int Span[10][2]; // �w�ԑ��s����
	int HiddenLine[10]; // �X�V����1�s����\���ɂ���
	int TrackPath[10]; // �J�ʏ��(100m����)
	int TrackPath2[120]; // �J�ʏ��(25m����)
	int Distance1000; // ���s����(km�̌�)
	int Distance100; // ���s����(100m�̌�)
	int Distance10; // ���s����(10m�̌�)
	int Location1000; // ��Ԉʒu(km�̌�)
	int Location100; // ��Ԉʒu(100m�̌�)
	int Location10; // ��Ԉʒu(10m�̌�)
	int TimsSpeed100; // TIMS���x(100�̌�)
	int TimsSpeed10; // TIMS���x(10�̌�)
	int TimsSpeed1; // TIMS���x(1�̌�)
	int UnitTims[10]; // TIMS���j�b�g�\��(0-9)
	int UnitState[10]; // ���j�b�g�\����(0-9)
	int BrakeDelay; // �u���[�L�w��
	int BcCaution; // 200kPa�x��
	int BcPressCut; // �u���[�L��������
	int RelayState; // ���s���m�����[
	int Direction; // �i�s����
	int ArrowDirection; // �i�s�������
	
	int Speed; // ���x�v
	int Speed100; // ���x(100�̌�)
	int Speed10; // ���x(10�̌�)
	int Speed1; // ���x(1�̌�)
	int BcPress0; // �u���[�L�V�����_�w�j(0-180kPa)
	int BcPress1; // �u���[�L�V�����_�w�j(200-380kPa)
	int BcPress2; // �u���[�L�V�����_�w�j(400-580kPa)
	int BcPress3; // �u���[�L�V�����_�w�j(600-780kPa)
	int MrPress0; // ����C�_���w�j(700-790kPa)
	int MrPress1; // ����C�_���w�j(800-890kPa)
	int MrPress2; // ����C�_���w�j(900-990kPa)
	int Ammeter; // �d���v

	int *c_DAtcSlideArrow; // DAtc�N���X��SlideArrow�̒l
	int SlideArrow; // ATC�X���C�h���x
	int AccelCutting; // �͍s�x��(���̎����܂Ŕ��f���Ȃ�)

	int RelayD; // ���s���m�����[
	int LbInit; // ����N�����f���퉹
	int AirHigh; // ���u���[�L�ɉ���
	int AirApply; // �u���[�L������
	int EmgAnnounce; // ���u���[�L����
	int AirApplyEmg; // �u���[�L������(���)
	int UpdateInfo; // �^�s���X�V

	// �𒼐ؑ�
	int AC; // ��
	int DC; // ����
	int CVacc; // ����d���ُ�
	int CVacc10; // ����d��(10��)
	int CVacc1; // ����d��(1��)
	int ACacc; // �𗬓d���ُ�
	int ACacc10000; // �𗬓d��(10000��)
	int ACacc1000; // �𗬓d��(1000��)
	int ACacc100; // �𗬓d��(100��)
	int DCacc; // �����d���ُ�
	int DCacc1000; // �����d��(1000��)
	int DCacc100; // �����d��(100��)
	int DCacc10; // �����d��(10��)
	int Cvmeter; // ����w�j
	int Acmeter; // �𗬎w�j
	int Dcmeter; // �����w�j
	int Accident; // ����
	int Tp; // �O��
	int VCB_ON; // VCB�S��
	int VCB_OFF; // VCB�S��
	int VCB; // VCB
	int tmr_ACDC;
	int step_ACDC;
	int alert_ACDC;

	// �葬�\��
	// TODO

	// INI�t�@�C������ݒ�
	int ini_cutPressTime; // �u���[�L�����������鎞��
	int ini_lbInit; // ����N�����͍s�x��
	int ini_timsUpdate; // TIMS��1�s�̍X�V�ɂ����鎞��

	CTims(void)
	{
	}

	virtual ~CTims(void)
	{
	}

	void initialize(void)
	{
		Kind = 0; // ��Ԏ��
		Charactor = 26; // ��Ԕԍ�(�L��)
		From = 0; // �w���\��(�n��)
		Destination = 0; // �w���\��(�s��)
		This = 0; // �w���\��(���w)
		Next = 0; // �w���\��(���w)
		NextBlink = 0; // �w���\���̓_��
		Crawl = 0; // ���s���
		CrawlLimit = 0; // ���s���x
		AfterChara = 0; // ���^�p�L��
		LastStopTrack = 0; // �I�_�����Ԑ�
		RelayName = 0; // �s�H�I�_�w��
		TrainArrow = 0; // �s�H�\���

		m_dist = 0; // ��~�\��_����

		m_pushUpFlag = -1; // �\���X�V�̃t���O
		m_pushUpBeacon = 0; // �\���X�V�̒n��q
		m_pushUpCount = 0; // �\���X�V�̌J��Ԃ���
		m_tmrVisible = *Time; // ���j�^�̃X�e�b�v�X�V

		m_thisName = 0; // ���w��
		m_nextName = 0; // ���w��

		m_pathStaDist = 0; // �J�ʏ��̉w�ʒu���

		m_distance = 0; // ���s����
		m_direction = 1; // �������Z�̔���
		m_distDef = 0; // ��Ԉʒu�̕␳

		ResetArray(); // �z��̏�����
		// PushNext(); // ���w���ŐV�ɂ���
		// Receive(0); // �e�[�u�����X�V����

		Distance1000 = 0; // ���s����(km�̌�)
		Distance100 = 0; // ���s����(100m�̌�)
		Distance10 = 0; // ���s����(10m�̌�)
		Location1000 = 0; // ��Ԉʒu(km�̌�)
		Location100 = 0; // ��Ԉʒu(100m�̌�)
		Location10 = 0; // ��Ԉʒu(10m�̌�)

		TimsSpeed100 = 10; // TIMS���x(100�̌�)
		TimsSpeed10 = 10; // TIMS���x(10�̌�)
		TimsSpeed1 = 0; // TIMS���x(1�̌�)

		BrakeDelay = 0; // �u���[�L�w��
		BcCaution = 0; // 200kPa�x��
		BcPressCut = 0; // �u���[�L��������
		RelayState = 0; // ���s���m�����[
		Direction = 0; // �i�s����
		ArrowDirection = 0; // �i�s�������

		Speed = 0; // ���x�v
		Speed100 = 10; // ���x(100�̌�)
		Speed10 = 10; // ���x(10�̌�)
		Speed1 = 0; // ���x(1�̌�)
		BcPress0 = 10; // �u���[�L�V�����_�w�j(0-180kPa)
		BcPress1 = 10; // �u���[�L�V�����_�w�j(200-380kPa)
		BcPress2 = 10; // �u���[�L�V�����_�w�j(400-580kPa)
		BcPress3 = 10; // �u���[�L�V�����_�w�j(600-780kPa)
		MrPress0 = 10; // ����C�_���w�j(700-790kPa)
		MrPress1 = 10; // ����C�_���w�j(800-890kPa)
		MrPress2 = 10; // ����C�_���w�j(900-990kPa)
		Ammeter = 0; // �d���v

		SlideArrow = *c_DAtcSlideArrow; // ATC�X���C�h���x
		AccelCutting = 0; // �͍s�x��

		m_tmrBcPressCut = 0; // �u���[�L�����̃^�C�}�[

		m_relayD = ATS_SOUND_STOP; // ���s���m�����[
		m_lbInit = ATS_SOUND_STOP; // ����N�����f���퉹
		m_airHigh = ATS_SOUND_STOP; // ���u���[�L�ɉ���
		AirApply = ATS_SOUND_STOP; // �u���[�L������
		m_bcPrsDelta = 0.0F; // �u���[�L�V�����_����[kPa]
		m_emgAnnounce = ATS_SOUND_STOP; // ���u���[�L����
		m_airApplyEmg = ATS_SOUND_STOP; // �u���[�L������(���)
		m_updateInfo = ATS_SOUND_STOP; // �^�s���X�V
		m_deltaUpdate = *Time - UPDATE_SPAN; // �O��^�s���X�V

		// �𒼐ؑ�
		AC = 0; // ��
		DC = 1; // ����
		CVacc = 0; // ����d���ُ�
		CVacc10 = 10; // ����d��(10��)
		CVacc1 = 10; // ����d��(1��)
		ACacc = 0; // �𗬓d���ُ�
		ACacc10000 = 10; // �𗬓d��(10000��)
		ACacc1000 = 10; // �𗬓d��(1000��)
		ACacc100 = 10; // �𗬓d��(100��)
		DCacc = 0; // �����d���ُ�
		DCacc1000 = 10; // �����d��(1000��)
		DCacc100 = 10; // �����d��(100��)
		DCacc10 = 10; // �����d��(10��)
		Cvmeter = 100; // ����w�j
		Acmeter = 0; // �𗬎w�j
		Dcmeter = 1470; // �����w�j
		Accident = 0; // ����
		Tp = 0; // �O��
		VCB_ON = 1; // VCB�S��
		VCB_OFF = 0; // VCB�S��
		VCB = 1; // VCB
		tmr_ACDC = *Time;
		step_ACDC = 3;
		alert_ACDC = 0;
	}

	void execute(void)
	{
		// ���t���[��
		float speed = fabsf(*TrainSpeed); // ���x�̐�Βl[km/h]
		float def = speed / 3600 * *DeltaT; // 1�t���[���œ���������(��Βl)[m]
		float def2 = *TrainSpeed / 3600 * *DeltaT; // 1�t���[���œ���������(���Βl)[m]
		m_dist -= def; // �c�苗�������Z����

		// �𒼐ؑ�
		int temp_time = *Time - tmr_ACDC;
		switch(step_ACDC)
		{
		case 0: // VCB��
			VCB_OFF = 1;
			VCB_ON = 0;
			VCB = 2;
			
			if(temp_time > 250)
			{
				Accident = 1; // ����
			}
			if(temp_time > 500)
			{
				DCacc = 1; // �����d���ُ�
				DCacc1000 = 10; // �����d��(1000��)
				DCacc100 = 10; // �����d��(100��)
				DCacc10 = 10; // �����d��(10��)
				Dcmeter = 0; // �����w�j
			}
			if(temp_time > 750)
			{
				CVacc = 1; // ����d���ُ�
				Cvmeter = 83; // ����w�j
				CVacc10 = 8; // ����d��(10��)
				CVacc1 = 3; // ����d��(1��)
			}
			if(temp_time > 1000)
			{
				Tp = 1; // �O��
			}
			break;
		case 1: // ���d���
			VCB_OFF = 0;
			VCB_ON = 0;
			VCB = 0;

			AC = 0;
			DC = 0;
			ACacc = 0; // �𗬓d���ُ�
			ACacc10000 = 10; // �𗬓d��(10000��)
			ACacc1000 = 10; // �𗬓d��(1000��)
			ACacc100 = 10; // �𗬓d��(100��)
			DCacc = 0; // �����d���ُ�
			DCacc1000 = 10; // �����d��(1000��)
			DCacc100 = 10; // �����d��(100��)
			DCacc10 = 10; // �����d��(10��)
			Acmeter = 0; // �𗬎w�j
			Dcmeter = 0; // �����w�j
			break;
		case 2: // AC�L�d���
			VCB_OFF = 0;
			VCB_ON = 1;
			VCB = 1;

			DC = 0;
			DCacc = 0; // �����d���ُ�
			DCacc1000 = 10; // �����d��(1000��)
			DCacc100 = 10; // �����d��(100��)
			DCacc10 = 10; // �����d��(10��)
			Dcmeter = 0; // �����w�j

			AC = 1;
			if(temp_time > 500)
			{
				ACacc = 1; // �𗬓d���ُ�
				ACacc10000 = 10; // �𗬓d��(10000��)
				ACacc1000 = 2; // �𗬓d��(1000��)
				ACacc100 = 2; // �𗬓d��(100��)
				Acmeter = 220; // �𗬎w�j
			}
			if(temp_time > 1000)
			{
				ACacc = 1; // �𗬓d���ُ�
				ACacc10000 = 1; // �𗬓d��(10000��)
				ACacc1000 = 1; // �𗬓d��(1000��)
				ACacc100 = 2; // �𗬓d��(100��)
				Acmeter = 11200; // �𗬎w�j
			}
			if(temp_time > 2000)
			{
				ACacc = 0; // �𗬓d���ُ�
				ACacc10000 = 10; // �𗬓d��(10000��)
				ACacc1000 = 10; // �𗬓d��(1000��)
				ACacc100 = 10; // �𗬓d��(100��)
				Acmeter = 15500; // �𗬎w�j
			}
			if(temp_time > 3000)
			{
				ACacc = 0; // �𗬓d���ُ�
				ACacc10000 = 10; // �𗬓d��(10000��)
				ACacc1000 = 10; // �𗬓d��(1000��)
				ACacc100 = 10; // �𗬓d��(100��)
				Acmeter = 18000; // �𗬎w�j
			}
			if(temp_time > 4000)
			{
				ACacc = 0; // �𗬓d���ُ�
				ACacc10000 = 10; // �𗬓d��(10000��)
				ACacc1000 = 10; // �𗬓d��(1000��)
				ACacc100 = 10; // �𗬓d��(100��)
				Acmeter = 20000; // �𗬎w�j
			}
			if(temp_time > 5000)
			{
				ACacc = 0; // �𗬓d���ُ�
				ACacc10000 = 10; // �𗬓d��(10000��)
				ACacc1000 = 10; // �𗬓d��(1000��)
				ACacc100 = 10; // �𗬓d��(100��)
				Acmeter = 22000; // �𗬎w�j
			}
			if(temp_time > 8000)
			{
				Accident = 0; // ����
			}
			if(temp_time > 12000)
			{
				Tp = 0; // �O��
			}
			if(temp_time > 13000)
			{
				CVacc = 0; // ����d���ُ�
				CVacc10 = 10; // ����d��(10��)
				CVacc1 = 10; // ����d��(1��)
				Cvmeter = 100; // ����w�j
			}
			break;
		case 3: // DC�L�d���
			AC = 0;
			ACacc = 0; // �𗬓d���ُ�
			Acmeter = 0; // �𗬎w�j
			break;
		}

		// ����N�����͍s�x��
		if(ini_lbInit > 0 && *PowerNotch > 0)
		{
			ini_lbInit = 0;
			AccelCutting = *Time + 500;
			m_lbInit = ATS_SOUND_PLAY;
		}

		// ����N�����f���퉹
		LbInit = m_lbInit;
		m_lbInit = ATS_SOUND_CONTINUE;

		// �u���[�L����
		if(m_tmrBcPressCut < *Time)
		{
			BcPressCut = 0;
			m_tmrBcPressCut = 0;
		}
		else
		{
			BcPressCut = 1;
		}

		// ���s���m�����[
		if(RelayState == 0 && speed >= RELAY_REQUIREMENT)
		{
			RelayState = 1;
			m_relayD = ATS_SOUND_PLAY;
		}
		else if(RelayState == 1 && speed <= RELAY_REQUIREMENT)
		{
			RelayState = 0;
			m_relayD = ATS_SOUND_PLAY;
		}

		RelayD = m_relayD;
		m_relayD = ATS_SOUND_CONTINUE;

		// ���u���[�L�ɉ���
		AirHigh = m_airHigh;
		m_airHigh = ATS_SOUND_CONTINUE;

		// �u���[�L������
		if(*BrakeNotch > 0 && BcPressure > 40.0F && BcPressure > m_bcPrsDelta)
		{
			AirApply = ATS_SOUND_PLAYLOOPING;
		}
		else
		{
			AirApply = ATS_SOUND_STOP;
		}

		m_bcPrsDelta = BcPressure; // �u���[�L�V�����_����[kPa]

		// ���u���[�L����
		EmgAnnounce = m_emgAnnounce;
		m_emgAnnounce = ATS_SOUND_CONTINUE;

		// �u���[�L������(���)
		AirApplyEmg = m_airApplyEmg;
		m_airApplyEmg = ATS_SOUND_CONTINUE;

		// �^�s���X�V
		if(*TrainSpeed > 10.0f && *Time > m_deltaUpdate + UPDATE_SPAN)
		{
			m_deltaUpdate = *Time;

			if(rand()%200 == 0)
			{
				m_updateInfo = ATS_SOUND_PLAY;
			}
		}

		UpdateInfo = m_updateInfo; // �^�s���X�V
		m_updateInfo = ATS_SOUND_CONTINUE;

		// ���w��_�ł�����
		if(speed * speed / TIMS_DECELERATION >= m_dist - 50 && m_dist > 0 && !m_blinking) // ���x�ƍ� (�p�^�[��)
		{
			m_blinking = true;

			if(m_pushUpFlag == 1) // ��ԉw�̂Ƃ�
			{
				m_pushUpFlag = 2;
			}
		}

		// �w���\���̓_��
		NextBlink = m_pushUpFlag == 2 ? ((*Time % 750) / 375) : 1;

		// �J�ʏ��25m����(0���\���A1���J�ʋ�ԁA2��~���E1�A3��~���E2�A4��~���E3�A5��~���E4�A6�J�ʋ��1�A7�J�ʋ��2�A8�w���1�A9�w���2)
		for(int initPath2=0;initPath2<120;initPath2++)
		{
			// �J�ʋ�Ԃ̕\��(��)
			TrackPath2[initPath2] = 6;
		}

		// �J�ʏ��̉w�ʒu���
		m_pathStaDist -= def2; // �c�苗�������Z����

		if(m_pathStaDist > 0)
		{
			int pathStaDistfabsf25 = (int)fabsf(m_pathStaDist) / 25;

			// �w��Ԃ̕\��(������)
			if(TrackPath2[pathStaDistfabsf25] >= 6){TrackPath2[pathStaDistfabsf25] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 1] >= 6){TrackPath2[pathStaDistfabsf25 - 1] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 2] >= 6){TrackPath2[pathStaDistfabsf25 - 2] = 9;}
			if(TrackPath2[pathStaDistfabsf25 - 3] >= 6){TrackPath2[pathStaDistfabsf25 - 3] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 4] >= 6){TrackPath2[pathStaDistfabsf25 - 4] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 5] >= 6){TrackPath2[pathStaDistfabsf25 - 5] = 9;}
			if(TrackPath2[pathStaDistfabsf25 - 6] >= 6){TrackPath2[pathStaDistfabsf25 - 6] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 7] >= 6){TrackPath2[pathStaDistfabsf25 - 7] = 8;}
			if(TrackPath2[pathStaDistfabsf25 - 8] >= 6){TrackPath2[pathStaDistfabsf25 - 8] = 8;}
		}

		// �J�ʏ��̕������
		for(int i=0;i<10;i++)
		{
			// �c�苗�������Z����
			m_pathSecDist[i] -= def2;
			m_passSecLine[i] -= def2;

			// ������؂���̕\��
			if(m_passSecLine[i] > 0)
			{
				int passSecLinefabsf25 = (int)fabsf(m_passSecLine[i]) / 25; // ��؂������ꂽ���n�_

				if(TrackPath2[passSecLinefabsf25] == 6){TrackPath2[passSecLinefabsf25] = 7;} // �J�ʋ��1�Ȃ��؂��������
			}

			// ���s���E�̕\��
			if(m_pathSecDist[i] > 0)
			{
				int pathSecDistfabsf25 = (int)fabsf(m_pathSecDist[i]) / 25; // ���s�ł�����E�̒n�_
				TrackPath2[pathSecDistfabsf25] = 5; // ��~���E�̕\��(�Ԃ��O�p�`)
				TrackPath2[pathSecDistfabsf25 + 1] = 4; // ��~���E�̕\��(�Ԃ��O�p�`)
				TrackPath2[pathSecDistfabsf25 + 2] = 3; // ��~���E�̕\��(�Ԃ��O�p�`)
				TrackPath2[pathSecDistfabsf25 + 3] = 2; // ��~���E�̕\��(�Ԃ��O�p�`)

				for(int pathNone2=pathSecDistfabsf25+4;pathNone2<120;pathNone2++)
				{
					TrackPath2[pathNone2] = 1; // ���J�ʋ�Ԃ̕\��(����)
				}
			}
		}

		// �O���X�R�b�N�s�b�g
		m_tmrUpdate -= abs(*DeltaT); // ���t���[�����Z
		if(m_tmrUpdate < 0) // || *TrainSpeed == 0) // �X�V����
		{
			Speed = fabs(*TrainSpeed);
			Speed100 = fabs(*TrainSpeed) / 100;
			Speed10 = (fabs(*TrainSpeed) / 10) - (Speed100 * 10);
			Speed1 = fabs(*TrainSpeed) - (Speed100 * 100 + Speed10 * 10);

			if(*TrainSpeed < 1000 && Speed100 == 0){Speed100 = 10;}
			if(*TrainSpeed < 100 && Speed10 == 0){Speed10 = 10;}

			/* 2015/01/31 unic: �}�X�R���L�[�������Ńu���[�L�w�߂����f���ꂵ�܂� */
			BrakeDelay = *BrakeNotch; // �u���[�L�w��
			BcCaution = BcPressure < 200 && !m_door; // 200kPa�x��

			BcPress0 = 10; // �u���[�L�V�����_�w�j(0-180kPa)
			BcPress1 = 10; // �u���[�L�V�����_�w�j(200-380kPa)
			BcPress2 = 10; // �u���[�L�V�����_�w�j(400-580kPa)
			BcPress3 = 10; // �u���[�L�V�����_�w�j(600-780kPa)
			MrPress0 = 10; // ����C�_���w�j(700-790kPa)
			MrPress1 = 10; // ����C�_���w�j(800-890kPa)
			MrPress2 = 10; // ����C�_���w�j(900-990kPa)

			if(BcPressure < 200){BcPress0 = BcPressure / 20;} // �u���[�L�V�����_�w�j(0-180kPa)
			else if(BcPressure < 400){BcPress1 = (BcPressure - 200) / 20;} // �u���[�L�V�����_�w�j(200-380kPa)
			else if(BcPressure < 600){BcPress2 = (BcPressure - 400) / 20;} // �u���[�L�V�����_�w�j(400-580kPa)
			else if(BcPressure < 800){BcPress3 = (BcPressure - 600) / 20;} // �u���[�L�V�����_�w�j(600-780kPa)

			if(MrPressure <= 700){MrPress0 = 10;} // 700kPa�ȉ�
			else if(MrPressure > 700 && MrPressure < 800){MrPress0 = (MrPressure - 700) / 10;} // ����C�_���w�j(700-790kPa)
			else if(MrPressure < 900){MrPress1 = (MrPressure - 800) / 10;} // ����C�_���w�j(800-890kPa)
			else if(MrPressure < 1000){MrPress2 = (MrPressure - 900) / 10;} // ����C�_���w�j(900-990kPa)

			Ammeter = Current; // �d���v
			SlideArrow = *c_DAtcSlideArrow; // ATC�X���C�h���x

			m_tmrUpdate = 200 + (*Time % 50) * 5;
		}

		// �E���j�^�[���x�v
		m_distance += (def2 / 10) * m_direction; // ���t���[�����Z
		m_tmrTims -= abs(*DeltaT); // ���t���[�����Z
		if(m_tmrTims < 0) // || *TrainSpeed == 0) // �X�V����
		{
			Distance1000 = fabs(m_distance) / 100; // ���s����(km�̌�)
			Distance100 = (fabs(m_distance) / 10) - (Distance1000 * 10); // ���s����(100m�̌�)
			Distance10 = fabs(m_distance) - (Distance1000 * 100 + Distance100 * 10); // ���s����(10m�̌�)

			double tmp_location = Location + m_distDef; // ��Ԉʒu�␳��K�p����
			Location1000 = tmp_location / 1000; // ��Ԉʒu(km�̌�)
			Location100 = (tmp_location / 100) - (Location1000 * 10); // ��Ԉʒu(100m�̌�)
			Location10 = (tmp_location / 10) - (Location1000 * 100 + Location100 * 10); // ��Ԉʒu(10m�̌�)

			TimsSpeed100 = fabs(*TrainSpeed) / 100;
			TimsSpeed10 = (fabs(*TrainSpeed) / 10) - (TimsSpeed100 * 10);
			TimsSpeed1 = fabs(*TrainSpeed) - (TimsSpeed100 * 100 + TimsSpeed10 * 10);

			if(*TrainSpeed < 1000 && TimsSpeed100 == 0){TimsSpeed100 = 10;}
			if(*TrainSpeed < 100 && TimsSpeed10 == 0){TimsSpeed10 = 10;}

			m_tmrTims = 1200;
		}

		// ���j�b�g�\��
		for(int i=0;i<10;i++) // ��Ԃ̍X�V
		{
			// TIMS�p
			if(Current > 0 && UnitTims[i] != 1 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 1;}
			else if(Current < 0 && UnitTims[i] != 2 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 2;}
			else if(Current == 0 && UnitTims[i] != 0 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 0;}

			// �\�����p
			if(Current != 0 && UnitState[i] != 1 && rand() % RANDOMIZE_UNITLAMP == 0){UnitState[i] = 1;}
			else if(Current == 0 && UnitState[i] != 0 && rand() % RANDOMIZE_UNITLAMP == 0){UnitState[i] = 0;}
		}

		// �i�s�������
		switch(Direction)
		{
		case 0:
			ArrowDirection = 0;
			break;
		case 1:
			switch(*Reverser)
			{
			case 1:
				ArrowDirection = 1;
				break;
			case -1:
				ArrowDirection = 2;
				break;
			case 0:
			default:
				ArrowDirection = 0;
				break;
			}
			break;
		case 2:
			switch(*Reverser)
			{
			case 1:
				ArrowDirection = 2;
				break;
			case -1:
				ArrowDirection = 1;
				break;
			case 0:
			default:
				ArrowDirection = 0;
				break;
			}
			break;
		}

		// �v�b�V���A�b�v�C�x���g
		if(*TrainSpeed != 0) // �w�W�����v�����O����
		{
			if((m_pushUpFlag == 2 && (m_pushUpBeacon == 1 || *TrainSpeed < 2.0f)) || (m_pushUpFlag == -1 && m_dist <= 0))
			{
				m_pushUpFlag = 0;

				// �X�e�b�v�X�V�̉񐔂������[�v
				for( ;m_pushUpCount>0;m_pushUpCount--)
				{
					if(m_pushUpCount > 1)
					{
						for(int i=0;i<10;i++)
						{
							PushUp(i);
						}
					}
					else
					{
						m_tmrVisible = *Time;
						for(int i=0;i<10;i++)
						{
							m_update[i] = 1;

							// ���w���ŐV�ɂ���
							if(i == 3)
							{
								PushNext();
							}
						}
					}
				}
			}
		}
		else if(m_pushUpFlag == 3 || m_pushUpBeacon == 2) // �w�W�����v�̂Ƃ�
		{
			m_pushUpFlag = 0;
			PushNext(); // ���w���ŐV�ɂ���

			for(int i=0;i<10;i++)
			{
				PushUp(i);
			}
		}
		else if(m_pushUpFlag == -1) // �N�����̏�����
		{
			PushNext(); // ���w���ŐV�ɂ���
		}

		// �X�e�b�v�X�V�̏���
		for(int i=0;i<10;i++)
		{
			if(*Time >= m_tmrVisible + (i * (ini_timsUpdate * (2 / 3))) && *Time <= m_tmrVisible + ini_timsUpdate * (i + 1) - (ini_timsUpdate * (1 / 3)))
			{
				HiddenLine[i] = 1;

				if(m_update[i] == 1)
				{
					m_update[i] = 0;
					PushUp(i);
				}
			}
			else
			{
				HiddenLine[i] = 0;
			}
		}
	}

	// �h�A���J����
	void DoorOpening(void)
	{
		m_door = 0; // �h�A���
		m_tmrVisible = 0; // �\���̍X�V�������Ȃ�
	}

	// �h�A���܂���
	void DoorClosing(void)
	{
		m_door = 1; // �h�A���
	}

	// ���w�ڋ߂���
	void Receive(int data)
	{
		m_pushUpFlag = data >= 0 ? 1 : -1;
		m_pushUpBeacon = 0;
		m_pushUpCount = abs(data) > 100000 ? 0 : abs(data) / 10000 > 0 ? abs(data) / 10000 : 1;

		m_dist = abs(data % 10000) - TIMS_OFFSET;
		m_blinking = false;

		m_pathStaDist = abs(data % 10000) + 5.0F;

		if(*TrainSpeed == 0) // �w�W�����v�����O����
		{
			// m_pushUpFlag = abs(data) < 1000000 ? 3 : 0;
			m_pushUpFlag = abs(data) < 100000 ? 3 : 0;
			m_pushUpCount = 0;
			m_dist = 0;
			m_pathStaDist = 0;
		}
	}

	// �\�����X�V����
	void UpdateView(void)
	{
		m_pushUpBeacon = *TrainSpeed != 0 ? 1 : 2; // �\���X�V�̒n��q
	}

	// ���w�A���w�̐ݒ�
	void SetNext(int data)
	{
		if(data >= 10000)
		{
			m_nextTime[0] = data / 10000;
			m_nextTime[1] = ((data % 10000) - (data % 100)) / 100;
			m_nextTime[2] = (data % 100) / 5;
		}
		else
		{
			m_thisName = data / 100;
			m_nextName = data % 100;
		}
	}

	// ��Ԏ�ʂ̐ݒ�
	void SetKind(int data)
	{
		Kind = data;
	}

	// ��Ԕԍ��̐ݒ�
	void SetNumber(int data)
	{
		Charactor = data / 10000;
		Number[0] = ((data % 10000) - (data % 1000)) / 1000;
		Number[1] = ((data % 1000) - (data % 100)) / 100;
		Number[2] = ((data % 100) - (data % 10)) / 10;
		Number[3] = data % 10;
	}

	// �^�s��Ԃ̐ݒ�
	void SetLeg(int data)
	{
		From = data / 100;
		Destination = data % 100;
	}

	// ���s��Ԃ̐ݒ�
	void SetCrawl(int data)
	{
		if(data > 0)
		{
			Crawl = 1;
			CrawlLimit = data / 5;
		}
		else
		{
			Crawl = 0;
			CrawlLimit = 0;
		}
	}

	// ���^�p�̐ݒ�
	void SetAfteruent(int type, int data)
	{
		if(type == 0) // ��Ԕԍ�
		{
			AfterChara = data / 10000;
			AfterNumber[0] = ((data % 10000) - (data % 1000)) / 1000;
			AfterNumber[1] = ((data % 1000) - (data % 100)) / 100;
			AfterNumber[2] = ((data % 100) - (data % 10)) / 10;
			AfterNumber[3] = data % 10;
		}
		else if(type == 1) // ��������
		{
			AfterTime[0][0] = data / 10000;
			AfterTime[0][1] = ((data % 10000) - (data % 100)) / 100;
			AfterTime[0][2] = (data % 100) / 5;
		}
		else if(type == 2) // ���Ԏ���
		{
			AfterTime[1][0] = data / 10000;
			AfterTime[1][1] = ((data % 10000) - (data % 100)) / 100;
			AfterTime[1][2] = (data % 100) / 5;
		}
	}

	// �i�s�����ݒ�
	void SetDirection(int direction)
	{
		switch(direction)
		{
		case 1: // ��
			Direction = 1;
			break;
		case 2: // �E
			Direction = 2;
			break;
		case 0:
		default: // ��O
			Direction = 0;
			break;
		}
	}

	// �I�_�w�̐ݒ�
	void SetLastStop(int type, int data)
	{
		if(type == 0) // ��������
		{
			LastStopTime[0] = data / 10000;
			LastStopTime[1] = ((data % 10000) - (data % 100)) / 100;
			LastStopTime[2] = (data % 100) / 5;
		}
		else if(type == 1) // �����Ԑ�
		{
			LastStopTrack = data;
		}
	}

	// �s�H�I�_�w���ݒ�
	void SetRelayStation(int data)
	{
		RelayName = data;
	}

	// �s�H�\���̐ݒ�
	void SetArrowState(int data)
	{
		TrainArrow = data;
	}

	// ���s�����̐ݒ�
	void SetDistance(int data)
	{
		if(data >= 10000) // �������Z���邩�ǂ���
		{
			m_direction = -1;
		}
		else 
		{
			m_direction = 1;
		}

		m_distance = data % 10000; // ���s����
	}

	// ��Ԉʒu�␳�̐ݒ�
	void SetPositionDef(int data)
	{
		m_distDef = data; // ��Ԉʒu�̕␳
	}

	// �X�^�t�e�[�u���̏�������
	void InputLine(int colum, int line, int data)
	{
		switch(colum) // ��̐U�蕪��
		{
		case 0:
			Span[line][0] = data / 100;
			Span[line][1] = (data % 100) / 5;
			break;
		case 1:
			Station[line] = data;
			break;
		case 2:
			Arrive[line][0] = data / 10000;
			Arrive[line][1] = ((data % 10000) - (data % 100)) / 100;
			Arrive[line][2] = (data % 100) / 5;
			break;
		case 3:
			Leave[line][0] = data / 10000;
			Leave[line][1] = ((data % 10000) - (data % 100)) / 100;
			Leave[line][2] = (data % 100) / 5;
			break;
		case 4:
			Track[line] = data;
			break;
		case 5:
			Limit[line] = data / 5;
			break;
		}
	}

	// �J�ʏ��̒�~���������X�V����
	/*
	void CheckPath(int signal, float dist=0, int section=0)
	{
		if((signal == 0 || signal == 9 || signal == 10) && dist > 0)
		{
			m_pathSecDist[section] = dist; // �J�ʏ��̕������
			m_passSecLine[section] = 0; // �J�ʏ��̕�����؂��
		}
		else
		{
			m_pathSecDist[section] = 0;
			m_passSecLine[section] = dist;
		}
	}
	*/
	void CheckPath(int signal, float dist = 0, int section = 0)
	{
		if (section < 0 || section >= MAX_SECTION_COUNT) return; // �͈̓`�F�b�N�ǉ�

		if ((signal == 0 || signal == 9 || signal == 10) && dist > 0.0f)
		{
			m_pathSecDist[section] = dist;
			m_passSecLine[section] = 0;
		}
		else
		{
			m_pathSecDist[section] = 0;
			m_passSecLine[section] = dist;
		}
	}

	// ���u���[�L�ɉ������Đ�����
	void PlaySoundAirHigh(int state, int newState)
	{
		if(state == EmergencyNotch && newState < EmergencyNotch && BcPressure > 340)
		{
			m_airHigh = ATS_SOUND_PLAY;
			m_tmrBcPressCut = *Time + ini_cutPressTime; // �u���[�L�����̃^�C�}�[
		}
	}

	// ���u���[�L�������Đ�����
	void PlaySoundEmgAnnounce(int state, int newState)
	{
		if(state != EmergencyNotch && newState == EmergencyNotch && *TrainSpeed > 10.0F)
		{
			m_emgAnnounce = ATS_SOUND_PLAY;
		}
	}

	// ����N�����f���퉹�����Z�b�g����
	void SetLbInit(int pos, int enable)
	{
		if(pos == 0 && enable == 1)
		{
			ini_lbInit = 1;
		}
	}

	// �u���[�L������(���)���Đ�����
	void PlaySoundAirApplyEmg(int state, int newState)
	{
		if(state != EmergencyNotch && newState == EmergencyNotch && BcPressure < 150.0f)
		{
			m_airApplyEmg = ATS_SOUND_PLAY;
		}
	}

	// �𒼐ؑւ��s��
	void SetACDC(int state)
	{
		step_ACDC = state;
		tmr_ACDC = *Time;
	}

	// �𒼐ؑւ̂��m�点
	void AlartACDC(int state)
	{
		alert_ACDC = state;
	}

	// �����_���V�[�h��ύX����
	void RandomizeSeed(void)
	{
		srand(*Time);
	}

private:
	int m_door; // �h�A���
	float m_dist; // ��~�\��_����
	bool m_blinking; // ���w�_�ł��ǂ���

	int m_pushUpFlag; // �\���X�V�̃t���O
	int m_pushUpBeacon; // �\���X�V�̒n��q
	int m_pushUpCount; // �\���X�V�̌J��Ԃ���
	int m_tmrVisible; // ���j�^�̃X�e�b�v�X�V
	int m_update[7]; // �X�e�b�v�X�V�̏��
	int m_thisName; // ���w��
	int m_nextName; // ���w��
	int m_nextTime[3]; // ���w����

	float m_pathStaDist; // �J�ʏ��̉w�ʒu���
	float m_pathSecDist[10]; // �J�ʏ��̕������
	float m_passSecLine[10]; // �J�ʏ��̕�����؂��

	double m_distance; // ���s����
	int m_direction; // �������Z���邩�ǂ���
	double m_distDef; // ��Ԉʒu�̕␳

	int m_tmrUpdate; // ���x�v�̍X�V�^�C�}�[
	int m_tmrTims; // �E���j�^�[�̍X�V�^�C�}�[
	int m_tmrBcPressCut; // �u���[�L�����̃^�C�}�[

	int m_relayD; // ���s���m�����[
	int m_lbInit; // ����N�����f���퉹
	int m_airHigh; // ���u���[�L�ɉ���
	int m_airApply; // �u���[�L������
	float m_bcPrsDelta; // �u���[�L�V�����_����[kPa]
	int m_emgAnnounce; // ���u���[�L����
	int m_airApplyEmg; // �u���[�L������(���)
	int m_updateInfo; // �^�s���X�V
	int m_deltaUpdate; // �O��^�s���X�V

	// �X�^�t�e�[�u���̃v�b�V���A�b�v
	void PushUp(int line)
	{
		if(line == 9)
		{
			Station[line] = 0;
			Track[line] = 0;
			Limit[line] = 0;
			Span[line][0] = 0;
			Span[line][1] = 0;
		}
		else
		{
			Station[line] = Station[line + 1];
			Track[line] = Track[line + 1];
			Limit[line] = Limit[line + 1];
			Span[line][0] = Span[line + 1][0];
			Span[line][1] = Span[line + 1][1];
		}

		for(int j=0;j<3;j++)
		{
			if(line == 9)
			{
				switch(j)
				{
				case 0:
					Arrive[line][j] = 24;
					Leave[line][j] = 24;
					break;
				case 1:
					Arrive[line][j] = 60;
					Leave[line][j] = 60;
					break;
				case 2:
					Arrive[line][j] = 12;
					Leave[line][j] = 12;
					break;
				}
				
			}
			else
			{
				Arrive[line][j] = Arrive[line + 1][j];
				Leave[line][j] = Leave[line + 1][j];
			}
		}
	}

	// ���͂���Ă��鎟�w����ʂɔ��f
	void PushNext(void)
	{
		This = m_thisName;
		Next = m_nextName;
		NextTime[0] = m_nextTime[0];
		NextTime[1] = m_nextTime[1];
		NextTime[2] = m_nextTime[2];
	}

	// �z��̏�����
	void ResetArray(void)
	{
		for(int c3=0;c3<3;c3++) // 3�񃋁[�v
		{
			LastStopTime[c3] = 0;
			m_nextTime[c3] = 0;

			for(int c3_2=0;c3_2<2;c3_2++) // 2�񃋁[�v
			{
				AfterTime[c3_2][c3] = 0;
			}
		}

		for(int c4=0;c4<4;c4++) // 4�񃋁[�v
		{
			Number[c4] = 10;
			AfterNumber[c4] = 10;
		}

		for(int c10=0;c10<10;c10++) // 10�񃋁[�v
		{
			Station[c10] = 0;
			PassSta[c10] = 0;
			Track[c10] = 0;
			Limit[c10] = 0;

			HiddenLine[c10] = 0;
			m_update[c10] = 0;

			TrackPath[c10] = 0;
			UnitTims[c10] = 0;
			UnitState[c10] = 0;
			m_pathSecDist[c10] = 0;
			m_passSecLine[10] = 0;

			// �����e�[�u���ɂ� 24:60:60 (24, 60, 60) ���o��
			for(int c10_3=0;c10_3<3;c10_3++) // 10�񂻂ꂻ���3�񂸂��[�v
			{
				switch(c10_3)
				{
				case 0:
					Arrive[c10][c10_3] = 24;
					Leave[c10][c10_3] = 24;
					Span[c10][c10_3] = 0;
					break;
				case 1:
					Arrive[c10][c10_3] = 60;
					Leave[c10][c10_3] = 60;
					Span[c10][c10_3] = 0;
					break;
				case 2:
					Arrive[c10][c10_3] = 12;
					Leave[c10][c10_3] = 12;
					break;
				}
			}
		}

		for(int c120=0;c120<120;c120++) // 120�񃋁[�v
		{
			TrackPath2[c120] = 0;
		}
	}

};	// CTims

#endif	// ATS_TIMS_HPP_INCLUDED
