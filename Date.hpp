// 時間情報連動装置
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_DATE_HPP_INCLUDED
#define ATS_DATE_HPP_INCLUDED

class CDate
{
public:
	// 時間変数・構造体
	time_t t;
	struct tm *status;

	// 出力
	int Cooler; // 冷房
	int CoolerSound; // 冷房音

	CDate(void)
	{
	}

	virtual ~CDate(void)
	{
	}

	// 初期化する
	void initialize(void)
	{
		m_month = 0; // 月

		Cooler = 0; // 冷房
		CoolerSound = ATS_SOUND_STOP; // 冷房音

		update();
	}

	// 日付を更新する
	void update(void)
	{
		time(&t);
		status = localtime(&t);
		m_month = status->tm_mon + 1;

		// 冷房の制御
		if(Cooler == 0)
		{
			Cooler = (m_month >= 6 && m_month <= 9);
		}
	}

private:
	int m_month; // 月(1〜12)
};

#endif	// ATS_DATE_HPP_INCLUDED