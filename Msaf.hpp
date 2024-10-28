// 相模鉄道 ATS
// Copyright (C) TomyRailway 2024

#ifndef ATS_MSAF_HPP_INCLUDED
#define ATS_MSAF_HPP_INCLUDED

#include "stdafx.h"
#include "atsplugin.h"

#define ATSMSAF_DECELERATION 19.5F // 減速定数 (減速度[km/h/s] x 7.2)

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
	int m_signal; // 信号現示
	int m_stepMs; // 閉そく内MS点の状態
	int m_preSignal; // 次閉そくの信号現示
	int m_limit; // 曲線照査の状態
	int m_loop; // R現示用ループの状態
	int m_halt; // 停車パターン
	double m_dist; // 停止位置までの距離
	int m_sps; // 二点間通過時間速照

	int m_result_sig; // 信号照査の結果
	int m_result_lim; // 曲線照査の結果

	int m_haltChime; // 停車チャイム
	int m_passChime; // 通過チャイム

public:
	int *Time; // 現在時刻[s]
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[flame/ms]
	int *BrakeNotch; // ブレーキノッチ
	int EmgNotch; // 非常ノッチ
	int SvcNotch; // 常用最大ノッチ

	int AtsBrake; // ATSブレーキ

	int Ats; // ATS電源
	int Normal; // ATS正常
	int Action; // ATS動作

	int AtsBell; // ATSベル
	int HaltChime; // 停車チャイム
	int PassChime; // 通過チャイム

	// Initalizeで実行します
	void initialize(void)
	{
		AtsBrake = 0; // ATSブレーキ

		Ats = 0; // ATS電源
		Action = 0; // ATS動作

		m_stepMs = 0; // 閉そく内MS点の状態
		m_limit = 0; // 曲線照査の状態
		m_loop = 0; // R現示用ループの状態
		m_halt = 0; // 停車パターン
		m_dist = 0; // 停止位置までの距離
		m_sps = 0; // 二点間通過時間速照

		m_result_sig = 0; // 信号照査の結果
		m_result_lim = 0; // 曲線照査の結果

		m_haltChime = ATS_SOUND_STOP; // 停車チャイム
		m_passChime = ATS_SOUND_STOP; // 通過チャイム
	}

	// Elapseで実行します
	void execute(void)
	{
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
		float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]

		// 信号による照査
		switch(m_signal)
		{
		case SIGNAL_G: // フリーラン
			if(!m_stepMs || (m_stepMs && m_preSignal > SIGNAL_Y)) // 次閉そくYより優位のとき
			{
				break;
			}
		case SIGNAL_YG: // 65
			if(speed > SPEED_YG){m_result_sig = ATS_SVC;}
			else{m_result_sig = ATS_FREE;}
			break;
		case SIGNAL_Y: // 45
			if(!m_stepMs || (m_stepMs && m_preSignal > SIGNAL_R)) // 次閉そくRより優位のとき
			{
				if(speed > SPEED_Y){m_result_sig = ATS_SVC;}
				else{m_result_sig = ATS_FREE;}
				break;
			}
		case SIGNAL_YY: // 25
			if(!m_stepMs) // MS点でない
			{
				if(speed > SPEED_YY){m_result_sig = ATS_SVC;}
				else{m_result_sig = ATS_FREE;}
				break;
			}
		case SIGNAL_R: // 15
		default:
			if(m_loop) // R現示用ループである
			{
				if(speed > 0){m_result_sig = ATS_EMG;}
			}
			else
			{	
				if(speed > SPEED_R){m_result_sig = ATS_EMG;}
			}
			break;
		}

		// 曲線による照査
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

		// 停車パターンによる照査
		if(m_halt)
		{
			m_dist -= def; // 毎フレーム減算
			float limit = pow(speed, 2.0f) / ATSMSAF_DECELERATION; // 停車パターン

			if(limit >= m_dist && m_dist > 0) // P当たった
			{
				m_result_lim = ATS_EMG;
			}
		}

		// ATSブレーキに結果を更新
		AtsBrake = max(m_result_sig, m_result_lim);

		// 表示灯を更新
		Ats = 1; // ATS電源
		Normal = (AtsBrake == 0); // ATS正常
		Action = (AtsBrake > 0); // ATS動作

		// ATSベル
		AtsBell = AtsBrake ? ATS_SOUND_PLAYLOOPING : ATS_SOUND_STOP;

		HaltChime = m_haltChime; // 停車チャイム
		m_haltChime = ATS_SOUND_CONTINUE;
		PassChime = m_passChime; // 通過チャイム
		m_passChime = ATS_SOUND_CONTINUE;
	}

	// ATS復帰スイッチで実行します
	void reset(void)
	{
		if(*BrakeNotch >= SvcNotch && *TrainSpeed == 0)
		{
			AtsBrake = 0;

			// 照査の結果をリセットする
			m_result_sig = 0;
			m_result_lim = 0;
		}
	}

	// DoorOpenで実行します
	void DoorOpening()
	{
		m_halt = 0;
		m_haltChime = ATS_SOUND_STOP;
	}

	// SetSignalで実行します
	void ChangedSignal(int signal)
	{
		m_signal = signal; // 信号現示
		m_preSignal = m_signal;
		m_stepMs = 0;
		m_loop = 0;
	}

	// 相鉄MS点を通過した時に実行します
	void PassedMs(int signal)
	{
		m_preSignal = signal;
		m_stepMs = 1;
	}

	// 相鉄曲線速度照査を通過した時に実行します
	void PassedCl(int data)
	{
		m_limit = data;
	}

	// 相鉄R現示ループを通過した時に実行します
	void PassedLoop(int data)
	{
		m_loop = data > 0;
	}

	// 相鉄二点間通過時間速照を通過した時に実行します
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

	// 相鉄誤通過防止を通過した時に実行します
	void PassedSpp(int is_halt, int dist)
	{
		if(is_halt == 1) // 停車駅
		{
			m_halt = 1;
			m_dist = dist > 0 ? (dist) : -(dist);
			m_haltChime = ATS_SOUND_PLAY;
		}
		else if(is_halt == 0) // 通過駅
		{
			m_passChime = ATS_SOUND_PLAY;
		}
	}

};	// CMsaf

#endif	// ATS_MSAF_HPP_INCLUDED
