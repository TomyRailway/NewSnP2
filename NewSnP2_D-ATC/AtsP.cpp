#include "stdafx.h"
#include "atsplugin.h"
#include "Atsp.h"

CAtsp::CAtsp()
{
}

CAtsp::~CAtsp()
{
}

// 初期化する
void CAtsp::InitP()
{
	m_approachLampOld = 0;
	init();
}

// ATS-SNに切替
void CAtsp::Exit()
{
	if(AtspLamp){blowBell(false);}
	init();
}

// 地上子通過
void CAtsp::PassedSig(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;
}

// 分岐器速度制限地上子通過
void CAtsp::PassedSpeedLim(int speed, int distance)
{
	startAtsp();
	m_limSpd = speed;
	m_DistLimSpd = distance - ATSP_OFFSET;
}

// 最高速度を設定
void CAtsp::PassedSpeedMax(int speed)
{
	startAtsp();
	m_maxSpd = speed;
}

// 即時停止(非常)地上子通過
void CAtsp::PassedStopEmg(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;

	if(distance < 50 && *TrainSpeed != 0){applyEmgBrake();}
}

// 即時停止(常用)地上子通過
void CAtsp::PassedStopSvc(float distance)
{
	startAtsp();
	m_DistSig = distance - ATSP_VO;

	if(distance < 50 && *TrainSpeed != 0){applyBrake(0);}
}

// 復帰
void CAtsp::Reset()
{
	if(EmergencyBrake && *BrakeNotch == EmergencyNotch){EmergencyBrake = false;} // 非常ブレーキ動作の場合
	if(ServiceBrake && *BrakeNotch >= ServiceNotch){ServiceBrake = false;} // 常用ブレーキ動作の場合

	if(!EmergencyBrake && !ServiceBrake)
	{
		BrakeApplyLamp = 0; // ブレーキ動作表示灯
		ApproachLamp = 0; // パターン接近表示灯
	}
}

void CAtsp::RunP()
{
	float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
    float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]
	m_DistSig -= def; // 残り距離を減算する
	m_DistLimSpd -= def; // 残り距離を減算する
	if(m_DistLimSpd < 0){m_DistLimSpd = 2000000;} // 制御点を越えたらパターンを消去する

	// 後退検知
	if(*TrainSpeed * *Reverser < -6){applyEmgBrake();}

	// パターンを計算する
	float patternSig = speed * speed / ATSP_DECELERATION; // 停止信号
	float pSeepdLim = (speed * speed - m_limSpd * m_limSpd) / ATSP_DECELERATION; // 速度制限

	// 緩解処理
	if(ServiceBrake)
	{
		// 目標速度以下になるか接近パターンを下回った(現示アップした)ら緩解する
		if(speed < m_target || (patternSig < m_DistSig - 50 && m_target <= 0))
		{
			ServiceBrake = false;
			BrakeApplyLamp = 0;
			ApproachLamp = 0; // パターン解除
		}
	}

	// ブレーキ処理
	if(!ServiceBrake && !EmergencyBrake)
	{
		// パターン接近
		if(patternSig >= m_DistSig - 50 && m_DistSig > -50 - ATSP_VO || 
		   pSeepdLim >= m_DistLimSpd - 50 && m_DistLimSpd > 0 && speed >= m_limSpd ||
		   speed >= m_maxSpd - 5)
		{
			ApproachLamp = 1; // パターン接近
		}
		else
		{
			ApproachLamp = 0; // パターン解除
		}
		
		// ブレーキ動作
		if(patternSig >= m_DistSig && speed >= 16 && m_DistSig > -50 - ATSP_VO) // 停止信号に対するブレーキパターン
		{
			applyBrake(0);
		}
		if(pSeepdLim >= m_DistLimSpd && m_DistLimSpd > 0) // 速度制限に対するブレーキパターン
		{
			applyBrake(m_limSpd - 5);
		}
		if(speed >= m_maxSpd + 1) // 最高速度に対するブレーキパターン
		{
			applyBrake(m_maxSpd - 5);
		}
	}

	if(ApproachLamp != m_approachLampOld)
	{
		blowBell((bool)m_approachLampOld); // 単打ベルを鳴らす
		m_approachLampOld = ApproachLamp;
	}

	Bell = m_bell; // サウンドを更新
	m_bell = ATS_SOUND_CONTINUE; // CONTINUEにしておく
}

// 初期化する
void CAtsp::init()
{
	m_DistSig = -50 - ATSP_VO; // 停止予定点距離
	m_DistLimSpd = 0; // 速度制限
	m_maxSpd = 370; // 最高速度[km/h]
	ApproachLamp = 0; // パターン接近表示灯
	BrakeApplyLamp = 0; // ブレーキ動作表示灯
	ServiceBrake = false; // 常用ブレーキ
	EmergencyBrake = false; // 非常ブレーキ
	AtspLamp = 0; // ATS-P表示灯
}

// ATS-P投入
void CAtsp::startAtsp()
{
	if(AtspLamp == 0)
	{
		AtspLamp = 1; // ATS-P表示灯点灯
		blowBell(false); // 単打ベルを鳴らす
	}
}

// ブレーキ動作
void CAtsp::applyBrake(int targetSpeed)
{
	ServiceBrake = true;
	BrakeApplyLamp = 1; // ブレーキ動作表示灯点灯
	m_target = targetSpeed; // 目標速度
}

// 単打ベルを鳴らす
void CAtsp::blowBell(bool off)
{
	if(*TrainSpeed != 0 || off){m_bell = ATS_SOUND_PLAY;} // 駅ジャンプ時には鳴らさない
}

// 非常ブレーキ動作
void CAtsp::applyEmgBrake()
{
	EmergencyBrake = true; // 非常ブレーキ
	BrakeApplyLamp = 1; // ブレーキ動作表示灯点灯
	ApproachLamp = 1; // パターン接近表示灯点灯
}