#include "stdafx.h"
#include "atsplugin.h"
#include "Atssn.h"

CAtssn::CAtssn(void)
{
}

CAtssn::~CAtssn(void)
{
}

void CAtssn::RunSn(void)
{
	// ATS作動
	if(m_atsAlarm == 2)
	{
		if(*Time >= m_tmrAtspassAlarm && m_tmrAtspassAlarm){cancelAts();} // 起動時チャイム自動切替

		if(*Time >= m_tmrAts && !AtsBrake)
		{
			AtsBrake = true;
			// メッセージ「ATS取り扱い不良です」
		}

		// 赤色灯点滅
		if(AtsBrake)
		{
			int n = (*Time % 750) / 375;
			RedLamp = n;
			m_blnAtsLamp = n;
		}
	}
}

// ATS確認扱い
void CAtssn::CheckAts(void)
{
	if(*Notch >= CancelNotch && m_atsButton){cancelAts();} // ATS確認
}

// 初期化する
void CAtssn::InitSn(void)
{
	AtsBrake = false;
	m_atsAlarm = 0;
	Bell = ATS_SOUND_STOP;
	Chime = ATS_SOUND_STOP;
	WhiteLamp = 1;
	RedLamp = 0;
}

// ATS確認ボタンを押す
void CAtssn::DownButtom(void)
{
	m_atsButton = true;
	CheckAts();
}

// チャイムを消す
void CAtssn::CancelChime(void)
{
	if(m_atsAlarm == 1)
	{
        m_atsAlarm = 0;
		Chime = ATS_SOUND_STOP;
        // メッセージ「持続チャイムを止めてはいけません」
	}
}

// ATS確認ボタンを離す
void CAtssn::UpButton(void)
{
	m_atsButton = false;
}

// ロング地上子通過
void CAtssn::PassedLong(int signal)
{
	if(signal == 0)
	{
		if(m_atsAlarm != 2)
		{
			m_tmrAts = *Time + 5000;
			m_tmrAtspassAlarm = 0;
			alarmSound();
			// メッセージ「ATS確認扱いをしてください」
		}
	}
	else if(WhiteLamp == 0)
	{
		m_tmrAts = *Time + 5000;
		m_tmrAtspassAlarm = *Time + 375;
		alarmSound();
	}
}

// 直下地上子通過
void CAtssn::PassedShort(int signal)
{
	if(signal == 0)
	{
		if(!AtsBrake)
		{
			AtsBrake = true; // ブレーキ動作
			// メッセージ「信号を守りなさい」
			if(m_atsAlarm != 2){alarmSound();}
		}
	}
	else if(WhiteLamp == 0)
	{
		m_tmrAts = *Time + 5000;
		m_tmrAtspassAlarm = *Time + 375;
		alarmSound();
	}
}

// 復帰
void CAtssn::Reset(void)
{
	if(*Notch == EmgNotch && AtsBrake)
	{
		AtsBrake = false; // ブレーキ緩解
		m_atsAlarm = 0; // チャイム過程
		Bell = ATS_SOUND_STOP; // ベル停止
		WhiteLamp = 1; // 白色灯点灯
		RedLamp = 0; // 赤色灯滅灯
	}
}

// 開放
void CAtssn::TurnOff()
{
	AtsBrake = false; // ブレーキ緩解
	m_atsAlarm = 0; // チャイム過程
	Bell = ATS_SOUND_STOP; // ベル停止
	Chime = ATS_SOUND_STOP; // チャイム停止
	WhiteLamp = 0; // 白色灯滅灯
	RedLamp = 0; // 赤色灯滅灯
}

// ATS確認
void CAtssn::cancelAts()
{
	if(m_atsAlarm == 2 && !AtsBrake)
	{
		m_atsAlarm = 1; // チャイム過程
		Bell = ATS_SOUND_STOP; // ベル停止
		Chime = ATS_SOUND_PLAYLOOPING; // チャイム鳴動
		WhiteLamp = 1; // 白色灯点灯
		RedLamp = 0; // 赤色灯滅灯
	}
}

// ATS警報を鳴らす
void CAtssn::alarmSound(void)
{
	m_atsAlarm = 2; // ベル過程
	m_blnAtsLamp = true; // 赤色灯点灯
	Chime = ATS_SOUND_STOP; // チャイム停止
	Bell = ATS_SOUND_PLAYLOOPING; // ベル鳴動
	WhiteLamp = 0; // 白色灯滅灯
	RedLamp = 1; // 赤色灯点灯
}