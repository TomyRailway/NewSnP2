#include "stdafx.h"
#include "atsplugin.h"
#include "Spp.h"

CSpp::CSpp()
{
}

CSpp::~CSpp()
{
}

void CSpp::RunSpp()
{
	float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
    float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]
	m_dist -= def; // 残り距離を減算する

	// 警報を鳴らす
	if(speed * speed / SPP_DECELERATION >= m_dist - 50 && m_dist > 0 && !m_played) // 速度照査 (パターン)
	{
		m_played = true;
		if(m_halt) // 停車の場合
		{
// #if(SPP_ALARMTYPE) // 1回発音型
			m_haltChime = ATS_SOUND_PLAY;
// #else              // 繰り返し発音型
			if(*BrakeNotch >= ServiceNotch)
			{
				m_haltChime2 = -1200; // ブレーキを込めた場合は音量を小さくする (-12dB)
			}
			else
			{
				m_haltChime2 = 0;
			}
// #endif
		}
		else // 通過の場合
		{
			m_passAlarm = ATS_SOUND_PLAY;
		}
	}

	// サウンドを更新
	HaltChime = m_haltChime;
	HaltChime2 = m_haltChime2;
	PassAlarm = m_passAlarm;

	// CONTINUEにしておく
	if(m_haltChime == ATS_SOUND_PLAY){m_haltChime = ATS_SOUND_CONTINUE;}
	if(m_haltChime2 == ATS_SOUND_PLAY){m_haltChime2 = ATS_SOUND_CONTINUE;}
	m_passAlarm = ATS_SOUND_CONTINUE;
}

void CSpp::NotchChanged()
{
	// ブレーキを込めた場合は音量を小さくする (-12dB)
	if(m_haltChime2 == 0 && *BrakeNotch >= ServiceNotch){m_haltChime2 = -1200;}
}

// チャイムを消す
void CSpp::StopChime()
{
	m_haltChime = ATS_SOUND_STOP;
	m_haltChime2 = ATS_SOUND_STOP;
}

// 車上子の情報を受け取る
void CSpp::Receive(int data)
{
	m_halt = data >= 0;
	m_dist = abs(data) - SPP_OFFSET;
	m_played = false;
	m_haltChime = ATS_SOUND_STOP;
	m_haltChime2 = ATS_SOUND_STOP;
}

// 初期化する
void CSpp::InitSpp()
{
	m_dist = 0;
	m_haltChime = ATS_SOUND_STOP;
	m_haltChime2 = ATS_SOUND_STOP;
}
