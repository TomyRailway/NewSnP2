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

	// CFGファイル読み込み
	char filePath[_MAX_PATH + 1] = _T(""); // ファイルパス格納
	char* posIni; // 検索文字列へのポインタ
	bool loadCheck; // INIファイルのロードに成功したかどうか

	// Ats.dllのファイルパスを取得
	::GetModuleFileName((HMODULE)hModule, filePath, _MAX_PATH);
	// パスから.dllの位置を検索
	posIni = strstr(filePath, ".dll");
	// .dllを.cfgに置換
	memmove(posIni, ".cfg", 4);
	// INIファイルをロードして結果を取得
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
	g_tims.ini_timsUpdate = g_ini.Tims.line_update >= 0 && g_ini.Tims.line_update < 250 ? g_ini.Tims.line_update : 90; // 0msから250msまで
	g_tims.ini_cutPressTime = g_ini.Emulate.ebCutPressure; // ブレーキ圧を減圧する時間
	g_tims.ini_lbInit = g_ini.Emulate.lbInit; // 初回起動時力行遅延
	g_sub.ini_keyEvent = g_ini.Emulate.keyEvent; // マスコンキー
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

	g_tims.Location = vehicleState.Location; // 列車位置[m]
	g_tims.BcPressure = vehicleState.BcPressure; // ブレーキシリンダ圧力[kPa]
	g_tims.MrPressure = vehicleState.MrPressure; // 元空気ダメ圧力[kPa]
	g_tims.Current = vehicleState.Current; // 電流[A]

	g_atssn.RunSn(); // ATS-SN
	g_atsp.RunP(); // ATS-P
	g_atsps.execute(); // ATS-Ps
	g_datc.execute(); // D-ATC
	g_spp.RunSpp(); // 停車駅通過防止装置
	g_eb.execute(); // EB装置
	g_tims.execute(); // TIMS装置
	g_date.update(); // 日時機能
	g_sub.execute(); // その他の機能
	g_accel.execute(); // 加速度

	// ハンドル出力
	if (g_atssn.AtsBrake || g_atsp.EmergencyBrake || g_datc.AtcBrake == 2 || g_eb.Emergency || g_sub.Key == 0)
	{
		g_output.Brake = g_emgBrake;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;

		g_powerNotch = 0; // 手放し運転防止
	}
	else if (g_brakeNotch != g_emgBrake && (g_atsp.ServiceBrake || g_datc.AtcBrake == 1))
	{
		g_output.Brake = g_svcBrake;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;

		g_powerNotch = 0; // 手放し運転防止
	}
	else if (g_tims.BcPressCut == 1)
	{
		g_output.Brake = 1; // EB緩解時にブレーキを減圧する
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
	if (g_time > g_tims.AccelCutting && g_tims.VCB_ON == 1) // 力行遅延とVCB
	{
		g_output.Power = g_powerNotch;
	}
	else
	{
		g_output.Power = 0;
	}
	// g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	for (int i = 0; i < 1024; i++) { panel[i] = 0; }

	// パネル出力
	panel[0] = !g_eb.Emergency ? g_atssn.WhiteLamp : (!g_atsp.AtspLamp && g_eb.RedLamp); // ATS白色灯
	panel[1] = g_atssn.RedLamp || g_eb.RedLamp; // ATS赤色灯
	panel[2] = 1; // P電源
	panel[3] = g_atsp.ApproachLamp; // パターン接近
	panel[4] = 0; // ブレーキ開放
	panel[5] = g_atsp.BrakeApplyLamp; // ブレーキ動作
	panel[6] = g_atsp.AtspLamp; // ATS-P
	panel[7] = 0; // 故障
	panel[8] = g_eb.Action; // EB作動
	panel[10] = g_time / 3600000; // デジタル時
	panel[11] = g_time / 60000 % 60; // デジタル分
	panel[12] = g_time / 1000 % 60; // デジタル秒

	// ATS-Ps
	panel[9] = g_atsps.AtsPs; // Ps電源 (主系・従系)
	panel[13] = g_atsps.PatternStart; // パターン発生
	panel[14] = g_atsps.PatternApproach; // パターン接近
	panel[15] = g_atsps.BrakeTurnOn; // ブレーキ動作
	panel[16] = g_atsps.BrakeTurnOff; // ブレーキ開放
	panel[17] = g_atsps.TrainSpeedLED; // 列車速度LED
	panel[18] = g_atsps.PatternSpeedLED; // パターン速度LED
	panel[19] = g_atsps.BrakeDown; // Ps故障

	// D-ATC
	panel[20] = g_datc.DAtc; // D-ATC電源
	panel[21] = g_datc.IsDAtc; // D-ATC
	panel[22] = 0; // 入換
	panel[23] = g_datc.AtcApproach; // パターン接近
	panel[24] = g_datc.AtcEmg; // ATC非常
	panel[25] = g_datc.AtcSvc; // ATC常用
	panel[26] = g_tims.SlideArrow; // 130711実験(フレーム制限) g_datc.SlideArrow; // ATCスライド速度
	panel[27] = g_datc.OrpNeedle;// ORP速度
	panel[28] = g_datc.Signal[0]; // 赤現示
	panel[29] = g_datc.Signal[1]; // 青現示
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
	panel[58] = g_datc.SigFall; // 前方予告
	panel[59] = g_datc.IsOrp; // ORP

	// TIMS装置
	panel[60] = g_tims.AC; // 交流
	panel[61] = g_tims.DC; // 直流
	panel[62] = g_tims.CVacc; // 制御電圧異常
	panel[63] = g_tims.CVacc10; // 制御電圧(10位)
	panel[64] = g_tims.CVacc1; // 制御電圧(1位)
	panel[65] = g_tims.ACacc; // 交流電圧異常
	panel[66] = g_tims.ACacc10000; // 交流電圧(10000位)
	panel[67] = g_tims.ACacc1000; // 交流電圧(1000位)
	panel[68] = g_tims.ACacc100; // 交流電圧(100位)
	panel[69] = g_tims.DCacc; // 直流電圧異常
	panel[70] = g_tims.DCacc1000; // 直流電圧(1000位)
	panel[71] = g_tims.DCacc100; // 直流電圧(100位)
	panel[72] = g_tims.DCacc10; // 直流電圧(10位)
	panel[73] = g_tims.Cvmeter; // 制御指針
	panel[74] = g_tims.Acmeter; // 交流指針
	panel[75] = g_tims.Dcmeter; // 直流指針
	panel[76] = g_tims.Accident; // 事故
	panel[77] = g_tims.Tp; // 三相
	panel[78] = g_tims.VCB_ON; // VCB全入
	panel[79] = g_tims.VCB_OFF; // VCB全切
	panel[80] = g_tims.VCB; // VCB(DigitalNumber、0無表示～1入～2切)
	panel[81] = g_tims.alert_ACDC > 0 ? 1 + ((g_time % 800) / 400) : 0; // 交直切替(点滅)

	/*
	panel[260] = g_tims.TrackPath2[0]; // 開通情報(0-25m)
	panel[261] = g_tims.TrackPath2[1]; // 開通情報(25-50m)
	panel[262] = g_tims.TrackPath2[2]; // 開通情報(50-75m)
	panel[263] = g_tims.TrackPath2[3]; // 開通情報(75-100m)
	panel[264] = g_tims.TrackPath2[4]; // 開通情報(100-125m)
	panel[265] = g_tims.TrackPath2[5]; // 開通情報(125-150m)
	panel[266] = g_tims.TrackPath2[6]; // 開通情報(150-175m)
	panel[267] = g_tims.TrackPath2[7]; // 開通情報(175-200m)
	panel[268] = g_tims.TrackPath2[8]; // 開通情報(200-225m)
	panel[269] = g_tims.TrackPath2[9]; // 開通情報(225-250m)
	panel[270] = g_tims.TrackPath2[10]; // 開通情報(250-275m)
	panel[271] = g_tims.TrackPath2[11]; // 開通情報(275-300m)
	panel[272] = g_tims.TrackPath2[12]; // 開通情報(300-325m)
	panel[273] = g_tims.TrackPath2[13]; // 開通情報(325-350m)
	panel[274] = g_tims.TrackPath2[14]; // 開通情報(350-375m)
	panel[275] = g_tims.TrackPath2[15]; // 開通情報(375-400m)
	panel[276] = g_tims.TrackPath2[16]; // 開通情報(400-425m)
	panel[277] = g_tims.TrackPath2[17]; // 開通情報(425-450m)
	panel[278] = g_tims.TrackPath2[18]; // 開通情報(450-475m)
	panel[279] = g_tims.TrackPath2[19]; // 開通情報(475-500m)
	panel[280] = g_tims.TrackPath2[20]; // 開通情報(500-525m)
	panel[281] = g_tims.TrackPath2[21]; // 開通情報(525-550m)
	panel[282] = g_tims.TrackPath2[22]; // 開通情報(550-575m)
	panel[283] = g_tims.TrackPath2[23]; // 開通情報(575-600m)
	panel[284] = g_tims.TrackPath2[24]; // 開通情報(600-625m)
	panel[285] = g_tims.TrackPath2[25]; // 開通情報(625-650m)
	panel[286] = g_tims.TrackPath2[26]; // 開通情報(650-675m)
	panel[287] = g_tims.TrackPath2[27]; // 開通情報(675-700m)
	panel[288] = g_tims.TrackPath2[28]; // 開通情報(700-725m)
	panel[289] = g_tims.TrackPath2[29]; // 開通情報(725-750m)
	panel[290] = g_tims.TrackPath2[30]; // 開通情報(750-775m)
	panel[291] = g_tims.TrackPath2[31]; // 開通情報(775-800m)
	panel[292] = g_tims.TrackPath2[32]; // 開通情報(800-825m)
	panel[293] = g_tims.TrackPath2[33]; // 開通情報(825-850m)
	panel[294] = g_tims.TrackPath2[34]; // 開通情報(850-875m)
	panel[295] = g_tims.TrackPath2[35]; // 開通情報(875-900m)
	panel[296] = g_tims.TrackPath2[36]; // 開通情報(900-925m)
	panel[297] = g_tims.TrackPath2[37]; // 開通情報(925-950m)
	panel[298] = g_tims.TrackPath2[38]; // 開通情報(950-975m)
	panel[299] = g_tims.TrackPath2[39]; // 開通情報(975-1000m)
	*/
	panel[100] = g_tims.Kind; // 列車種別
	panel[101] = g_tims.Number[0]; // 列車番号(千の桁)
	panel[102] = g_tims.Number[1]; // 列車番号(百の桁)
	panel[103] = g_tims.Number[2]; // 列車番号(十の桁)
	panel[104] = g_tims.Number[3]; // 列車番号(一の桁)
	panel[105] = g_tims.Charactor; // 列車番号(記号)
	panel[106] = g_tims.From; // 駅名表示(始発)
	panel[107] = g_tims.Destination; // 駅名表示(行先)
	panel[108] = g_tims.This; // 駅名表示(自駅)
	panel[109] = g_tims.Next; // 駅名表示(次駅)
	panel[110] = g_tims.HiddenLine[3] ? 0 : (g_tims.Next * g_tims.NextBlink); // 駅名表示(次駅、点滅する)
	panel[111] = g_tims.HiddenLine[3] ? 24 : g_tims.NextTime[0]; // 到着時刻(次駅、時)
	panel[112] = g_tims.HiddenLine[3] ? 60 : g_tims.NextTime[1]; // 到着時刻(次駅、分)
	panel[113] = g_tims.HiddenLine[3] ? 60 : g_tims.NextTime[2]; // 到着時刻(次駅、秒)
	panel[114] = g_tims.Number[3] != 10 ? 1 : 0; // 設定完了
	panel[115] = g_tims.Crawl ? ((g_time % 750) / 375) : 0; // 徐行区間(点滅)
	panel[116] = g_tims.Crawl ? g_tims.CrawlLimit : 0;// 徐行速度
	panel[117] = g_tims.Crawl ? g_tims.CrawlLimit * ((g_time % 750) / 375) : 0;// 徐行速度(点滅)
	//	panel[118] = g_tims.HiddenLine[1] ? 0 : g_tims.PassSta[1]; // 通過駅名表示1
	panel[119] = g_tims.AfterNumber[0]; // 次運用列番(千の桁)
	panel[120] = g_tims.AfterNumber[1]; // 次運用列番(百の桁)
	panel[121] = g_tims.AfterNumber[2]; // 次運用列番(十の桁)
	panel[122] = g_tims.AfterNumber[3]; // 次運用列番(一の桁)
	panel[123] = g_tims.AfterChara; // 次運用列番(記号)
	//	panel[124] = g_tims.AfterTime[0][0]; // 次行路着時(時)
	//	panel[125] = g_tims.AfterTime[0][1]; // 次行路着時(分)
	//	panel[126] = g_tims.AfterTime[0][2]; // 次行路着時(秒)
	panel[127] = g_tims.AfterTime[1][0]; // 次行路発時(時)
	panel[128] = g_tims.AfterTime[1][1]; // 次行路発時(分)
	panel[129] = g_tims.AfterTime[1][2]; // 次行路発時(秒)
	panel[130] = g_tims.HiddenLine[0] ? 0 : g_tims.Station[0]; // 駅名表示1
	panel[131] = g_tims.HiddenLine[1] ? 0 : g_tims.Station[1]; // 駅名表示2
	panel[132] = g_tims.HiddenLine[2] ? 0 : g_tims.Station[2]; // 駅名表示3
	panel[133] = g_tims.HiddenLine[3] ? 0 : g_tims.Station[3]; // 駅名表示4
	panel[134] = g_tims.HiddenLine[4] ? 0 : g_tims.Station[4]; // 駅名表示5
	panel[135] = g_tims.HiddenLine[5] ? 0 : g_tims.Station[5]; // 駅名表示6
	panel[136] = g_tims.HiddenLine[6] ? 0 : g_tims.Station[6]; // 駅名表示7
	panel[137] = g_tims.LastStopTime[0]; // 終点到着時刻(時)
	panel[138] = g_tims.LastStopTime[1]; // 終点到着時刻(分)
	panel[139] = g_tims.LastStopTime[2]; // 終点到着時刻(秒)
	panel[140] = g_tims.HiddenLine[0] ? 24 : g_tims.Arrive[0][0]; // 到着時刻1H
	panel[141] = g_tims.HiddenLine[0] ? 60 : g_tims.Arrive[0][1]; // 到着時刻1M
	panel[142] = g_tims.HiddenLine[0] ? 12 : g_tims.Arrive[0][2]; // 到着時刻1S
	panel[143] = g_tims.HiddenLine[1] ? 24 : g_tims.Arrive[1][0]; // 到着時刻2H
	panel[144] = g_tims.HiddenLine[1] ? 60 : g_tims.Arrive[1][1]; // 到着時刻2M
	panel[145] = g_tims.HiddenLine[1] ? 12 : g_tims.Arrive[1][2]; // 到着時刻2S
	panel[146] = g_tims.HiddenLine[2] ? 24 : g_tims.Arrive[2][0]; // 到着時刻3H
	panel[147] = g_tims.HiddenLine[2] ? 60 : g_tims.Arrive[2][1]; // 到着時刻3M
	panel[148] = g_tims.HiddenLine[2] ? 12 : g_tims.Arrive[2][2]; // 到着時刻3S
	panel[149] = g_tims.HiddenLine[3] ? 24 : g_tims.Arrive[3][0]; // 到着時刻4H
	panel[150] = g_tims.HiddenLine[3] ? 60 : g_tims.Arrive[3][1]; // 到着時刻4M
	panel[151] = g_tims.HiddenLine[3] ? 12 : g_tims.Arrive[3][2]; // 到着時刻4S
	panel[152] = g_tims.HiddenLine[4] ? 24 : g_tims.Arrive[4][0]; // 到着時刻5H
	panel[153] = g_tims.HiddenLine[4] ? 60 : g_tims.Arrive[4][1]; // 到着時刻5M
	panel[154] = g_tims.HiddenLine[4] ? 12 : g_tims.Arrive[4][2]; // 到着時刻5S
	panel[155] = g_tims.HiddenLine[5] ? 24 : g_tims.Arrive[5][0]; // 到着時刻6H
	panel[156] = g_tims.HiddenLine[5] ? 60 : g_tims.Arrive[5][1]; // 到着時刻6M
	panel[157] = g_tims.HiddenLine[5] ? 12 : g_tims.Arrive[5][2]; // 到着時刻6S
	panel[158] = g_tims.HiddenLine[6] ? 24 : g_tims.Arrive[6][0]; // 到着時刻7H
	panel[159] = g_tims.HiddenLine[6] ? 60 : g_tims.Arrive[6][1]; // 到着時刻7M
	panel[160] = g_tims.HiddenLine[0] ? 24 : g_tims.Leave[0][0]; // 出発時刻1H
	panel[161] = g_tims.HiddenLine[0] ? 60 : g_tims.Leave[0][1]; // 出発時刻1M
	panel[162] = g_tims.HiddenLine[0] ? 12 : g_tims.Leave[0][2]; // 出発時刻1S
	panel[163] = g_tims.HiddenLine[1] ? 24 : g_tims.Leave[1][0]; // 出発時刻2H
	panel[164] = g_tims.HiddenLine[1] ? 60 : g_tims.Leave[1][1]; // 出発時刻2M
	panel[165] = g_tims.HiddenLine[1] ? 12 : g_tims.Leave[1][2]; // 出発時刻2S
	panel[166] = g_tims.HiddenLine[2] ? 24 : g_tims.Leave[2][0]; // 出発時刻3H
	panel[167] = g_tims.HiddenLine[2] ? 60 : g_tims.Leave[2][1]; // 出発時刻3M
	panel[168] = g_tims.HiddenLine[2] ? 12 : g_tims.Leave[2][2]; // 出発時刻3S
	panel[169] = g_tims.HiddenLine[3] ? 24 : g_tims.Leave[3][0]; // 出発時刻4H
	panel[170] = g_tims.HiddenLine[3] ? 60 : g_tims.Leave[3][1]; // 出発時刻4M
	panel[171] = g_tims.HiddenLine[3] ? 12 : g_tims.Leave[3][2]; // 出発時刻4S
	panel[172] = g_tims.HiddenLine[4] ? 24 : g_tims.Leave[4][0]; // 出発時刻5H
	panel[173] = g_tims.HiddenLine[4] ? 60 : g_tims.Leave[4][1]; // 出発時刻5M
	panel[174] = g_tims.HiddenLine[4] ? 12 : g_tims.Leave[4][2]; // 出発時刻5S
	panel[175] = g_tims.HiddenLine[5] ? 24 : g_tims.Leave[5][0]; // 出発時刻6H
	panel[176] = g_tims.HiddenLine[5] ? 60 : g_tims.Leave[5][1]; // 出発時刻6M
	panel[177] = g_tims.HiddenLine[5] ? 12 : g_tims.Leave[5][2]; // 出発時刻6S
	panel[178] = g_tims.HiddenLine[6] ? 24 : g_tims.Leave[6][0]; // 出発時刻7H
	panel[179] = g_tims.HiddenLine[6] ? 60 : g_tims.Leave[6][1]; // 出発時刻7M
	panel[180] = g_tims.HiddenLine[0] ? 0 : g_tims.Span[0][0]; // 駅間走行時間12M
	panel[181] = g_tims.HiddenLine[0] ? 0 : g_tims.Span[0][1]; // 駅間走行時間12S
	panel[182] = g_tims.HiddenLine[1] ? 0 : g_tims.Span[1][0]; // 駅間走行時間23M
	panel[183] = g_tims.HiddenLine[1] ? 0 : g_tims.Span[1][1]; // 駅間走行時間23S
	panel[184] = g_tims.HiddenLine[2] ? 0 : g_tims.Span[2][0]; // 駅間走行時間34H
	panel[185] = g_tims.HiddenLine[2] ? 0 : g_tims.Span[2][1]; // 駅間走行時間34S
	panel[186] = g_tims.HiddenLine[3] ? 0 : g_tims.Span[3][0]; // 駅間走行時間45H
	panel[187] = g_tims.HiddenLine[3] ? 0 : g_tims.Span[3][1]; // 駅間走行時間45S
	panel[188] = (g_speed != 0) && (g_accel.accel_kmhs > 2.2F) && (vehicleState.Current > 0) ? 1 + ((g_time % 1200) / 600) : 0; // 空転(点滅)
	panel[189] = (g_speed != 0) && (g_accel.accel_kmhs < -5.2F) ? 1 + ((g_time % 1200) / 600) : 0; // 滑走(点滅)
	panel[190] = g_tims.HiddenLine[0] ? 0 : g_tims.Track[0]; // 次駅到着番線1
	panel[191] = g_tims.HiddenLine[1] ? 0 : g_tims.Track[1]; // 次駅到着番線2
	panel[192] = g_tims.HiddenLine[2] ? 0 : g_tims.Track[2]; // 次駅到着番線3
	panel[193] = g_tims.HiddenLine[3] ? 0 : g_tims.Track[3]; // 次駅到着番線4
	panel[194] = g_tims.HiddenLine[4] ? 0 : g_tims.Track[4]; // 次駅到着番線5
	panel[195] = g_tims.HiddenLine[5] ? 0 : g_tims.Track[5]; // 次駅到着番線6
	panel[196] = g_tims.HiddenLine[6] ? 0 : g_tims.Track[6]; // 次駅到着番線7
	panel[197] = g_tims.LastStopTrack; // 終点到着番線
	panel[198] = g_tims.RelayName; // 行路終点駅名
	panel[199] = g_tims.TrainArrow; // 行路表矢印
	panel[200] = g_tims.HiddenLine[0] ? 0 : g_tims.Limit[0]; // 制限速度1
	panel[201] = g_tims.HiddenLine[1] ? 0 : g_tims.Limit[1]; // 制限速度2
	panel[202] = g_tims.HiddenLine[2] ? 0 : g_tims.Limit[2]; // 制限速度3
	panel[203] = g_tims.HiddenLine[3] ? 0 : g_tims.Limit[3]; // 制限速度4
	panel[204] = g_tims.HiddenLine[4] ? 0 : g_tims.Limit[4]; // 制限速度5
	panel[205] = g_tims.ArrowDirection; // 進行方向矢印
	//	panel[205] = g_tims.HiddenLine[5] ? 0 : g_tims.Limit[5]; // 制限速度6
	panel[206] = g_sub.SpeedMeter; // 速度計([0-5km/h]で反応悪い)
	panel[207] = g_tims.Location1000; // 列車位置(kmの桁)
	panel[208] = g_tims.Location100; // 列車位置(100mの桁)
	panel[209] = g_sub.Key; // マスコンキー
	panel[210] = g_tims.Distance1000; // 走行距離(kmの桁)
	panel[211] = g_tims.Distance100; // 走行距離(100mの桁)
	panel[212] = g_date.Cooler; // 冷房状態
	panel[213] = g_tims.UnitTims[0]; // TIMSユニット表示1
	panel[214] = g_tims.UnitTims[1]; // TIMSユニット表示2
	panel[215] = g_tims.UnitTims[2]; // TIMSユニット表示3
	//	panel[216] = g_tims.UnitTims[3]; // TIMSユニット表示4
	panel[216] = g_tims.UnitState[0]; // ユニット表示灯1
	panel[217] = g_tims.UnitState[1]; // ユニット表示灯2
	panel[218] = g_tims.UnitState[2]; // ユニット表示灯3
	panel[219] = g_tims.UnitState[3]; // ユニット表示灯4
	panel[220] = g_tims.TrackPath[0]; // 開通表示(0m-100m)
	panel[221] = g_tims.TrackPath[1]; // 開通表示(100m-200m)
	panel[222] = g_tims.TrackPath[2]; // 開通表示(200m-300m)
	panel[223] = g_tims.TrackPath[3]; // 開通表示(300m-400m)
	panel[224] = g_tims.TrackPath[4]; // 開通表示(400m-500m)
	panel[225] = g_tims.TrackPath[5]; // 開通表示(500m-600m)
	panel[226] = g_tims.TrackPath[6]; // 開通表示(600m-700m)
	panel[227] = g_tims.TrackPath[7]; // 開通表示(700m-800m)
	panel[228] = g_tims.BrakeDelay; // ブレーキ指令
	panel[229] = g_tims.BcCaution ? ((g_time % 1000) / 500) : 0; // 200kPa警告
	panel[230] = g_tims.Speed; // 速度計
	panel[231] = g_tims.Speed100; // 速度(100km/hの桁)
	panel[232] = g_tims.Speed10; // 速度(10km/hの桁)
	panel[233] = g_tims.Speed1; // 速度(1km/hの桁)
	panel[234] = g_tims.Ammeter; // 電流計[A]
	panel[235] = g_tims.BcPress0; // ブレーキシリンダ指針(0-180kPa)
	panel[236] = g_tims.BcPress1; // ブレーキシリンダ指針(200-380kPa)
	panel[237] = g_tims.BcPress2; // ブレーキシリンダ指針(400-580kPa)
	panel[238] = g_tims.BcPress3; // ブレーキシリンダ指針(600-780kPa)
	panel[239] = g_tims.TrackPath[8]; // 開通表示(800m-900m)
	panel[240] = g_tims.MrPress0; // 元空気ダメ指針(700-790kPa)
	panel[241] = g_tims.MrPress1; // 元空気ダメ指針(800-890kPa)
	panel[242] = g_tims.MrPress2; // 元空気ダメ指針(900-990kPa)
	panel[243] = g_tims.TrackPath[9]; // 開通表示(900m-1000m)
	panel[244] = g_tims.TimsSpeed100; // TIMS速度計(100km/hの桁)
	panel[245] = g_tims.TimsSpeed10; // TIMS速度計(10km/hの桁)
	panel[246] = g_tims.TimsSpeed1; // TIMS速度計(1km/hの桁)
	//	panel[247] = g_tims.HiddenLine[2] ? 0 : g_tims.PassSta[2]; // 通過駅名表示3
	//	panel[248] = g_tims.HiddenLine[3] ? 0 : g_tims.PassSta[3]; // 通過駅名表示4
	//	panel[249] = g_tims.HiddenLine[4] ? 0 : g_tims.PassSta[4]; // 通過駅名表示5
	//	panel[250] = g_tims.HiddenLine[5] ? 0 : g_tims.PassSta[5]; // 通過駅名表示6
	//	panel[251] = g_tims.HiddenLine[6] ? 0 : g_tims.PassSta[6]; // 通過駅名表示7
	panel[252] = g_tims.HiddenLine[6] ? 12 : g_tims.Arrive[6][2]; // 到着時刻7S
	panel[253] = g_tims.HiddenLine[6] ? 12 : g_tims.Leave[6][2]; // 出発時刻7S
	//	panel[254] = g_tims.HiddenLine[5] ? 0 : g_tims.Span[5][0]; // 駅間走行時間67M
	//	panel[255] = g_tims.HiddenLine[5] ? 0 : g_tims.Span[5][1]; // 駅間走行時間67S

		// サウンド出力
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
	// sound[10] // ATS切替要請
	// sound[11] // ATC切替要請
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
	case ATS_KEY_S: // ATS 確認
		g_atssn.DownButtom();
		break;
	case ATS_KEY_A1: // 警報持続
		g_atssn.CancelChime();
		break;
	case ATS_KEY_A2: // EBリセット
		g_eb.ResetPhase1(1);
		break;
	case ATS_KEY_B1: // 復帰
		g_atssn.Reset();
		g_atsp.Reset();
		break;
	case ATS_KEY_B2: // 非常運転
		g_datc.EmgDrive();
		break;
	case ATS_KEY_I: // マスコンキー抜取
		g_sub.KeyEvent(0);
		break;
	case ATS_KEY_J: // マスコンキー投入
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
	g_spp.StopChime(); // 停通のチャイムを止める
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
	case ATS_BEACON_S: // Sロング
		g_atsp.Exit();
		g_datc.Exit();
		if (g_speed != 0) { g_atssn.PassedLong(beaconData.Signal); } // 駅ジャンプを除外する
		break;
	case ATS_BEACON_SN: // SN直下
		g_atsp.Exit();
		g_datc.Exit();
		g_atssn.PassedShort(beaconData.Signal);
		break;
	case ATS_BEACON_P: // 停止信号
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSig(beaconData.Distance);
		break;
	case ATS_BEACON_EMG: // 即停(非常)
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedStopEmg(beaconData.Distance);
		break;
	case ATS_BEACON_SVC: // 即停(常用)
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedStopSvc(beaconData.Distance);
		break;
	case ATS_BEACON_SPDLIM: // 分岐器速度制限
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSpeedLim(beaconData.Optional % 1000, beaconData.Optional / 1000);
		break;
	case ATS_BEACON_SPDMAX: // 最高速度
		g_atssn.TurnOff();
		g_datc.Exit();
		g_atsp.PassedSpeedMax(beaconData.Optional);
		break;
	case ATS_BEACON_SPP: // 停車駅通過防止装置
		if (g_speed != 0) { g_spp.Receive(beaconData.Optional % 10000); } // 駅ジャンプを除外する
		break;
	case ATS_BEACON_SX: // Sx二点間通過時間速照
		g_atsp.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_DATC: // D-ATC更新
		// 130715 試験
		g_datc.PassedAtcFall(beaconData.Signal, beaconData.Distance);
		break;
	case ATS_BEACON_PS1: // Ps第一パターン発生
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PS2: // Ps第二パターン発生
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSC: // Ps第一・第二パターン消去
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSM: // Psパターン種別認識
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSP: // Ps分岐器速度制限
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSR: // Ps曲線速度制限
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSL: // Ps下り勾配速度制限
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PST: // Ps臨時速度制限
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSY: // Ps入換パターン発生
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_PSZ: // Ps誘導パターン発生
		g_atsps.Exit();
		g_datc.Exit();
		break;
	case ATS_BEACON_APP: // TIMS次駅接近
		if (g_speed != 0) { g_spp.Receive(beaconData.Optional % 10000); } // 駅ジャンプを除外する
		g_tims.Receive(beaconData.Optional); // 駅ジャンプを除外しない
		break;
	case ATS_BEACON_UPD: // TIMS更新宣言
		g_tims.UpdateView();
		break;
	case ATS_BEACON_NEXT: // TIMS次駅設定
		g_tims.SetNext(beaconData.Optional);
		break;
	case ATS_BEACON_PATH: // TIMS開通情報
		g_tims.CheckPath(beaconData.Signal, beaconData.Distance, beaconData.Optional);
		break;
	case ATS_BEACON_CRWL: // TIMS徐行区間
		g_tims.SetCrawl(beaconData.Optional);
		break;
	case ATS_BEACON_STA: // TIMS駅名表示
		g_tims.InputLine(1, (beaconData.Optional / 100) - 1, beaconData.Optional % 100);
		break;
	case ATS_BEACON_ARV: // TIMS到着時刻
		g_tims.InputLine(2, (beaconData.Optional / 1000000) - 1, beaconData.Optional % 1000000);
		break;
	case ATS_BEACON_LEV: // TIMS出発時刻
		g_tims.InputLine(3, (beaconData.Optional / 1000000) - 1, beaconData.Optional % 1000000);
		break;
	case ATS_BEACON_TRL: // TIMS到着番線･制限速度
		g_tims.InputLine(4, (beaconData.Optional / 10000) - 1, ((beaconData.Optional % 10000) - (beaconData.Optional % 100)) / 100); // 到着番線
		g_tims.InputLine(5, (beaconData.Optional / 10000) - 1, beaconData.Optional % 100); // 制限速度
		break;
	case ATS_BEACON_SPN: // TIMS駅間時間
		g_tims.InputLine(0, (beaconData.Optional / 10000) - 1, beaconData.Optional % 10000);
		break;
	case ATS_BEACON_KIND: // TIMS列車種別
		g_tims.SetKind(beaconData.Optional);
		break;
	case ATS_BEACON_NUM: // TIMS列車番号
		g_tims.SetNumber(beaconData.Optional);
		break;
	case ATS_BEACON_LEG: // TIMS運行区間
		g_tims.SetLeg(beaconData.Optional);
		break;
	case ATS_BEACON_DIST: // TIMS距離加算設定
		g_tims.SetDistance(beaconData.Optional);
		break;
	case ATS_BEACON_DISDEF: // TIMS列車位置補正
		g_tims.SetPositionDef(beaconData.Optional);
		break;
	case ATS_BEACON_SUBN: // TIMS次運用列番
		g_tims.SetAfteruent(0, beaconData.Optional);
		break;
	case ATS_BEACON_SUBA: // TIMS次運用着時
		g_tims.SetAfteruent(1, beaconData.Optional);
		break;
	case ATS_BEACON_SUBL: // TIMS次運用発時
		g_tims.SetAfteruent(2, beaconData.Optional);
		break;
	case ATS_BEACON_DIR: // TIMS進行方向設定
		g_tims.SetDirection(beaconData.Optional);
		break;
		/*
		case ATS_BEACON_FSA: // TIMS始発発車時刻
			break;
		case ATS_BEACON_FST: // TIMS始発発車番線
			break;
		*/
	case ATS_BEACON_LSA: // TIMS終点到着時刻
		g_tims.SetLastStop(0, beaconData.Optional);
		break;
	case ATS_BEACON_LST: // TIMS終点到着番線
		g_tims.SetLastStop(1, beaconData.Optional);
		break;
	case ATS_BEACON_RST: // TIMS行路終点駅名
		g_tims.SetRelayStation(beaconData.Optional);
		break;
	case ATS_BEACON_ARW: // TIMS行路表矢印
		g_tims.SetArrowState(beaconData.Optional);
		break;
	case 201: // 切替お知らせ
		g_tims.AlartACDC(beaconData.Optional);
		break;
	case 200: // 交直切替
		g_tims.SetACDC(beaconData.Optional);
		break;
	}
}
