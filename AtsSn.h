class CAtssn
{
private:
	int m_atsAlarm; // 警報過程
	int m_tmrAtspassAlarm; // チャイム切替タイマ
	int m_tmrAts; // 5秒タイマ
	int m_blnAtsLamp; // 赤色灯
	bool m_atsButton; // ATSボタン

	void alarmSound(void);
	void cancelAts(void);

public:
	int *Time; // 現在時刻[s]
	int *Notch; // ブレーキノッチ
	int CancelNotch; // 車両のATS確認ノッチ
	int EmgNotch; // 車両の非常ノッチ
	bool AtsBrake; // ATSブレーキ
	int Bell; // ATS警報
	int Chime; // 持続チャイム
	int RedLamp; // 赤色灯
	int WhiteLamp; // 白色灯

	CAtssn(void);
	virtual ~CAtssn(void);
	void RunSn(void);
	void CheckAts(void);
	void InitSn(void);
	void DownButtom(void);
	void CancelChime(void);    
	void UpButton(void);
	void PassedLong(int);
	void PassedShort(int);
	void Reset(void);
	void TurnOff(void);
};
