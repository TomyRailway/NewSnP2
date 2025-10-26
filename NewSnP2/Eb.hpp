// JR各社 EB装置
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_EB_HPP_INCLUDED
#define ATS_EB_HPP_INCLUDED

#include "stdafx.h"
#include <cmath> // fabsf のために必要

class CEb
{
private:
	int m_tmrPhase1;   // 60秒タイマー
	int m_tmrPhase2;   // 5秒タイマー
	int m_ebSwitch;    // EBスイッチ
	int m_iniEBEnabled; // iniで読み込んだEB電源（0=OFF, 1=ON）

public:
	int EmergencyNotch;   // 非常ノッチ
	int* Time;            // 現在時刻ミリ秒（ポインタで参照）
	float* TrainSpeed;    // 列車速度 [km/h]
	int Emergency;        // 非常制動状態
	int Action;           // EB作動状態
	int RedLamp;          // 赤ランプ点滅
	int Bell;             // ATSベル
	int Buzzer;           // EBブザー
	int EbSwitch;         // EBスイッチ音

	CEb() = default;
	virtual ~CEb() = default;

	// 初期化関数（iniから受け取ったEB状態を引数で指定）
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

	// メイン処理（Elapseで呼び出し）
	void execute()
	{
		if (m_iniEBEnabled == 0) return; // EB電源OFFなら無効化

		RedLamp = 0;
		Buzzer = ATS_SOUND_STOP;
		Bell = ATS_SOUND_STOP;

		if (*TrainSpeed == 0)
		{
			ResetPhase1(); // 静止中は初期化
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

	// 停止中などでタイマーを初期化
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

	// Brake設定で呼び出し：非常ブレーキ復帰処理
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
