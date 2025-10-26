#define ATSP_DECELERATION 19.5F // 減速定数 (減速度[km/h/s] x 7.2)
// 19.5=55^2/(180-25)
#define ATSP_OFFSET 0.5F // 車上子オフセット[m]
#define ATSP_VO 25 // 停止予定点から信号機までの距離[m]

class CAtsp
{
private:
	float m_DistSig; // 停止予定点までの距離[m]
	float m_DistLimSpd; // 速度制限位置までの距離[m]
	int m_target; // 目標速度[km/h]
	int m_limSpd; // 制限速度[km/h]
	int m_maxSpd; // 最高速度[km/h]
	int m_approachLampOld; // 前回のパターン接近表示灯
	int m_bell; // 単打ベルの状態

	void applyBrake(int); // ブレーキ動作
	void startAtsp(void); // ATS-P投入
	void blowBell(bool); // 単打ベルを鳴らす
	void applyEmgBrake(void); // 非常ブレーキ動作
	void init(void); // 変数の初期化

public:
	float *TrainSpeed; // 列車速度[km/h]
	int *DeltaT; // フレーム時間[ms/frame]
	int *BrakeNotch; // ブレーキノッチ
	int *Reverser; // レバーサー位置
    int EmergencyNotch; // 車両の非常ノッチ
	int ServiceNotch; // 車両のATS確認ノッチ
	int Bell; // 単打ベル
	int ApproachLamp; // パターン接近表示灯
	int BrakeApplyLamp; // ブレーキ動作表示灯
	int AtspLamp; // ATS-P表示灯
	bool ServiceBrake; // 常用ブレーキ
	bool EmergencyBrake; // 非常ブレーキ
    
	CAtsp(void);
	virtual ~CAtsp(void);
	void RunP(void);
	void InitP(void);
	void PassedSig(float);
	void PassedStopEmg(float);
	void PassedStopSvc(float);
	void PassedSpeedLim(int, int);
	void PassedSpeedMax(int);
	void Reset(void);
	void Exit(void);
};
