// JR東日本 ATS-Ps
// Copyright (C) 2009 Unicorn

#ifndef ATS_ATSPS_HPP_INCLUDED
#define ATS_ATSPS_HPP_INCLUDED

#include "stdafx.h"
#include "atsplugin.h"
#include "AtsPs.h"

#define ATSPS_DECELERATION 19.5F // 減速定数 (減速度[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
 #define ATSPS_OFFSET 0.5F // 車上子オフセット[m]

class CAtsps
{
public:
	int EmergencyNotch; // 非常ノッチ
	int *BrakeNotch; // ブレーキノッチ
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[ms/frame]

	int AtsBrake; // ATSブレーキ

	int AtsPs; // Ps電源
	int IsAtsPs; // ATS-Ps
	int PatternStart; // パターン発生
	int PatternApproach; // パターン接近
	int BrakeTurnOff; // ブレーキ開放
	int BrakeTurnOn; // ブレーキ動作
	int TrainSpeedLED; // 列車速度LED
	int PatternSpeedLED; // パターン速度LED
	int BrakeDown; // 故障

	int PatternBegin; // Psパターン発生
	int PatternEnd; // Psパターン終了
	int Chime; // Psチャイム

	CAtsps(void)
	{
	}

	virtual ~CAtsps()
	{
	}

	// Initalizeで実行します
	void initialize(void)
	{
		Init();
	}

	/*
    // ATS-SNに切替
    void CAtsps::Exit()
    {
	    if(AtspLamp){blowBell(false);}
	    init();
    }

    

	*/

	// Elapseで実行します
	void execute(void)
	{
		if(IsAtsPs == 0) // 電源が切である
		{
			AtsBrake = 0; // ATSブレーキ
		}
		else // 電源が入である
		{
		}

		PatternBegin = m_patternBegin; // Psパターン発生
		m_patternBegin = ATS_SOUND_CONTINUE;
		PatternEnd = m_patternEnd; // Psパターン終了
		m_patternEnd = ATS_SOUND_CONTINUE;
		Chime = m_chime; // Psチャイム
		m_chime = ATS_SOUND_CONTINUE;
	}

	// SetSignalで実行します
	void ChangedSignal(int signal)
	{
		// !Powerなら、Start(); // 電源を入れる
	}

	// ATS-Psを遮断するときに実行します
	void Exit(void)
	{
		if(IsAtsPs == 1)
		{
			IsAtsPs = 0; // ATS-Ps
		}
	}

	// ATS-Psを投入するときに実行します
	void Start(void)
	{
		if(IsAtsPs == 0)
		{
			IsAtsPs = 1; // ATS-Ps
		}
	}

private:
	int m_patternBegin; // Psパターン発生
	int m_patternEnd; // Psパターン終了
	int m_chime; // Psチャイム

	void Init(void)
	{
		AtsPs = 1; // Ps電源

		m_patternBegin = ATS_SOUND_STOP; // Psパターン発生
		m_patternEnd = ATS_SOUND_STOP; // Psパターン終了
		m_chime = ATS_SOUND_STOP; // Psチャイム
	}

};	// CAtsps

#endif	// ATS_ATSPS_HPP_INCLUDED
