#include "stdafx.h"
#include "atsplugin.h"
#include "Atssn.h"
#include "Atsp.h"
#include "Spp.h"
#include "AtsPs.hpp"
#include "DAtc.hpp"
#include "Eb.hpp"
#include "Tims.hpp"
#include "Date.hpp"
#include "Sub.hpp"
#include "Accel.hpp"
#include "NewSnp2Cfg.h"
#include "Ats.h"

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_atssn.Time = &g_time;
		g_atssn.Notch = &g_brakeNotch;

		g_atsp.TrainSpeed = &g_speed;
		g_atsp.DeltaT = &g_deltaT;
		g_atsp.BrakeNotch = &g_brakeNotch;
		g_atsp.Reverser = &g_reverser;

		g_atsps.BrakeNotch = &g_brakeNotch;
		g_atsps.TrainSpeed = &g_speed;
		g_atsps.DeltaT = &g_deltaT;

		g_datc.BrakeNotch = &g_brakeNotch;
		g_datc.Time = &g_time;
		g_datc.TrainSpeed = &g_speed;
		g_datc.DeltaT = &g_deltaT;

		g_spp.TrainSpeed = &g_speed;
		g_spp.DeltaT = &g_deltaT;
		g_spp.BrakeNotch = &g_brakeNotch;

		g_eb.Time = &g_time;
		g_eb.TrainSpeed = &g_speed;

		g_tims.BrakeNotch = &g_brakeNotch;
		g_tims.PowerNotch = &g_powerNotch;
		g_tims.Reverser = &g_reverser;
		g_tims.Time = &g_time;
		g_tims.TrainSpeed = &g_speed;
		g_tims.DeltaT = &g_deltaT;
		g_tims.c_DAtcSlideArrow = &g_datc.SlideArrow;

		g_sub.BrakeNotch = &g_brakeNotch;
		g_sub.PowerNotch = &g_powerNotch;
		g_sub.Reverser = &g_reverser;
		g_sub.TrainSpeed = &g_speed;

		g_accel.Time = &g_time;
		g_accel.TrainSpeed = &g_speed;
		g_accel.DeltaT = &g_deltaT;
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	// CFG�t�@�C���ǂݍ���
	char filePath[_MAX_PATH + 1] = _T(""); // �t�@�C���p�X�i�[
	char* posIni; // ����������ւ̃|�C���^
	bool loadCheck; // INI�t�@�C���̃��[�h�ɐ����������ǂ���

	// Ats.dll�̃t�@�C���p�X���擾
	::GetModuleFileName((HMODULE)hModule, filePath, _MAX_PATH);
	// �p�X����.dll�̈ʒu������
	posIni = strstr(filePath, ".dll");
	// .dll��.cfg�ɒu��
	memmove(posIni, ".cfg", 4);
	// INI�t�@�C�������[�h���Č��ʂ��擾
	loadCheck = g_ini.load(filePath);

	return TRUE;
}

ATS_API void WINAPI Load()
{
}

ATS_API void WINAPI Dispose()
{
}

ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec)
{
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
	g_atssn.EmgNotch = g_emgBrake;
	g_atssn.CancelNotch = vehicleSpec.AtsNotch;
	g_atsp.EmergencyNotch = g_emgBrake;
	g_atsp.ServiceNotch = vehicleSpec.AtsNotch;
	g_atsps.EmergencyNotch = g_emgBrake;
	g_datc.EmergencyNotch = g_emgBrake;
	g_spp.ServiceNotch = vehicleSpec.AtsNotch;
	g_eb.EmergencyNotch = g_emgBrake;
	g_tims.EmergencyNotch = g_emgBrake;
	g_sub.EmergencyNotch = g_emgBrake;
}

ATS_API void WINAPI Initialize(int brake)
{
	g_tims.ini_timsUpdate = g_ini.Tims.line_update >= 0 && g_ini.Tims.line_update < 250 ? g_ini.Tims.line_update : 90; // 0ms����250ms�܂�
	g_tims.ini_cutPressTime = g_ini.Emulate.ebCutPressure; // �u���[�L�����������鎞��
	g_tims.ini_lbInit = g_ini.Emulate.lbInit; // ����N�����͍s�x��
	g_sub.ini_keyEvent = g_ini.Emulate.keyEvent; // �}�X�R���L�[
	g_eb.initialize(g_ini.Emulate.EBEnabled);

	g_atssn.InitSn();
	g_atsp.InitP();
	g_atsps.initialize();
	g_datc.initialize();
	g_spp.InitSpp();
	g_eb.initialize(g_ini.Emulate.EBEnabled);
	g_tims.initialize();
	g_date.initialize();
	g_sub.initialize();
	g_accel.initialize();
	g_speed = 0;
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int* panel, int* sound)
{
	g_deltaT = vehicleState.Time - g_time;
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	g_tims.Location = vehicleState.Location; // ��Ԉʒu[m]
	g_tims.BcPressure = vehicleState.BcPressure; // �u���[�L�V�����_����[kPa]
	g_tims.MrPressure = vehicleState.MrPressure; // ����C�_������[kPa]
	g_tims.Current = vehicleState.Current; // �d��[A]

	g_atssn.RunSn(); // ATS-SN
	g_atsp.RunP(); // ATS-P
	g_atsps.execute(); // ATS-Ps
	g_datc.execute(); // D-ATC
	g_spp.RunSpp(); // ��ԉw�ʉߖh�~���u
	g_eb.execute(); // EB���u
	g_tims.execute(); // TIMS���u
	g_date.update(); // �����@�\
	g_sub.execute(); // ���̑��̋@�\
	g_accel.execute(); // �����x

	// �n���h���o��
	if (g_atssn.AtsBrake || g_atsp.EmergencyBrake || g_datc.AtcBrake == 2 || g_eb.Emergency || g_sub.Key == 0)
	{
		g_output.Brake = g_emgBrake;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;

		g_powerNotch = 0; // ������^�]�h�~
	}
	else if (g_brakeNotch != g_emgBrake && (g_atsp.ServiceBrake || g_datc.AtcBrake == 1))
	{
		g_output.Brake = g_svcBrake;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;

		g_powerNotch = 0; // ������^�]�h�~
	}
	else if (g_tims.BcPressCut == 1)
	{
		g_output.Brake = 1; // EB�ɉ����Ƀu���[�L����������
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;
	}
	else
	{
		g_output.Brake = g_brakeNotch;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;
	}
	if (g_pilotlamp)
	{
		g_output.Reverser = g_reverser;
	}
	else
	{
		g_output.Reverser = 0;
	}
	if (g_time > g_tims.AccelCutting && g_tims.VCB_ON == 1) // �͍s�x����VCB
	{
		g_output.Power = g_powerNotch;
	}
	else
	{
		g_output.Power = 0;
	}
	// g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	for (int i = 0; i < 1024; i++) { panel[i] = 0; }

	// �p�l���o��
	panel[0] = !g_eb.Emergency ? g_atssn.WhiteLamp : (!g_atsp.AtspLamp && g_eb.RedLamp); // ATS���F��
	panel[1] = g_atssn.RedLamp || g_eb.RedLamp; // ATS�ԐF��
	panel[2] = 1; // P�d��
	panel[3] = g_atsp.ApproachLamp; // �p�^�[���ڋ�
	panel[4] = 0; // �u���[�L�J��
	panel[5] = g_atsp.BrakeApplyLamp; // �u���[�L����
	panel[6] = g_atsp.AtspLamp; // ATS-P
	panel[7] = 0; // �̏�
	panel[8] = g_eb.Action; // EB�쓮
	panel[10] = g_time / 3600000; // �f�W�^����
	panel[11] = g_time / 60000 % 60; // �f�W�^����
	panel[12] = g_time / 1000 % 60; // �f�W�^���b

	// ATS-Ps
	panel[9] = g_atsps.AtsPs; // Ps�d�� (��n�E�]�n)
	panel[13] = g_atsps.PatternStart; // �p�^�[������
	panel[14] = g_atsps.PatternApproach; // �p�^�[���ڋ�
	panel[15] = g_atsps.BrakeTurnOn; // �u���[�L����
	panel[16] = g_atsps.BrakeTurnOff; // �u���[�L�J��
	panel[17] = g_atsps.TrainSpeedLED; // ��ԑ��xLED
	panel[18] = g_atsps.PatternSpeedLED; // �p�^�[�����xLED
	panel[19] = g_atsps.BrakeDown; // Ps�̏�

	// D-ATC
	panel[20] = g_datc.DAtc; // D-ATC�d��
	panel[21] = g_datc.IsDAtc; // D-ATC
	panel[22] = 0; // ����
	panel[23] = g_datc.AtcApproach; // �p�^�[���ڋ�
	panel[24] = g_datc.AtcEmg; // ATC���
	panel[25] = g_datc.AtcSvc; // ATC��p
	panel[26] = g_tims.SlideArrow; // 130711����(�t���[������) g_datc.SlideArrow; // ATC�X���C�h���x
	panel[27] = g_datc.OrpNeedle;// ORP���x
	panel[28] = g_datc.Signal[0]; // �Ԍ���
	panel[29] = g_datc.Signal[1]; // ����
	panel[30] = g_datc.Arrow[0]; // ATC-X
	panel[31] = g_datc.Arrow[1]; // ATC-0
	panel[32] = g_datc.Arrow[2]; // ATC-5
	panel[33] = g_datc.Arrow[3]; // ATC-10
	panel[34] = g_datc.Arrow[4]; // ATC-15
	panel[35] = g_datc.Arrow[5]; // ATC-20
	panel[36] = g_datc.Arrow[6]; // ATC-25
	panel[37] = g_datc.Arrow[7]; // ATC-30
	panel[38] = g_datc.Arrow[8]; // ATC-35
	panel[39] = g_datc.Arrow[9]; // ATC-40
	panel[40] = g_datc.Arrow[10]; // ATC-45
	panel[41] = g_datc.Arrow[11]; // ATC-50
	panel[42] = g_datc.Arrow[12]; // ATC-55
	panel[43] = g_datc.Arrow[13]; // ATC-60
	panel[44] = g_datc.Arrow[14]; // ATC-65
	panel[45] = g_datc.Arrow[15]; // ATC-70
	panel[46] = g_datc.Arrow[16]; // ATC-75
	panel[47] = g_datc.Arrow[17]; // ATC-80
	panel[48] = g_datc.Arrow[18]; // ATC-85
	panel[49] = g_datc.Arrow[19]; // ATC-90
	panel[50] = g_datc.Arrow[20]; // ATC-95
	panel[51] = g_datc.Arrow[21]; // ATC-100
	panel[52] = g_datc.Arrow[22]; // ATC-105
	panel[53] = g_datc.Arrow[23]; // ATC-110
	panel[54] = g_datc.Arrow[24]; // ATC-115
	panel[55] = g_datc.Arrow[25]; // ATC-120
	panel[56] = g_datc.Arrow[26]; // ATC-125
	panel[57] = g_datc.Arrow[27]; // ATC-130
	panel[58] = g_datc.SigFall; // �O���\��
	panel[59] = g_datc.IsOrp; // ORP

	// TIMS���u
	panel[60] = g_tims.AC; // ��
	panel[61] = g_tims.DC; // ����
	panel[62] = g_tims.CVacc; // ����d���ُ�
	panel[63] = g_tims.CVacc10; // ����d��(10��)
	panel[64] = g_tims.CVacc1; // ����d��(1��)
	panel[65] = g_tims.ACacc; // �𗬓d���ُ�
	panel[66] = g_tims.ACacc10000; // �𗬓d��(10000��)
	panel[67] = g_tims.ACacc1000; // �𗬓d��(1000��)
	panel[68] = g_tims.ACacc100; // �𗬓d��(100��)
	panel[69] = g_tims.DCacc; // �����d���ُ�
	panel[70] = g_tims.DCacc1000; // �����d��(1000��)
	panel[71] = g_tims.DCacc100; // �����d��(100��)
	panel[72] = g_tims.DCacc10; // �����d��(10��)
	panel[73] = g_tims.Cvmeter; // ����w�j
	panel[74] = g_tims.Acmeter; // �𗬎w�j
	panel[75] = g_tims.Dcmeter; // �����w�j
	panel[76] = g_tims.Accident; // ����
	panel[77] = g_tims.Tp; // �O��
	panel[78] = g_tims.VCB_ON; // VCB�S��
	panel[79] = g_tims.VCB_OFF; // VCB�S��
	panel[80] = g_tims.VCB; // VCB(DigitalNumber�A0���\���`1���`2��)
	panel[81] = g_tims.alert_ACDC > 0 ? 1 + ((g_time % 800) / 400) : 0; // �𒼐ؑ�(�_��)

	/*
	panel[260] = g_tims.TrackPath2[0]; // �J�ʏ��(0-25m)
	panel[261] = g_tims.TrackPath2[1]; // �J�ʏ��(25-50m)
	panel[262] = g_tims.TrackPath2[2]; // �J�ʏ��(50-75m)
	panel[263] = g_tims.TrackPath2[3]; // �J�ʏ��(75-100m)
	panel[264] = g_tims.TrackPath2[4]; // �J�ʏ��(100-125m)
	panel[265] = g_tims.TrackPath2[5]; // �J�ʏ��(125-150m)
	panel[266] = g_tims.TrackPath2[6]; // �J�ʏ��(150-175m)
	panel[267] = g_tims.TrackPath2[7]; // �J�ʏ��(175-200m)
	panel[268] = g_tims.TrackPath2[8]; // �J�ʏ��(200-225m)
	panel[269] = g_tims.TrackPath2[9]; // �J�ʏ��(225-250m)
	panel[270] = g_tims.TrackPath2[10]; // �J�ʏ��(250-275m)
	panel[271] = g_tims.TrackPath2[11]; // �J�ʏ��(275-300m)
	panel[272] = g_tims.TrackPath2[12]; // �J�ʏ��(300-325m)
	panel[273] = g_tims.TrackPath2[13]; // �J�ʏ��(325-350m)
	panel[274] = g_tims.TrackPath2[14]; // �J�ʏ��(350-375m)
	panel[275] = g_tims.TrackPath2[15]; // �J�ʏ��(375-400m)
	panel[276] = g_tims.TrackPath2[16]; // �J�ʏ��(400-425m)
	panel[277] = g_tims.TrackPath2[17]; // �J�ʏ��(425-450m)
	panel[278] = g_tims.TrackPath2[18]; // �J�ʏ��(450-475m)
	panel[279] = g_tims.TrackPath2[19]; // �J�ʏ��(475-500m)
	panel[280] = g_tims.TrackPath2[20]; // �J�ʏ��(500-525m)
	panel[281] = g_tims.TrackPath2[21]; // �J�ʏ��(525-550m)
	panel[282] = g_tims.TrackPath2[22]; // �J�ʏ��(550-575m)
	panel[283] = g_tims.TrackPath2[23]; // �J�ʏ��(575-600m)
	panel[284] = g_tims.TrackPath2[24]; // �J�ʏ��(600-625m)
	panel[285] = g_tims.TrackPath2[25]; // �J�ʏ��(625-650m)
	panel[286] = g_tims.TrackPath2[26]; // �J�ʏ��(650-675m)
	panel[287] = g_tims.TrackPath2[27]; // �J�ʏ��(675-700m)
	panel[288] = g_tims.TrackPath2[28]; // �J�ʏ��(700-725m)
	panel[289] = g_tims.TrackPath2[29]; // �J�ʏ��(725-750m)
	panel[290] = g_tims.TrackPath2[30]; // �J�ʏ��(750-775m)
	panel[291] = g_tims.TrackPath2[31]; // �J�ʏ��(775-800m)
	panel[292] = g_tims.TrackPath2[32]; // �J�ʏ��(800-825m)
	panel[293] = g_tims.TrackPath2[33]; // �J�ʏ��(825-850m)
	panel[294] = g_tims.TrackPath2[34]; // �J�ʏ��(850-875m)
	panel[295] = g_tims.TrackPath2[35]; // �J�ʏ��(875-900m)
	panel[296] = g_tims.TrackPath2[36]; // �J�ʏ��(900-925m)
	panel[297] = g_tims.TrackPath2[37]; // �J�ʏ��(925-950m)
	panel[298] = g_tims.TrackPath2[38]; // �J�ʏ��(950-975m)
	panel[299] = g_tims.TrackPath2[39]; // �J�ʏ��(975-1000m)
	*/
	panel[100] = g_tims.Kind; // ��Ԏ��
	panel[101] = g_tims.Number[0]; // ��Ԕԍ�(��̌�)
	panel[102] = g_tims.Number[1]; // ��Ԕԍ�(�S�̌�)
	panel[103] = g_tims.Number[2]; // ��Ԕԍ�(�\�̌�)
	panel[104] = g_tims.Number[3]; // ��Ԕԍ�(��̌�)
	panel[105] = g_tims.Charactor; // ��Ԕԍ�(�L��)
	panel[106] = g_tims.From; // �w���\��(�n��)
	panel[107] = g_tims.Destination; // �w���\��(�s��)
	panel[108] = g_tims.This; // �w���\��(���w)
	panel[109] = g_tims.Next; // �w���\��(���w)
	panel[110] = g_tims.HiddenLine[3] ? 0 : (g_tims.Next * g_tims.NextBlink); // �w���\��(���w�A�_�ł���)
	panel[111] = g_tims.HiddenLine[3] ? 24 : g_tims.NextTime[0]; // ��������(���w�A��)
	panel[112] = g_tims.HiddenLine[3] ? 60 : g_tims.NextTime[1]; // ��������(���w�A��)
	panel[113] = g_tims.HiddenLine[3] ? 60 : g_tims.NextTime[2]; // ��������(���w�A�b)
	panel[114] = g_tims.Number[3] != 10 ? 1 : 0; // �ݒ芮��
	panel[115] = g_tims.Crawl ? ((g_time % 750) / 375) : 0; // ���s���(�_��)
	panel[116] = g_tims.Crawl ? g_tims.CrawlLimit : 0;// ���s���x
	panel[117] = g_tims.Crawl ? g_tims.CrawlLimit * ((g_time % 750) / 375) : 0;// ���s���x(�_��)
	//	panel[118] = g_tims.HiddenLine[1] ? 0 : g_tims.PassSta[1]; // �ʉ߉w���\��1
	panel[119] = g_tims.AfterNumber[0]; // ���^�p���(��̌�)
	panel[120] = g_tims.AfterNumber[1]; // ���^�p���(�S�̌�)
	panel[121] = g_tims.AfterNumber[2]; // ���^�p���(�\�̌�)
	panel[122] = g_tims.AfterNumber[3]; // ���^�p���(��̌�)
	panel[123] = g_tims.AfterChara; // ���^�p���(�L��)
	//	panel[124] = g_tims.AfterTime[0][0]; // ���s�H����(��)
	//	panel[125] = g_tims.AfterTime[0][1]; // ���s�H����(��)
	//	panel[126] = g_tims.AfterTime[0][2]; // ���s�H����(�b)
	panel[127] = g_tims.AfterTime[1][0]; // ���s�H����(��)
	panel[128] = g_tims.AfterTime[1][1]; // ���s�H����(��)
	panel[129] = g_tims.AfterTime[1][2]; // ���s�H����(�b)
	panel[130] = g_tims.HiddenLine[0] ? 0 : g_tims.Station[0]; // �w���\��1
	panel[131] = g_tims.HiddenLine[1] ? 0 : g_tims.Station[1]; // �w���\��2
	panel[132] = g_tims.HiddenLine[2] ? 0 : g_tims.Station[2]; // �w���\��3
	panel[133] = g_tims.HiddenLine[3] ? 0 : g_tims.Station[3]; // �w���\��4
	panel[134] = g_tims.HiddenLine[4] ? 0 : g_tims.Station[4]; // �w���\��5
	panel[135] = g_tims.HiddenLine[5] ? 0 : g_tims.Station[5]; // �w���\��6
	panel[136] = g_tims.HiddenLine[6] ? 0 : g_tims.Station[6]; // �w���\��7
	panel[137] = g_tims.LastStopTime[0]; // �I�_��������(��)
	panel[138] = g_tims.LastStopTime[1]; // �I�_��������(��)
	panel[139] = g_tims.LastStopTime[2]; // �I�_��������(�b)
	panel[140] = g_tims.HiddenLine[0] ? 24 : g_tims.Arrive[0][0]; // ��������1H
	panel[141] = g_tims.HiddenLine[0] ? 60 : g_tims.Arrive[0][1]; // ��������1M
	panel[142] = g_tims.HiddenLine[0] ? 12 : g_tims.Arrive[0][2]; // ��������1S
	panel[143] = g_tims.HiddenLine[1] ? 24 : g_tims.Arrive[1][0]; // ��������2H
	panel[144] = g_tims.HiddenLine[1] ? 60 : g_tims.Arrive[1][1]; // ��������2M
	panel[145] = g_tims.HiddenLine[1] ? 12 : g_tims.Arrive[1][2]; // ��������2S
	panel[146] = g_tims.HiddenLine[2] ? 24 : g_tims.Arrive[2][0]; // ��������3H
	panel[147] = g_tims.HiddenLine[2] ? 60 : g_tims.Arrive[2][1]; // ��������3M
	panel[148] = g_tims.HiddenLine[2] ? 12 : g_tims.Arrive[2][2]; // ��������3S
	panel[149] = g_tims.HiddenLine[3] ? 24 : g_tims.Arrive[3][0]; // ��������4H
	panel[150] = g_tims.HiddenLine[3] ? 60 : g_tims.Arrive[3][1]; // ��������4M
	panel[151] = g_tims.HiddenLine[3] ? 12 : g_tims.Arrive[3][2]; // ��������4S
	panel[152] = g_tims.HiddenLine[4] ? 24 : g_tims.Arrive[4][0]; // ��������5H
	panel[153] = g_tims.HiddenLine[4] ? 60 : g_tims.Arrive[4][1]; // ��������5M
	panel[154] = g_tims.HiddenLine[4] ? 12 : g_tims.Arrive[4][2]; // ��������5S
	panel[155] = g_tims.HiddenLine[5] ? 24 : g_tims.Arrive[5][0]; // ��������6H
	panel[156] = g_tims.HiddenLine[5] ? 60 : g_tims.Arrive[5][1]; // ��������6M
	panel[157] = g_tims.HiddenLine[5] ? 12 : g_tims.Arrive[5][2]; // ��������6S
	panel[158] = g_tims.HiddenLine[6] ? 24 : g_tims.Arrive[6][0]; // ��������7H
	panel[159] = g_tims.HiddenLine[6] ? 60 : g_tims.Arrive[6][1]; // ��������7M
	panel[160] = g_tims.HiddenLine[0] ? 24 : g_tims.Leave[0][0]; // �o������1H
	panel[161] = g_tims.HiddenLine[0] ? 60 : g_tims.Leave[0][1]; // �o������1M
	panel[162] = g_tims.HiddenLine[0] ? 12 : g_tims.Leave[0][2]; // �o������1S
	panel[163] = g_tims.HiddenLine[1] ? 24 : g_tims.Leave[1][0]; // �o������2H
	panel[164] = g_tims.HiddenLine[1] ? 60 : g_tims.Leave[1][1]; // �o������2M
	panel[165] = g_tims.HiddenLine[1] ? 12 : g_tims.Leave[1][2]; // �o������2S
	panel[166] = g_tims.HiddenLine[2] ? 24 : g_tims.Leave[2][0]; // �o������3H
	panel[167] = g_tims.HiddenLine[2] ? 60 : g_tims.Leave[2][1]; // �o������3M
	panel[168] = g_tims.HiddenLine[2] ? 12 : g_tims.Leave[2][2]; // �o������3S
	panel[169] = g_tims.HiddenLine[3] ? 24 : g_tims.Leave[3][0]; // �o������4H
	panel[170] = g_tims.HiddenLine[3] ? 60 : g_tims.Leave[3][1]; // �o������4M
	panel[171] = g_tims.HiddenLine[3] ? 12 : g_tims.Leave[3][2]; // �o������4S
	panel[172] = g_tims.HiddenLine[4] ? 24 : g_tims.Leave[4][0]; // �o������5H
	panel[173] = g_tims.HiddenLine[4] ? 60 : g_tims.Leave[4][1]; // �o������5M
	panel[174] = g_tims.HiddenLine[4] ? 12 : g_tims.Leave[4][2]; // �o������5S
	panel[175] = g_tims.HiddenLine[5] ? 24 : g_tims.Leave[5][0]; // �o������6H
	panel[176] = g_tims.HiddenLine[5] ? 60 : g_tims.Leave[5][1]; // �o������6M
	panel[177] = g_tims.HiddenLine[5] ? 12 : g_tims.Leave[5][2]; // �o������6S
	panel[178] = g_tims.HiddenLine[6] ? 24 : g_tims.Leave[6][0]; // �o������7H
	panel[179] = g_tims.HiddenLine[6] ? 60 : g_tims.Leave[6][1]; // �o������7M
	panel[180] = g_tims.HiddenLine[0] ? 0 : g_tims.Span[0][0]; // �w�ԑ��s����12M
	panel[181] = g_tims.HiddenLine[0] ? 0 : g_tims.Span[0][1]; // �w�ԑ��s����12S
	panel[182] = g_tims.HiddenLine[1] ? 0 : g_tims.Span[1][0]; // �w�ԑ��s����23M
	panel[183] = g_tims.HiddenLine[1] ? 0 : g_tims.Span[1][1]; // �w�ԑ��s����23S
	panel[184] = g_tims.HiddenLine[2] ? 0 : g_tims.Span[2][0]; // �w�ԑ��s����34H
	panel[185] = g_tims.HiddenLine[2] ? 0 : g_tims.Span[2][1]; // �w�ԑ��s����34S
	panel[186] = g_tims.HiddenLine[3] ? 0 : g_tims.Span[3][0]; // �w�ԑ��s����45H
	panel[187] = g_tims.HiddenLine[3] ? 0 : g_tims.Span[3][1]; // �w�ԑ��s����45S
	panel[188] = (g_speed != 0) && (g_accel.accel_kmhs > 2.2F) && (vehicleState.Current > 0) ? 1 + ((g_time % 1200) / 600) : 0; // ��](�_��)
	panel[189] = (g_speed != 0) && (g_accel.accel_kmhs < -5.2F) ? 1 + ((g_time % 1200) / 600) : 0; // ����(�_��)
	panel[190] = g_tims.HiddenLine[0] ? 0 : g_tims.Track[0]; // ���w�����Ԑ�1
	panel[191] = g_tims.HiddenLine[1] ? 0 : g_tims.Track[1]; // ���w�����Ԑ�2
	panel[192] = g_tims.HiddenLine[2] ? 0 : g_tims.Track[2]; // ���w�����Ԑ�3
	panel[193] = g_tims.HiddenLine[3] ? 0 : g_tims.Track[3]; // ���w�����Ԑ�4
	panel[194] = g_tims.HiddenLine[4] ? 0 : g_tims.Track[4]; // ���w�����Ԑ�5
	panel[195] = g_tims.HiddenLine[5] ? 0 : g_tims.Track[5]; // ���w�����Ԑ�6
	panel[196] = g_tims.HiddenLine[6] ? 0 : g_tims.Track[6]; // ���w�����Ԑ�7
	panel[197] = g_tims.LastStopTrack; // �I�_�����Ԑ�
	panel[198] = g_tims.RelayName; // �s�H�I�_�w��
	panel[199] = g_tims.TrainArrow; // �s�H�\���
	panel[200] = g_tims.HiddenLine[0] ? 0 : g_tims.Limit[0]; // �������x1
	panel[201] = g_tims.HiddenLine[1] ? 0 : g_tims.Limit[1]; // �������x2
	panel[202] = g_tims.HiddenLine[2] ? 0 : g_tims.Limit[2]; // �������x3
	panel[203] = g_tims.HiddenLine[3] ? 0 : g_tims.Limit[3]; // �������x4
	panel[204] = g_tims.HiddenLine[4] ? 0 : g_tims.Limit[4]; // �������x5
	panel[205] = g_tims.ArrowDirection; // �i�s�������
	//	panel[205] = g_tims.HiddenLine[5] ? 0 : g_tims.Limit[5]; // �������x6
	panel[206] = g_sub.SpeedMeter; // ���x�v([0-5km/h]�Ŕ�������)
	panel[207] = g_tims.Location1000; // ��Ԉʒu(km�̌�)
	panel[208] = g_tims.Location100; // ��Ԉʒu(100m�̌�)
	panel[209] = g_sub.Key; // �}�X�R���L�[
	panel[210] = g_tims.Distance1000; // ���s����(km�̌�)
	panel[211] = g_tims.Distance100; // ���s����(100m�̌�)
	panel[212] = g_date.Cooler; // ��[���
	panel[213] = g_tims.UnitTims[0]; // TIMS���j�b�g�\��1
	panel[214] = g_tims.UnitTims[1]; // TIMS���j�b�g�\��2
	panel[215] = g_tims.UnitTims[2]; // TIMS���j�b�g�\��3
	//	panel[216] = g_tims.UnitTims[3]; // TIMS���j�b�g�\��4
	panel[216] = g_tims.UnitState[0]; // ���j�b�g�\����1
	panel[217] = g_tims.UnitState[1]; // ���j�b�g�\����2
	panel[218] = g_tims.UnitState[2]; // ���j�b�g�\����3
	panel[219] = g_tims.UnitState[3]; // ���j�b�g�\����4
	panel[220] = g_tims.TrackPath[0]; // �J�ʕ\��(0m-100m)
	panel[221] = g_tims.TrackPath[1]; // �J�ʕ\��(100m-200m)
	panel[222] = g_tims.TrackPath[2]; // �J�ʕ\��(200m-300m)
	panel[223] = g_tims.TrackPath[3]; // �J�ʕ\��(300m-400m)
	panel[224] = g_tims.TrackPath[4]; // �J�ʕ\��(400m-500m)
	panel[225] = g_tims.TrackPath[5]; // �J�ʕ\��(500m-600m)
	panel[226] = g_tims.TrackPath[6]; // �J�ʕ\��(600m-700m)
	panel[227] = g_tims.TrackPath[7]; // �J�ʕ\��(700m-800m)
	panel[228] = g_tims.BrakeDelay; // �u���[�L�w��
	panel[229] = g_tims.BcCaution ? ((g_time % 1000) / 500) : 0; // 200kPa�x��
	panel[230] = g_tims.Speed; // ���x�v
	panel[231] = g_tims.Speed100; // ���x(100km/h�̌�)
	panel[232] = g_tims.Speed10; // ���x(10km/h�̌�)
	panel[233] = g_tims.Speed1; // ���x(1km/h�̌�)
	panel[234] = g_tims.Ammeter; // �d���v[A]
	panel[235] = g_tims.BcPress0; // �u���[�L�V�����_�w�j(0-180kPa)
	panel[236] = g_tims.BcPress1; // �u���[�L�V�����_�w�j(200-380kPa)
	panel[237] = g_tims.BcPress2; // �u���[�L�V�����_�w�j(400-580kPa)
	panel[238] = g_tims.BcPress3; // �u���[�L�V�����_�w�j(600-780kPa)
	panel[239] = g_tims.TrackPath[8]; // �J�ʕ\��(800m-900m)
	panel[240] = g_tims.MrPress0; // ����C�_���w�j(700-790kPa)
	panel[241] = g_tims.MrPress1; // ����C�_���w�j(800-890kPa)
	panel[242] = g_tims.MrPress2; // ����C�_���w�j(900-990kPa)
	panel[243] = g_tims.TrackPath[9]; // �J�ʕ\��(900m-1000m)
	panel[244] = g_tims.TimsSpeed100; // TIMS���x�v(100km/h�̌�)
	panel[245] = g_tims.TimsSpeed10; // TIMS���x�v(10km/h�̌�)
	panel[246] = g_tims.TimsSpeed1; // TIMS���x�v(1km/h�̌�)
	//	panel[247] = g_tims.HiddenLine[2] ? 0 : g_tims.PassSta[2]; // �ʉ߉w���\��3
	//	panel[248] = g_tims.HiddenLine[3] ? 0 : g_tims.PassSta[3]; // �ʉ߉w���\��4
	//	panel[249] = g_tims.HiddenLine[4] ? 0 : g_tims.PassSta[4]; // �ʉ߉w���\��5
	//	panel[250] = g_tims.HiddenLine[5] ? 0 : g_tims.PassSta[5]; // �ʉ߉w���\��6
	//	panel[251] = g_tims.HiddenLine[6] ? 0 : g_tims.PassSta[6]; // �ʉ߉w���\��7
	panel[252] = g_tims.HiddenLine[6] ? 12 : g_tims.Arrive[6][2]; // ��������7S
	panel[253] = g_tims.HiddenLine[6] ? 12 : g_tims.Leave[6][2]; // �o������7S
	//	panel[254] = g_tims.HiddenLine[5] ? 0 : g_tims.Span[5][0]; // �w�ԑ��s����67M
	//	panel[255] = g_tims.HiddenLine[5] ? 0 : g_tims.Span[5][1]; // �w�ԑ��s����67S

		// �T�E���h�o��
	sound[0] = max(g_atssn.Bell, g_eb.Bell);
	sound[1] = g_atssn.Chime;
	sound[2] = g_atsp.Bell;
	sound[3] = g_atsps.PatternBegin;
	sound[4] = g_atsps.PatternEnd;
	sound[5] = g_atsps.Chime;
	sound[6] = g_spp.HaltChime;
	sound[7] = g_spp.PassAlarm;
	sound[7] = g_datc.AtcDing;
	sound[8] = g_datc.VoiceAts;
	sound[9] = g_datc.VoiceAtc;
	// sound[10] // ATS�ؑ֗v��
	// sound[11] // ATC�ؑ֗v��
	sound[13] = g_eb.Buzzer;
	sound[14] = g_spp.HaltChime;
	sound[15] = g_spp.PassAlarm;
	sound[16] = g_spp.HaltChime2;
	sound[18] = g_sub.KeyOn;
	sound[19] = g_sub.KeyOff;
	sound[20] = g_eb.EbSwitch;
	sound[25] = g_tims.RelayD;
	sound[26] = g_tims.LbInit;
	sound[100] = g_tims.AirHigh;
	sound[101] = g_tims.AirApply;
	sound[102] = g_tims.EmgAnnounce;
	sound[103] = g_tims.AirApplyEmg;
	sound[105] = g_tims.UpdateInfo;
	sound[106] = (sound[106] == ATS_SOUND_PLAYLOOPING) ? ATS_SOUND_CONTINUE : (g_date.Cooler == 1 ? ATS_SOUND_PLAYLOOPING : ATS_SOUND_STOP);

	return g_output;
}

ATS_API void WINAPI SetPower(int notch)
{
	g_powerNotch = notch;
	g_eb.ResetPhase1();
	g_tims.RandomizeSeed();
}

ATS_API void WINAPI SetBrake(int notch)
{
	g_tims.PlaySoundAirHigh(g_brakeNotch, notch);
	g_tims.PlaySoundEmgAnnounce(g_brakeNotch, notch);
	g_tims.PlaySoundAirApplyEmg(g_brakeNotch, notch);

	g_brakeNotch = notch;
	g_atssn.CheckAts();
	g_eb.ResetPhase1();
	g_eb.ResetPhase2(notch);
	g_spp.NotchChanged();
	g_tims.RandomizeSeed();
}

ATS_API void WINAPI SetReverser(int pos)
{
	g_reverser = pos;
	g_tims.SetLbInit(pos, g_ini.Emulate.lbInit);
}

ATS_API void WINAPI KeyDown(int atsKeyCode)
{
	switch (atsKeyCode)
	{
	case ATS_KEY_S: // ATS �m�F
		g_atssn.DownButtom();
		break;
	case ATS_KEY_A1: // �x�񎝑�
		g_atssn.CancelChime();
		break;
	case ATS_KEY_A2: // EB���Z�b�g
		g_eb.ResetPhase1(1);
		break;
	case ATS_KEY_B1: // ���A
		g_atssn.Reset();
		g_atsp.Reset();
		break;
	case ATS_KEY_B2: // ���^�]
		g_datc.EmgDrive();
		break;
	case ATS_KEY_I: // �}�X�R���L�[����
		g_sub.KeyEvent(0);
		break;
	case ATS_KEY_J: // �}�X�R���L�[����
		g_sub.KeyEvent(1);
		break;
	}
}

ATS_API void WINAPI KeyUp(int atsKeyCode)
{
	if (atsKeyCode == ATS_KEY_S) { g_atssn.UpButton(); }
}

ATS_API void WINAPI HornBlow(int hornType)
{
	g_eb.ResetPhase1();
}

ATS_API void WINAPI DoorOpen()
{
	g_pilotlamp = false;
	g_spp.StopChime(); // ��ʂ̃`���C�����~�߂�
	g_tims.DoorOpening();
}

ATS_API void WINAPI DoorClose()
{
	g_pilotlamp = true;
	g_tims.DoorClosing();
}

ATS_API void WINAPI SetSignal(int signal)
{
	g_datc.ChangedSignal(signal);
	g_tims.CheckPath(signal);
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData) {

	switch (beaconData.Type)
	{
	case ATS_BEACON_S: // S�����O
		g_atsp.Exit();
		g_datc.Exit();
		if (g_speed != 0) { g_atssn.PassedLong(beaconData.Signal); } // �w�W�����v�����O����
		break;
	case ATS_BEACON_SN: // SN����
		g_atsp.Exit();
		g_datc.Exit();
		g_atssn.PassedShort(beaconData.Signal);
		break;
	case ATS_BEACON_P: // ��~�M��
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSig(beaconData.Distance);
		break;
	case ATS_BEACON_EMG: // ����(���)
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedStopEmg(beaconData.Distance);
		break;
	case ATS_BEACON_SVC: // ����(��p)
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedStopSvc(beaconData.Distance);
		break;
	case ATS_BEACON_SPDLIM: // ����푬�x����
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSpeedLim(beaconData.Optional % 1000, beaconData.Optional / 1000);
		break;
	case ATS_BEACON_SPDMAX: // �ō����x
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSpeedMax(beaconData.Optional);
		break;
	case ATS_BEACON_SPP: // ��ԉw�ʉߖh�~���u
		if (g_speed != 0) { g_spp.Receive(beaconData.Optional % 10000); } // �w�W�����v�����O����
		break;
	case ATS_BEACON_SX: // Sx��_�Ԓʉߎ��ԑ���
		g_atsp.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_DATC: // D-ATC�X�V
		// 130715 ����
		g_datc.PassedAtcFall(beaconData.Signal, beaconData.Distance);
		break;
	case ATS_BEACON_PS1: // Ps���p�^�[������
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PS2: // Ps���p�^�[������
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSC: // Ps���E���p�^�[������
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSM: // Ps�p�^�[����ʔF��
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSP: // Ps����푬�x����
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSR: // Ps�Ȑ����x����
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSL: // Ps������z���x����
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PST: // Ps�Վ����x����
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSY: // Ps�����p�^�[������
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSZ: // Ps�U���p�^�[������
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_APP: // TIMS���w�ڋ�
		if (g_speed != 0) { g_spp.Receive(beaconData.Optional % 10000); } // �w�W�����v�����O����
		g_tims.Receive(beaconData.Optional); // �w�W�����v�����O���Ȃ�
		break;
	case ATS_BEACON_UPD: // TIMS�X�V�錾
		g_tims.UpdateView();
		break;
	case ATS_BEACON_NEXT: // TIMS���w�ݒ�
		g_tims.SetNext(beaconData.Optional);
		break;
	case ATS_BEACON_PATH: // TIMS�J�ʏ��
		g_tims.CheckPath(beaconData.Signal, beaconData.Distance, beaconData.Optional);
		break;
	case ATS_BEACON_CRWL: // TIMS���s���
		g_tims.SetCrawl(beaconData.Optional);
		break;
	case ATS_BEACON_STA: // TIMS�w���\��
		g_tims.InputLine(1, (beaconData.Optional / 100) - 1, beaconData.Optional % 100);
		break;
	case ATS_BEACON_ARV: // TIMS��������
		g_tims.InputLine(2, (beaconData.Optional / 1000000) - 1, beaconData.Optional % 1000000);
		break;
	case ATS_BEACON_LEV: // TIMS�o������
		g_tims.InputLine(3, (beaconData.Optional / 1000000) - 1, beaconData.Optional % 1000000);
		break;
	case ATS_BEACON_TRL: // TIMS�����Ԑ���������x
		g_tims.InputLine(4, (beaconData.Optional / 10000) - 1, ((beaconData.Optional % 10000) - (beaconData.Optional % 100)) / 100); // �����Ԑ�
		g_tims.InputLine(5, (beaconData.Optional / 10000) - 1, beaconData.Optional % 100); // �������x
		break;
	case ATS_BEACON_SPN: // TIMS�w�Ԏ���
		g_tims.InputLine(0, (beaconData.Optional / 10000) - 1, beaconData.Optional % 10000);
		break;
	case ATS_BEACON_KIND: // TIMS��Ԏ��
		g_tims.SetKind(beaconData.Optional);
		break;
	case ATS_BEACON_NUM: // TIMS��Ԕԍ�
		g_tims.SetNumber(beaconData.Optional);
		break;
	case ATS_BEACON_LEG: // TIMS�^�s���
		g_tims.SetLeg(beaconData.Optional);
		break;
	case ATS_BEACON_DIST: // TIMS�������Z�ݒ�
		g_tims.SetDistance(beaconData.Optional);
		break;
	case ATS_BEACON_DISDEF: // TIMS��Ԉʒu�␳
		g_tims.SetPositionDef(beaconData.Optional);
		break;
	case ATS_BEACON_SUBN: // TIMS���^�p���
		g_tims.SetAfteruent(0, beaconData.Optional);
		break;
	case ATS_BEACON_SUBA: // TIMS���^�p����
		g_tims.SetAfteruent(1, beaconData.Optional);
		break;
	case ATS_BEACON_SUBL: // TIMS���^�p����
		g_tims.SetAfteruent(2, beaconData.Optional);
		break;
	case ATS_BEACON_DIR: // TIMS�i�s�����ݒ�
		g_tims.SetDirection(beaconData.Optional);
		break;
		/*
		case ATS_BEACON_FSA: // TIMS�n�����Ԏ���
			break;
		case ATS_BEACON_FST: // TIMS�n�����ԔԐ�
			break;
		*/
	case ATS_BEACON_LSA: // TIMS�I�_��������
		g_tims.SetLastStop(0, beaconData.Optional);
		break;
	case ATS_BEACON_LST: // TIMS�I�_�����Ԑ�
		g_tims.SetLastStop(1, beaconData.Optional);
		break;
	case ATS_BEACON_RST: // TIMS�s�H�I�_�w��
		g_tims.SetRelayStation(beaconData.Optional);
		break;
	case ATS_BEACON_ARW: // TIMS�s�H�\���
		g_tims.SetArrowState(beaconData.Optional);
		break;
	case 201: // �ؑւ��m�点
		g_tims.AlartACDC(beaconData.Optional);
		break;
	case 200: // �𒼐ؑ�
		g_tims.SetACDC(beaconData.Optional);
		break;
	}
}
