#define SPP_DECELERATION 19.5F // 減速定数 (減速度[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define SPP_OFFSET 0.5F // 車上子オフセット[m]
#define SPP_ALARMTYPE 1 // 0: 繰り返し発音, 1; 1回発音

class CSpp
{
private:
	float m_dist; // 停止予定点距離
	bool m_played;
	bool m_halt;
	int m_haltChime;
	int m_haltChime2;
	int m_passAlarm;

public:
	float *TrainSpeed; // 列車速度[km/h]
	int *DeltaT; // フレーム時間
	int *BrakeNotch; // 現在のブレーキノッチ

	int HaltChime; // 停車チャイム
	int HaltChime2; // 停車チャイムループ
	int PassAlarm; // 通過案内音声
	int ServiceNotch; // ATS確認ノッチ

	CSpp(void);
	virtual ~CSpp(void);
	void RunSpp(void);
	void InitSpp(void);
	void Receive(int);
	void NotchChanged(void);
	void StopChime(void);
};
