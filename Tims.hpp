// JR東日本向け TIMS装置 および グラスコックピット装置
// Copyright (C) 2009 Unicorn

#ifndef ATS_TIMS_HPP_INCLUDED
#define ATS_TIMS_HPP_INCLUDED

#define TIMS_DECELERATION 19.5F // 減速定数 (減速度[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define TIMS_OFFSET 0.5F // 車上子オフセット[m]

#define RANDOMIZE_UNITTIMS 30 // TIMSユニット表示の更新確率
#define RANDOMIZE_UNITLAMP 10 // ユニット表示灯の更新確率
#define UPDATE_SPAN 180000 // 運行情報更新の最低間隔
#define RELAY_REQUIREMENT 7.5f // 走行検知リレー動作する速度

class CTims
{
public:
	int EmergencyNotch; // 非常ノッチ
	int *BrakeNotch; // ブレーキノッチ
	int *PowerNotch; // 力行ノッチ
	int *Reverser; // レバーサー
	int *Time; // 現在時刻
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[ms/frame]
	float BcPressure; // ブレーキシリンダ圧力[kPa]
	float MrPressure; // 元空気ダメ圧力[kPa]
	float Current; // 電流[A]
	double Location; // 列車位置[m]

	int Kind; // 列車種別
	int Number[4]; // 列車番号(1000 - 100 - 10 - 1)
	int Charactor; // 列車番号(記号)
	int From; // 駅名表示(始発)
	int Destination; // 駅名表示(行先)
	int This; // 駅名表示(自駅)
	int Next; // 駅名表示(次駅)
	int NextBlink; // 駅名表示の点滅
	int NextTime[3]; // 到着時刻(次駅、時 - 分 - 秒)
	int Crawl; // 徐行区間
	int CrawlLimit; // 徐行速度
	int AfterNumber[4]; // 次運用列車番号(1000 - 100 - 10 - 1)
	int AfterChara; // 次運用記号
	int AfterTime[2][3]; // 次運用着発時刻
	int LastStopTime[3]; // 終点到着時刻(時 - 分 - 秒)
	int LastStopTrack; // 終点到着番線
	int RelayName; // 行路終点駅名
	int TrainArrow; // 行路表矢印
	int Station[10]; // 駅名表示
	int PassSta[10]; // 通過駅名表示
	int Arrive[10][3]; // 到着時刻(時 - 分 - 秒)
	int Leave[10][3]; // 出発時刻(時 - 分 - 秒)
	int Track[10]; // 次駅到着番線
	int Limit[10]; // 駅間最高速度
	int Span[10][2]; // 駅間走行時間
	int HiddenLine[10]; // 更新時に1行ずつ非表示にする
	int TrackPath[10]; // 開通情報(100mごと)
	int TrackPath2[120]; // 開通情報(25mごと)
	int Distance1000; // 走行距離(kmの桁)
	int Distance100; // 走行距離(100mの桁)
	int Distance10; // 走行距離(10mの桁)
	int Location1000; // 列車位置(kmの桁)
	int Location100; // 列車位置(100mの桁)
	int Location10; // 列車位置(10mの桁)
	int TimsSpeed100; // TIMS速度(100の桁)
	int TimsSpeed10; // TIMS速度(10の桁)
	int TimsSpeed1; // TIMS速度(1の桁)
	int UnitTims[10]; // TIMSユニット表示(0-9)
	int UnitState[10]; // ユニット表示灯(0-9)
	int BrakeDelay; // ブレーキ指令
	int BcCaution; // 200kPa警告
	int BcPressCut; // ブレーキ減圧する
	int RelayState; // 走行検知リレー
	int Direction; // 進行方向
	int ArrowDirection; // 進行方向矢印
	
	int Speed; // 速度計
	int Speed100; // 速度(100の桁)
	int Speed10; // 速度(10の桁)
	int Speed1; // 速度(1の桁)
	int BcPress0; // ブレーキシリンダ指針(0-180kPa)
	int BcPress1; // ブレーキシリンダ指針(200-380kPa)
	int BcPress2; // ブレーキシリンダ指針(400-580kPa)
	int BcPress3; // ブレーキシリンダ指針(600-780kPa)
	int MrPress0; // 元空気ダメ指針(700-790kPa)
	int MrPress1; // 元空気ダメ指針(800-890kPa)
	int MrPress2; // 元空気ダメ指針(900-990kPa)
	int Ammeter; // 電流計

	int *c_DAtcSlideArrow; // DAtcクラスのSlideArrowの値
	int SlideArrow; // ATCスライド速度
	int AccelCutting; // 力行遅延(この時刻まで反映しない)

	int RelayD; // 走行検知リレー
	int LbInit; // 初回起動時断流器音
	int AirHigh; // 非常ブレーキ緩解音
	int AirApply; // ブレーキ昇圧音
	int EmgAnnounce; // 非常ブレーキ放送
	int AirApplyEmg; // ブレーキ昇圧音(非常)
	int UpdateInfo; // 運行情報更新

	// 交直切替
	int AC; // 交流
	int DC; // 直流
	int CVacc; // 制御電圧異常
	int CVacc10; // 制御電圧(10位)
	int CVacc1; // 制御電圧(1位)
	int ACacc; // 交流電圧異常
	int ACacc10000; // 交流電圧(10000位)
	int ACacc1000; // 交流電圧(1000位)
	int ACacc100; // 交流電圧(100位)
	int DCacc; // 直流電圧異常
	int DCacc1000; // 直流電圧(1000位)
	int DCacc100; // 直流電圧(100位)
	int DCacc10; // 直流電圧(10位)
	int Cvmeter; // 制御指針
	int Acmeter; // 交流指針
	int Dcmeter; // 直流指針
	int Accident; // 事故
	int Tp; // 三相
	int VCB_ON; // VCB全入
	int VCB_OFF; // VCB全切
	int VCB; // VCB
	int tmr_ACDC;
	int step_ACDC;
	int alert_ACDC;

	// 定速表示
	// TODO

	// INIファイルから設定
	int ini_cutPressTime; // ブレーキ圧を減圧する時間
	int ini_lbInit; // 初回起動時力行遅延
	int ini_timsUpdate; // TIMSで1行の更新にかける時間

	CTims(void)
	{
	}

	virtual ~CTims(void)
	{
	}

	void initialize(void)
	{
		Kind = 0; // 列車種別
		Charactor = 26; // 列車番号(記号)
		From = 0; // 駅名表示(始発)
		Destination = 0; // 駅名表示(行先)
		This = 0; // 駅名表示(自駅)
		Next = 0; // 駅名表示(次駅)
		NextBlink = 0; // 駅名表示の点滅
		Crawl = 0; // 徐行区間
		CrawlLimit = 0; // 徐行速度
		AfterChara = 0; // 次運用記号
		LastStopTrack = 0; // 終点到着番線
		RelayName = 0; // 行路終点駅名
		TrainArrow = 0; // 行路表矢印

		m_dist = 0; // 停止予定点距離

		m_pushUpFlag = -1; // 表示更新のフラグ
		m_pushUpBeacon = 0; // 表示更新の地上子
		m_pushUpCount = 0; // 表示更新の繰り返し数
		m_tmrVisible = *Time; // モニタのステップ更新

		m_thisName = 0; // 自駅名
		m_nextName = 0; // 次駅名

		m_pathStaDist = 0; // 開通情報の駅位置情報

		m_distance = 0; // 走行距離
		m_direction = 1; // 距離減算の判定
		m_distDef = 0; // 列車位置の補正

		ResetArray(); // 配列の初期化
		// PushNext(); // 次駅を最新にする
		// Receive(0); // テーブルを更新する

		Distance1000 = 0; // 走行距離(kmの桁)
		Distance100 = 0; // 走行距離(100mの桁)
		Distance10 = 0; // 走行距離(10mの桁)
		Location1000 = 0; // 列車位置(kmの桁)
		Location100 = 0; // 列車位置(100mの桁)
		Location10 = 0; // 列車位置(10mの桁)

		TimsSpeed100 = 10; // TIMS速度(100の桁)
		TimsSpeed10 = 10; // TIMS速度(10の桁)
		TimsSpeed1 = 0; // TIMS速度(1の桁)

		BrakeDelay = 0; // ブレーキ指令
		BcCaution = 0; // 200kPa警告
		BcPressCut = 0; // ブレーキ減圧する
		RelayState = 0; // 走行検知リレー
		Direction = 0; // 進行方向
		ArrowDirection = 0; // 進行方向矢印

		Speed = 0; // 速度計
		Speed100 = 10; // 速度(100の桁)
		Speed10 = 10; // 速度(10の桁)
		Speed1 = 0; // 速度(1の桁)
		BcPress0 = 10; // ブレーキシリンダ指針(0-180kPa)
		BcPress1 = 10; // ブレーキシリンダ指針(200-380kPa)
		BcPress2 = 10; // ブレーキシリンダ指針(400-580kPa)
		BcPress3 = 10; // ブレーキシリンダ指針(600-780kPa)
		MrPress0 = 10; // 元空気ダメ指針(700-790kPa)
		MrPress1 = 10; // 元空気ダメ指針(800-890kPa)
		MrPress2 = 10; // 元空気ダメ指針(900-990kPa)
		Ammeter = 0; // 電流計

		SlideArrow = *c_DAtcSlideArrow; // ATCスライド速度
		AccelCutting = 0; // 力行遅延

		m_tmrBcPressCut = 0; // ブレーキ減圧のタイマー

		m_relayD = ATS_SOUND_STOP; // 走行検知リレー
		m_lbInit = ATS_SOUND_STOP; // 初回起動時断流器音
		m_airHigh = ATS_SOUND_STOP; // 非常ブレーキ緩解音
		AirApply = ATS_SOUND_STOP; // ブレーキ昇圧音
		m_bcPrsDelta = 0.0F; // ブレーキシリンダ圧力[kPa]
		m_emgAnnounce = ATS_SOUND_STOP; // 非常ブレーキ放送
		m_airApplyEmg = ATS_SOUND_STOP; // ブレーキ昇圧音(非常)
		m_updateInfo = ATS_SOUND_STOP; // 運行情報更新
		m_deltaUpdate = *Time - UPDATE_SPAN; // 前回運行情報更新

		// 交直切替
		AC = 0; // 交流
		DC = 1; // 直流
		CVacc = 0; // 制御電圧異常
		CVacc10 = 10; // 制御電圧(10位)
		CVacc1 = 10; // 制御電圧(1位)
		ACacc = 0; // 交流電圧異常
		ACacc10000 = 10; // 交流電圧(10000位)
		ACacc1000 = 10; // 交流電圧(1000位)
		ACacc100 = 10; // 交流電圧(100位)
		DCacc = 0; // 直流電圧異常
		DCacc1000 = 10; // 直流電圧(1000位)
		DCacc100 = 10; // 直流電圧(100位)
		DCacc10 = 10; // 直流電圧(10位)
		Cvmeter = 100; // 制御指針
		Acmeter = 0; // 交流指針
		Dcmeter = 1470; // 直流指針
		Accident = 0; // 事故
		Tp = 0; // 三相
		VCB_ON = 1; // VCB全入
		VCB_OFF = 0; // VCB全切
		VCB = 1; // VCB
		tmr_ACDC = *Time;
		step_ACDC = 3;
		alert_ACDC = 0;
	}

	void execute(void)
	{
		// 毎フレーム
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
		float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]
		float def2 = *TrainSpeed / 3600 * *DeltaT; // 1フレームで動いた距離(相対値)[m]
		m_dist -= def; // 残り距離を減算する

		// 交直切替
		int temp_time = *Time - tmr_ACDC;
		switch(step_ACDC)
		{
		case 0: // VCB切
			VCB_OFF = 1;
			VCB_ON = 0;
			VCB = 2;
			
			if(temp_time > 250)
			{
				Accident = 1; // 事故
			}
			if(temp_time > 500)
			{
				DCacc = 1; // 直流電圧異常
				DCacc1000 = 10; // 直流電圧(1000位)
				DCacc100 = 10; // 直流電圧(100位)
				DCacc10 = 10; // 直流電圧(10位)
				Dcmeter = 0; // 直流指針
			}
			if(temp_time > 750)
			{
				CVacc = 1; // 制御電圧異常
				Cvmeter = 83; // 制御指針
				CVacc10 = 8; // 制御電圧(10位)
				CVacc1 = 3; // 制御電圧(1位)
			}
			if(temp_time > 1000)
			{
				Tp = 1; // 三相
			}
			break;
		case 1: // 無電区間
			VCB_OFF = 0;
			VCB_ON = 0;
			VCB = 0;

			AC = 0;
			DC = 0;
			ACacc = 0; // 交流電圧異常
			ACacc10000 = 10; // 交流電圧(10000位)
			ACacc1000 = 10; // 交流電圧(1000位)
			ACacc100 = 10; // 交流電圧(100位)
			DCacc = 0; // 直流電圧異常
			DCacc1000 = 10; // 直流電圧(1000位)
			DCacc100 = 10; // 直流電圧(100位)
			DCacc10 = 10; // 直流電圧(10位)
			Acmeter = 0; // 交流指針
			Dcmeter = 0; // 直流指針
			break;
		case 2: // AC有電区間
			VCB_OFF = 0;
			VCB_ON = 1;
			VCB = 1;

			DC = 0;
			DCacc = 0; // 直流電圧異常
			DCacc1000 = 10; // 直流電圧(1000位)
			DCacc100 = 10; // 直流電圧(100位)
			DCacc10 = 10; // 直流電圧(10位)
			Dcmeter = 0; // 直流指針

			AC = 1;
			if(temp_time > 500)
			{
				ACacc = 1; // 交流電圧異常
				ACacc10000 = 10; // 交流電圧(10000位)
				ACacc1000 = 2; // 交流電圧(1000位)
				ACacc100 = 2; // 交流電圧(100位)
				Acmeter = 220; // 交流指針
			}
			if(temp_time > 1000)
			{
				ACacc = 1; // 交流電圧異常
				ACacc10000 = 1; // 交流電圧(10000位)
				ACacc1000 = 1; // 交流電圧(1000位)
				ACacc100 = 2; // 交流電圧(100位)
				Acmeter = 11200; // 交流指針
			}
			if(temp_time > 2000)
			{
				ACacc = 0; // 交流電圧異常
				ACacc10000 = 10; // 交流電圧(10000位)
				ACacc1000 = 10; // 交流電圧(1000位)
				ACacc100 = 10; // 交流電圧(100位)
				Acmeter = 15500; // 交流指針
			}
			if(temp_time > 3000)
			{
				ACacc = 0; // 交流電圧異常
				ACacc10000 = 10; // 交流電圧(10000位)
				ACacc1000 = 10; // 交流電圧(1000位)
				ACacc100 = 10; // 交流電圧(100位)
				Acmeter = 18000; // 交流指針
			}
			if(temp_time > 4000)
			{
				ACacc = 0; // 交流電圧異常
				ACacc10000 = 10; // 交流電圧(10000位)
				ACacc1000 = 10; // 交流電圧(1000位)
				ACacc100 = 10; // 交流電圧(100位)
				Acmeter = 20000; // 交流指針
			}
			if(temp_time > 5000)
			{
				ACacc = 0; // 交流電圧異常
				ACacc10000 = 10; // 交流電圧(10000位)
				ACacc1000 = 10; // 交流電圧(1000位)
				ACacc100 = 10; // 交流電圧(100位)
				Acmeter = 22000; // 交流指針
			}
			if(temp_time > 8000)
			{
				Accident = 0; // 事故
			}
			if(temp_time > 12000)
			{
				Tp = 0; // 三相
			}
			if(temp_time > 13000)
			{
				CVacc = 0; // 制御電圧異常
				CVacc10 = 10; // 制御電圧(10位)
				CVacc1 = 10; // 制御電圧(1位)
				Cvmeter = 100; // 制御指針
			}
			break;
		case 3: // DC有電区間
			AC = 0;
			ACacc = 0; // 交流電圧異常
			Acmeter = 0; // 交流指針
			break;
		}

		// 初回起動時力行遅延
		if(ini_lbInit > 0 && *PowerNotch > 0)
		{
			ini_lbInit = 0;
			AccelCutting = *Time + 500;
			m_lbInit = ATS_SOUND_PLAY;
		}

		// 初回起動時断流器音
		LbInit = m_lbInit;
		m_lbInit = ATS_SOUND_CONTINUE;

		// ブレーキ減圧
		if(m_tmrBcPressCut < *Time)
		{
			BcPressCut = 0;
			m_tmrBcPressCut = 0;
		}
		else
		{
			BcPressCut = 1;
		}

		// 走行検知リレー
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

		// 非常ブレーキ緩解音
		AirHigh = m_airHigh;
		m_airHigh = ATS_SOUND_CONTINUE;

		// ブレーキ昇圧音
		if(*BrakeNotch > 0 && BcPressure > 40.0F && BcPressure > m_bcPrsDelta)
		{
			AirApply = ATS_SOUND_PLAYLOOPING;
		}
		else
		{
			AirApply = ATS_SOUND_STOP;
		}

		m_bcPrsDelta = BcPressure; // ブレーキシリンダ圧力[kPa]

		// 非常ブレーキ放送
		EmgAnnounce = m_emgAnnounce;
		m_emgAnnounce = ATS_SOUND_CONTINUE;

		// ブレーキ昇圧音(非常)
		AirApplyEmg = m_airApplyEmg;
		m_airApplyEmg = ATS_SOUND_CONTINUE;

		// 運行情報更新
		if(*TrainSpeed > 10.0f && *Time > m_deltaUpdate + UPDATE_SPAN)
		{
			m_deltaUpdate = *Time;

			if(rand()%200 == 0)
			{
				m_updateInfo = ATS_SOUND_PLAY;
			}
		}

		UpdateInfo = m_updateInfo; // 運行情報更新
		m_updateInfo = ATS_SOUND_CONTINUE;

		// 次駅を点滅させる
		if(speed * speed / TIMS_DECELERATION >= m_dist - 50 && m_dist > 0 && !m_blinking) // 速度照査 (パターン)
		{
			m_blinking = true;

			if(m_pushUpFlag == 1) // 停車駅のとき
			{
				m_pushUpFlag = 2;
			}
		}

		// 駅名表示の点滅
		NextBlink = m_pushUpFlag == 2 ? ((*Time % 750) / 375) : 1;

		// 開通情報25mごと(0無表示、1未開通区間、2停止境界1、3停止境界2、4停止境界3、5停止境界4、6開通区間1、7開通区間2、8駅区間1、9駅区間2)
		for(int initPath2=0;initPath2<120;initPath2++)
		{
			// 開通区間の表示(青線)
			TrackPath2[initPath2] = 6;
		}

		// 開通情報の駅位置情報
		m_pathStaDist -= def2; // 残り距離を減算する

		if(m_pathStaDist > 0)
		{
			int pathStaDistfabsf25 = (int)fabsf(m_pathStaDist) / 25;

			// 駅区間の表示(太い青線)
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

		// 開通情報の閉そく情報
		for(int i=0;i<10;i++)
		{
			// 残り距離を減算する
			m_pathSecDist[i] -= def2;
			m_passSecLine[i] -= def2;

			// 閉そく区切り線の表示
			if(m_passSecLine[i] > 0)
			{
				int passSecLinefabsf25 = (int)fabsf(m_passSecLine[i]) / 25; // 区切り線を入れたい地点

				if(TrackPath2[passSecLinefabsf25] == 6){TrackPath2[passSecLinefabsf25] = 7;} // 開通区間1なら区切り線を入れる
			}

			// 走行限界の表示
			if(m_pathSecDist[i] > 0)
			{
				int pathSecDistfabsf25 = (int)fabsf(m_pathSecDist[i]) / 25; // 走行できる限界の地点
				TrackPath2[pathSecDistfabsf25] = 5; // 停止限界の表示(赤い三角形)
				TrackPath2[pathSecDistfabsf25 + 1] = 4; // 停止限界の表示(赤い三角形)
				TrackPath2[pathSecDistfabsf25 + 2] = 3; // 停止限界の表示(赤い三角形)
				TrackPath2[pathSecDistfabsf25 + 3] = 2; // 停止限界の表示(赤い三角形)

				for(int pathNone2=pathSecDistfabsf25+4;pathNone2<120;pathNone2++)
				{
					TrackPath2[pathNone2] = 1; // 未開通区間の表示(白線)
				}
			}
		}

		// グラスコックピット
		m_tmrUpdate -= abs(*DeltaT); // 毎フレーム減算
		if(m_tmrUpdate < 0) // || *TrainSpeed == 0) // 更新する
		{
			Speed = fabs(*TrainSpeed);
			Speed100 = fabs(*TrainSpeed) / 100;
			Speed10 = (fabs(*TrainSpeed) / 10) - (Speed100 * 10);
			Speed1 = fabs(*TrainSpeed) - (Speed100 * 100 + Speed10 * 10);

			if(*TrainSpeed < 1000 && Speed100 == 0){Speed100 = 10;}
			if(*TrainSpeed < 100 && Speed10 == 0){Speed10 = 10;}

			/* 2015/01/31 unic: マスコンキー未投入でブレーキ指令が反映されしまう */
			BrakeDelay = *BrakeNotch; // ブレーキ指令
			BcCaution = BcPressure < 200 && !m_door; // 200kPa警告

			BcPress0 = 10; // ブレーキシリンダ指針(0-180kPa)
			BcPress1 = 10; // ブレーキシリンダ指針(200-380kPa)
			BcPress2 = 10; // ブレーキシリンダ指針(400-580kPa)
			BcPress3 = 10; // ブレーキシリンダ指針(600-780kPa)
			MrPress0 = 10; // 元空気ダメ指針(700-790kPa)
			MrPress1 = 10; // 元空気ダメ指針(800-890kPa)
			MrPress2 = 10; // 元空気ダメ指針(900-990kPa)

			if(BcPressure < 200){BcPress0 = BcPressure / 20;} // ブレーキシリンダ指針(0-180kPa)
			else if(BcPressure < 400){BcPress1 = (BcPressure - 200) / 20;} // ブレーキシリンダ指針(200-380kPa)
			else if(BcPressure < 600){BcPress2 = (BcPressure - 400) / 20;} // ブレーキシリンダ指針(400-580kPa)
			else if(BcPressure < 800){BcPress3 = (BcPressure - 600) / 20;} // ブレーキシリンダ指針(600-780kPa)

			if(MrPressure <= 700){MrPress0 = 10;} // 700kPa以下
			else if(MrPressure > 700 && MrPressure < 800){MrPress0 = (MrPressure - 700) / 10;} // 元空気ダメ指針(700-790kPa)
			else if(MrPressure < 900){MrPress1 = (MrPressure - 800) / 10;} // 元空気ダメ指針(800-890kPa)
			else if(MrPressure < 1000){MrPress2 = (MrPressure - 900) / 10;} // 元空気ダメ指針(900-990kPa)

			Ammeter = Current; // 電流計
			SlideArrow = *c_DAtcSlideArrow; // ATCスライド速度

			m_tmrUpdate = 200 + (*Time % 50) * 5;
		}

		// 右モニター速度計
		m_distance += (def2 / 10) * m_direction; // 毎フレーム減算
		m_tmrTims -= abs(*DeltaT); // 毎フレーム減算
		if(m_tmrTims < 0) // || *TrainSpeed == 0) // 更新する
		{
			Distance1000 = fabs(m_distance) / 100; // 走行距離(kmの桁)
			Distance100 = (fabs(m_distance) / 10) - (Distance1000 * 10); // 走行距離(100mの桁)
			Distance10 = fabs(m_distance) - (Distance1000 * 100 + Distance100 * 10); // 走行距離(10mの桁)

			double tmp_location = Location + m_distDef; // 列車位置補正を適用する
			Location1000 = tmp_location / 1000; // 列車位置(kmの桁)
			Location100 = (tmp_location / 100) - (Location1000 * 10); // 列車位置(100mの桁)
			Location10 = (tmp_location / 10) - (Location1000 * 100 + Location100 * 10); // 列車位置(10mの桁)

			TimsSpeed100 = fabs(*TrainSpeed) / 100;
			TimsSpeed10 = (fabs(*TrainSpeed) / 10) - (TimsSpeed100 * 10);
			TimsSpeed1 = fabs(*TrainSpeed) - (TimsSpeed100 * 100 + TimsSpeed10 * 10);

			if(*TrainSpeed < 1000 && TimsSpeed100 == 0){TimsSpeed100 = 10;}
			if(*TrainSpeed < 100 && TimsSpeed10 == 0){TimsSpeed10 = 10;}

			m_tmrTims = 1200;
		}

		// ユニット表示
		for(int i=0;i<10;i++) // 状態の更新
		{
			// TIMS用
			if(Current > 0 && UnitTims[i] != 1 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 1;}
			else if(Current < 0 && UnitTims[i] != 2 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 2;}
			else if(Current == 0 && UnitTims[i] != 0 && rand() % RANDOMIZE_UNITTIMS == 0){UnitTims[i] = 0;}

			// 表示灯用
			if(Current != 0 && UnitState[i] != 1 && rand() % RANDOMIZE_UNITLAMP == 0){UnitState[i] = 1;}
			else if(Current == 0 && UnitState[i] != 0 && rand() % RANDOMIZE_UNITLAMP == 0){UnitState[i] = 0;}
		}

		// 進行方向矢印
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

		// プッシュアップイベント
		if(*TrainSpeed != 0) // 駅ジャンプを除外する
		{
			if((m_pushUpFlag == 2 && (m_pushUpBeacon == 1 || *TrainSpeed < 2.0f)) || (m_pushUpFlag == -1 && m_dist <= 0))
			{
				m_pushUpFlag = 0;

				// ステップ更新の回数だけループ
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

							// 次駅を最新にする
							if(i == 3)
							{
								PushNext();
							}
						}
					}
				}
			}
		}
		else if(m_pushUpFlag == 3 || m_pushUpBeacon == 2) // 駅ジャンプのとき
		{
			m_pushUpFlag = 0;
			PushNext(); // 次駅を最新にする

			for(int i=0;i<10;i++)
			{
				PushUp(i);
			}
		}
		else if(m_pushUpFlag == -1) // 起動時の初期化
		{
			PushNext(); // 次駅を最新にする
		}

		// ステップ更新の処理
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

	// ドアが開いた
	void DoorOpening(void)
	{
		m_door = 0; // ドア状態
		m_tmrVisible = 0; // 表示の更新をさせない
	}

	// ドアが閉まった
	void DoorClosing(void)
	{
		m_door = 1; // ドア状態
	}

	// 次駅接近した
	void Receive(int data)
	{
		m_pushUpFlag = data >= 0 ? 1 : -1;
		m_pushUpBeacon = 0;
		m_pushUpCount = abs(data) > 100000 ? 0 : abs(data) / 10000 > 0 ? abs(data) / 10000 : 1;

		m_dist = abs(data % 10000) - TIMS_OFFSET;
		m_blinking = false;

		m_pathStaDist = abs(data % 10000) + 5.0F;

		if(*TrainSpeed == 0) // 駅ジャンプを除外する
		{
			// m_pushUpFlag = abs(data) < 1000000 ? 3 : 0;
			m_pushUpFlag = abs(data) < 100000 ? 3 : 0;
			m_pushUpCount = 0;
			m_dist = 0;
			m_pathStaDist = 0;
		}
	}

	// 表示を更新する
	void UpdateView(void)
	{
		m_pushUpBeacon = *TrainSpeed != 0 ? 1 : 2; // 表示更新の地上子
	}

	// 自駅、次駅の設定
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

	// 列車種別の設定
	void SetKind(int data)
	{
		Kind = data;
	}

	// 列車番号の設定
	void SetNumber(int data)
	{
		Charactor = data / 10000;
		Number[0] = ((data % 10000) - (data % 1000)) / 1000;
		Number[1] = ((data % 1000) - (data % 100)) / 100;
		Number[2] = ((data % 100) - (data % 10)) / 10;
		Number[3] = data % 10;
	}

	// 運行区間の設定
	void SetLeg(int data)
	{
		From = data / 100;
		Destination = data % 100;
	}

	// 徐行区間の設定
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

	// 次運用の設定
	void SetAfteruent(int type, int data)
	{
		if(type == 0) // 列車番号
		{
			AfterChara = data / 10000;
			AfterNumber[0] = ((data % 10000) - (data % 1000)) / 1000;
			AfterNumber[1] = ((data % 1000) - (data % 100)) / 100;
			AfterNumber[2] = ((data % 100) - (data % 10)) / 10;
			AfterNumber[3] = data % 10;
		}
		else if(type == 1) // 到着時刻
		{
			AfterTime[0][0] = data / 10000;
			AfterTime[0][1] = ((data % 10000) - (data % 100)) / 100;
			AfterTime[0][2] = (data % 100) / 5;
		}
		else if(type == 2) // 発車時刻
		{
			AfterTime[1][0] = data / 10000;
			AfterTime[1][1] = ((data % 10000) - (data % 100)) / 100;
			AfterTime[1][2] = (data % 100) / 5;
		}
	}

	// 進行方向設定
	void SetDirection(int direction)
	{
		switch(direction)
		{
		case 1: // 左
			Direction = 1;
			break;
		case 2: // 右
			Direction = 2;
			break;
		case 0:
		default: // 例外
			Direction = 0;
			break;
		}
	}

	// 終点駅の設定
	void SetLastStop(int type, int data)
	{
		if(type == 0) // 到着時刻
		{
			LastStopTime[0] = data / 10000;
			LastStopTime[1] = ((data % 10000) - (data % 100)) / 100;
			LastStopTime[2] = (data % 100) / 5;
		}
		else if(type == 1) // 到着番線
		{
			LastStopTrack = data;
		}
	}

	// 行路終点駅名設定
	void SetRelayStation(int data)
	{
		RelayName = data;
	}

	// 行路表矢印の設定
	void SetArrowState(int data)
	{
		TrainArrow = data;
	}

	// 走行距離の設定
	void SetDistance(int data)
	{
		if(data >= 10000) // 距離減算するかどうか
		{
			m_direction = -1;
		}
		else 
		{
			m_direction = 1;
		}

		m_distance = data % 10000; // 走行距離
	}

	// 列車位置補正の設定
	void SetPositionDef(int data)
	{
		m_distDef = data; // 列車位置の補正
	}

	// スタフテーブルの書き込み
	void InputLine(int colum, int line, int data)
	{
		switch(colum) // 列の振り分け
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

	// 開通情報の停止閉そく情報を更新する
	void CheckPath(int signal, float dist=0, int section=0)
	{
		if((signal == 0 || signal == 9 || signal == 10) && dist > 0)
		{
			m_pathSecDist[section] = dist; // 開通情報の閉そく情報
			m_passSecLine[section] = 0; // 開通情報の閉そく区切り線
		}
		else
		{
			m_pathSecDist[section] = 0;
			m_passSecLine[section] = dist;
		}
	}

	// 非常ブレーキ緩解音を再生する
	void PlaySoundAirHigh(int state, int newState)
	{
		if(state == EmergencyNotch && newState < EmergencyNotch && BcPressure > 340)
		{
			m_airHigh = ATS_SOUND_PLAY;
			m_tmrBcPressCut = *Time + ini_cutPressTime; // ブレーキ減圧のタイマー
		}
	}

	// 非常ブレーキ放送を再生する
	void PlaySoundEmgAnnounce(int state, int newState)
	{
		if(state != EmergencyNotch && newState == EmergencyNotch && *TrainSpeed > 10.0F)
		{
			m_emgAnnounce = ATS_SOUND_PLAY;
		}
	}

	// 初回起動時断流器音をリセットする
	void SetLbInit(int pos, int enable)
	{
		if(pos == 0 && enable == 1)
		{
			ini_lbInit = 1;
		}
	}

	// ブレーキ昇圧音(非常)を再生する
	void PlaySoundAirApplyEmg(int state, int newState)
	{
		if(state != EmergencyNotch && newState == EmergencyNotch && BcPressure < 150.0f)
		{
			m_airApplyEmg = ATS_SOUND_PLAY;
		}
	}

	// 交直切替を行う
	void SetACDC(int state)
	{
		step_ACDC = state;
		tmr_ACDC = *Time;
	}

	// 交直切替のお知らせ
	void AlartACDC(int state)
	{
		alert_ACDC = state;
	}

	// ランダムシードを変更する
	void RandomizeSeed(void)
	{
		srand(*Time);
	}

private:
	int m_door; // ドア状態
	float m_dist; // 停止予定点距離
	bool m_blinking; // 次駅点滅かどうか

	int m_pushUpFlag; // 表示更新のフラグ
	int m_pushUpBeacon; // 表示更新の地上子
	int m_pushUpCount; // 表示更新の繰り返し数
	int m_tmrVisible; // モニタのステップ更新
	int m_update[7]; // ステップ更新の状態
	int m_thisName; // 自駅名
	int m_nextName; // 次駅名
	int m_nextTime[3]; // 次駅時刻

	float m_pathStaDist; // 開通情報の駅位置情報
	float m_pathSecDist[10]; // 開通情報の閉そく情報
	float m_passSecLine[10]; // 開通情報の閉そく区切り線

	double m_distance; // 走行距離
	int m_direction; // 距離減算するかどうか
	double m_distDef; // 列車位置の補正

	int m_tmrUpdate; // 速度計の更新タイマー
	int m_tmrTims; // 右モニターの更新タイマー
	int m_tmrBcPressCut; // ブレーキ減圧のタイマー

	int m_relayD; // 走行検知リレー
	int m_lbInit; // 初回起動時断流器音
	int m_airHigh; // 非常ブレーキ緩解音
	int m_airApply; // ブレーキ昇圧音
	float m_bcPrsDelta; // ブレーキシリンダ圧力[kPa]
	int m_emgAnnounce; // 非常ブレーキ放送
	int m_airApplyEmg; // ブレーキ昇圧音(非常)
	int m_updateInfo; // 運行情報更新
	int m_deltaUpdate; // 前回運行情報更新

	// スタフテーブルのプッシュアップ
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

	// 入力されている次駅を画面に反映
	void PushNext(void)
	{
		This = m_thisName;
		Next = m_nextName;
		NextTime[0] = m_nextTime[0];
		NextTime[1] = m_nextTime[1];
		NextTime[2] = m_nextTime[2];
	}

	// 配列の初期化
	void ResetArray(void)
	{
		for(int c3=0;c3<3;c3++) // 3回ループ
		{
			LastStopTime[c3] = 0;
			m_nextTime[c3] = 0;

			for(int c3_2=0;c3_2<2;c3_2++) // 2回ループ
			{
				AfterTime[c3_2][c3] = 0;
			}
		}

		for(int c4=0;c4<4;c4++) // 4回ループ
		{
			Number[c4] = 10;
			AfterNumber[c4] = 10;
		}

		for(int c10=0;c10<10;c10++) // 10回ループ
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

			// 時刻テーブルには 24:60:60 (24, 60, 60) を出力
			for(int c10_3=0;c10_3<3;c10_3++) // 10回それそれに3回ずつループ
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

		for(int c120=0;c120<120;c120++) // 120回ループ
		{
			TrackPath2[c120] = 0;
		}
	}

};	// CTims

#endif	// ATS_TIMS_HPP_INCLUDED
