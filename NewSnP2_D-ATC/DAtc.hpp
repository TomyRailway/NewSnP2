// JR東日本 D-ATC
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_DATC_HPP_INCLUDED
#define ATS_DATC_HPP_INCLUDED

#include "stdafx.h"

#define DATC_DECELERATION 14.4F // 減速定数 (減速度[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define DATC_OFFSET 0.5F // 車上子オフセット[m]

#define ATCMODE_DATC 1 // 0:10型ATC 1:D-ATC

class CDatc
{
public:
	int EmergencyNotch; // 非常ノッチ
	int *BrakeNotch; // ブレーキノッチ
	int *Time; // 現在時刻
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[ms/frame]

	int AtcBrake; // ATCブレーキ

	int DAtc; // ATC電源
	int IsDAtc; // D-ATC
	int EmgDriving; // ATC非常運転
	int AtcEmg; // ATC非常
	int AtcSvc; // ATC常用
	int AtcApproach; // パターン接近

	int Signal[2]; // ATC現示
	int Arrow[32]; // ATC速度
	int SlideArrow; // ATCスライド速度
	int IsOrp; // ORP
	int OrpNeedle; // ORP速度

	int SigFall; // 前方予告

	int AtcDing; // ATC単打ベル
	int VoiceAts; // ATS切替音声
	int VoiceAtc; // ATC切替音声

	CDatc()
	{
	}

	virtual ~CDatc()
	{
	}

	// Initalizeで実行します
	void initialize(void)
	{
		DAtc = 1; // D-ATC
		EmgDriving = 0; // ATC非常運転
		AtcApproach = 0; // パターン接近
		m_falling = 0; // ATC降下表示

		ResetIndicator(); // ATC表示リセット

		m_deltaApproach = 0; // 前回パターン接近
		m_atcDing = ATS_SOUND_STOP; // ATC単打ベル
		m_voiceAts = ATS_SOUND_STOP; // ATS切替音声
		m_voiceAtc = ATS_SOUND_STOP; // ATC切替音声
	}

	// Elapseで実行します
	void execute(void)
	{
		if(IsDAtc == 0) // 電源が切である
		{
			AtcBrake = 0; // ATCブレーキ
		}
		else // 電源が入である
		{
			float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
			float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]

			if(EmgDriving == 1) // ATC非常運転である
			{
				AtcBrake = speed > 26.0f ? 1 : 0; // 25km/h制限

			}
			else // ATC非常運転でない
			{
				// ATCブレーキ
				if(m_signal < 10 || m_signal > 42) // 無信号
				{
					AtcBrake = 2;
				}
				else if(m_signal == 10) // 01信号
				{
					AtcBrake = 1;
				}
				else if(*TrainSpeed > itov(m_signal) + 2.0f) // 有効なコード
				{
					AtcBrake = 1;
				}
				else
				{
					AtcBrake = 0;
				}

#if (ATCMODE_DATC)
				if(m_falling == 1) // ATC信号降下
				{
					if(m_distance > 0){m_distance -= def;} // 降下までの距離
					float limit = (pow(speed, 2.0f) - pow(itov(m_target), 2.0f)) / DATC_DECELERATION;
					if(limit >= m_distance){AtcBrake = 1;} // パターン超過
					
					m_deltaApproach = AtcApproach; // 前回パターン接近

					if(itov(m_target) <= speed && limit + (speed / 3.6 * 5) >= m_distance)
					{
						AtcApproach = 1; // パターン接近
					}
					else
					{
						AtcApproach = 0; // パターン接近
					}

					// 接近状態変わった
					if(m_deltaApproach != AtcApproach)
					{
						m_atcDing = ATS_SOUND_PLAY; // ATC単打ベル
					}
				}
#endif
			}
		}

		ResetIndicator(); // ATC表示リセット
		if(IsDAtc == 1){RunIndicator();} // ATC表示部

		AtcEmg = AtcBrake == 2 ? 1 : 0; // ATC非常
		AtcSvc = AtcBrake > 0 ? 1 : 0; // ATC常用

		AtcDing = m_atcDing; // ATC単打ベル
		m_atcDing = ATS_SOUND_CONTINUE;
		VoiceAts = m_voiceAts; // ATS切替音声
		m_voiceAts = ATS_SOUND_CONTINUE;
		VoiceAtc = m_voiceAtc; // ATC切替音声
		m_voiceAtc = ATS_SOUND_CONTINUE;
	}

	// SetSignalで実行します
	void ChangedSignal(int signal)
	{
		// 信号インデックス互換
		switch(signal)
		{
		case 100: Exit(); break; // 切替
		case 101: signal = 9; break; // X
		case 102: signal = 10; break; // 0
		case 103: signal = 13; break; // 15
		case 104: signal = 15; break; // 25
		case 105: signal = 19; break; // 45
		case 106: signal = 21; break; // 55
		case 107: signal = 23; break; // 65
		case 108: signal = 25; break; // 75
		case 109: signal = 28; break; // 90
		case 110: signal = 30; break; // 100
		case 111: signal = 32; break; // 110
		case 112: signal = 34; break; // 120
		default: break;
		}

		if(!IsDAtc && signal >= 9 && signal < 37) // ATC信号のとき
		{
			Start(); // 電源を入れる
		}
		else if(IsDAtc && signal < 9) // ATS信号のとき
		{
			Exit(); // 電源を切る
		}

#if (ATCMODE_DATC)
		if(IsDAtc && !EmgDriving && (itov(signal) && signal > m_signal || !m_falling && signal > m_signal)) // D-ATC
#else
		if(IsDAtc && !EmgDriving && (signal >= 9 && signal <= 42 && signal != m_signal)) // 10型ATC
#endif
		{
			m_atcDing = ATS_SOUND_PLAY; // ATC単打ベル
		}

		m_signal = signal; // 信号の更新
		m_falling = 0; // ATC降下表示
	}

	// D-ATCを遮断するときに実行します
	void Exit(void)
	{
		if(IsDAtc == 1)
		{
			IsDAtc = 0; // D-ATC
			m_atcDing = ATS_SOUND_PLAY; // ATC単打ベル
			m_voiceAts = ATS_SOUND_PLAY; // ATS切替音声

			EmgDriving = 0; // ATC非常運転
			m_falling = 0; // ATC降下表示
		}
	}

	// D-ATCを投入するときに実行します
	void Start(void)
	{
		if(IsDAtc == 0)
		{
			IsDAtc = 1; // D-ATC
			m_atcDing = ATS_SOUND_PLAY; // ATC単打ベル
			m_voiceAtc = ATS_SOUND_PLAY; // ATC切替音声
		}
	}

	// D-ATCの非常運転を切替するときに実行します
	void EmgDrive(void)
	{
		if(IsDAtc == 1 && *BrakeNotch == EmergencyNotch)
		{
			EmgDriving = !EmgDriving ? 1 : 0; // ATC非常運転
			m_atcDing = ATS_SOUND_PLAY; // ATC単打ベル
		}
	}

	// ATC信号降下を通過した時に実行します
	void PassedAtcFall(int signal, float dist)
	{
		// 信号インデックス互換
		switch(signal)
		{
		case 100: Exit(); break; // 切替
		case 101: signal = 9; break; // X
		case 102: signal = 10; break; // 0
		case 103: signal = 13; break; // 15
		case 104: signal = 15; break; // 25
		case 105: signal = 19; break; // 45
		case 106: signal = 21; break; // 55
		case 107: signal = 23; break; // 65
		case 108: signal = 25; break; // 75
		case 109: signal = 28; break; // 90
		case 110: signal = 30; break; // 100
		case 111: signal = 32; break; // 110
		case 112: signal = 34; break; // 120
		default: break;
		}

		if(IsDAtc == 1)
		{
			if(itov(signal) && signal < m_signal)
			{
				m_falling = 1; // ATC降下表示
				m_target = signal; // ATC降下目標
				m_distance = dist; // 降下までの距離
			}
			else
			{
				m_falling = 0; // ATC降下表示
			}
		}
	}

private:
	int m_signal; // 信号現示
	int m_falling; // ATC降下表示
	int m_target; // ATC降下目標
	float m_distance; // 降下までの距離
	int m_deltaApproach; // 前回パターン接近

	int m_atcDing; // ATC単打ベル
	int m_voiceAts; // ATS切替音声
	int m_voiceAtc; // ATC切替音声

	// ATC表示灯を初期化
	void ResetIndicator(void)
	{
		// ATC表示のリセット
		for(int i=0;i<1;i++){Signal[i] = 0;} // ATC現示
		for(int j=0;j<32;j++){Arrow[j] = 0;} // ATC速度
		SlideArrow = -10; // ATCスライド速度
		SigFall = 0; // 前方予告
		IsOrp = 0; // ORP
		OrpNeedle = 0; // ORP速度
	}

	// ATC表示灯を更新
	void RunIndicator(void)
	{
		if(EmgDriving == 1) // ATC非常運転である
		{
			Signal[0] = 1; // 赤現示
			Arrow[6] = 1; // ATC-25
			SlideArrow = 25; // ATCスライド速度
		}
		else // ATC非常運転でない
		{
			// ATC信号
			if(m_signal >= 12 && m_signal <= 42){Signal[1] = 1;}
			else{Signal[0] = 1;}

			// ORP表示灯
			IsOrp = (m_signal == 11);

#if (ATCMODE_DATC)
			if(m_falling == 1) // ATC信号降下
			{
				// ATC速度
				double pattern = sqrt((m_distance * DATC_DECELERATION) + pow(itov(m_target), 2.0f));
				Arrow[vtoi(pattern)] = 1;

				// ATCスライド速度
				if(pattern > itov(m_signal)){SlideArrow = itov(m_signal);}
				else if(pattern > itov(m_target)){SlideArrow = pattern;}
				else{SlideArrow = itov(m_target);}
			}
			else // 通常のとき
			{
				// ATC速度
				if(m_signal < 10 || m_signal > 36){Arrow[0] = 1;}
				else{Arrow[(m_signal-10)+1] = 1;}

				// ATCスライド速度
				SlideArrow = itov(m_signal);
			}
#else
			// ATC速度
			if(m_signal < 10 || m_signal == 11 || m_signal > 36){Arrow[0] = 1;}
			else{Arrow[(m_signal-10)+1] = 1;}

			// ATCスライド速度
			SlideArrow = itov(m_signal);
#endif
			// ATC前方予告
			SigFall = m_falling; // 前方予告

			// ORP速度
			OrpNeedle = (m_signal == 11) ? 0 : 0;
		}
	}

	// インデックスをATC速度に変換する
	int itov(int index)
	{
		int speed = 0;

		// ATC-5 から ATC-130
		if(index >= 12 && index <= 42)
		{
			speed = (index - 10) * 5;
		}
		else // それ以外は無信号
		{
			speed = 0;
		}

		return speed;
	}

	// 速度をArrowインデックスに変換する
	int vtoi(int speed)
	{
		int index = 1;

		index += ((speed % 130) - (speed % 5)) / 5;

		return index;
	}

};	// CDatc

#endif	// ATS_DATC_HPP_INCLUDED
