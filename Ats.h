#define ATS_BEACON_S 0 // Sロング
#define ATS_BEACON_SN 1 // SN直下
#define ATS_BEACON_SNRED 2 // SN誤出発防止
#define ATS_BEACON_P 3 // P停止信号
#define ATS_BEACON_EMG 4 // P即停(非常)
#define ATS_BEACON_SVC 5 // P即停(常用)
#define ATS_BEACON_SPDLIM 6 // P分岐器速度制限
#define ATS_BEACON_SPDMAX 7 // P最高速度
#define ATS_BEACON_SPP 8 // 停車駅通過防止装置
#define ATS_BEACON_SX 9 // Sx二点間通過時間速照
#define ATS_BEACON_DATC 10 // D-ATC更新
#define ATS_BEACON_PS1 11 // Ps第一パターン発生
#define ATS_BEACON_PS2 12 // Ps第二パターン発生
#define ATS_BEACON_PSC 13 // Ps第一・第二パターン消去
#define ATS_BEACON_PSM 14 // Psパターン種別認識
#define ATS_BEACON_PSP 15 // Ps分岐器速度制限
#define ATS_BEACON_PSR 16 // Ps曲線速度制限
#define ATS_BEACON_PSL 17 // Ps下り勾配速度制限
#define ATS_BEACON_PST 18 // Ps臨時速度制限
#define ATS_BEACON_PSY 19 // Ps入換パターン発生
#define ATS_BEACON_PSZ 20 // Ps誘導パターン発生
#define ATS_BEACON_APP 100 // TIMS次駅接近
#define ATS_BEACON_UPD 101 // TIMS更新宣言
#define ATS_BEACON_NEXT 102 // TIMS次駅設定
#define ATS_BEACON_PATH 103 // TIMS開通情報
#define ATS_BEACON_CRWL 104 // TIMS徐行区間
#define ATS_BEACON_STA 105 // TIMS駅名表示
#define ATS_BEACON_ARV 106 // TIMS到着時刻
#define ATS_BEACON_LEV 107 // TIMS出発時刻
#define ATS_BEACON_TRL 108 // TIMS到着番線･制限速度
#define ATS_BEACON_SPN 109 // TIMS駅間時間
#define ATS_BEACON_KIND 110 // TIMS列車種別
#define ATS_BEACON_NUM 111 // TIMS列車番号
#define ATS_BEACON_LEG 112 // TIMS運行区間
#define ATS_BEACON_DIST 113 // TIMS距離加算設定
#define ATS_BEACON_DISDEF 114 // TIMS列車位置補正
#define ATS_BEACON_SUBN 115 // TIMS次運用列番
#define ATS_BEACON_SUBA 116 // TIMS次運用着時
#define ATS_BEACON_SUBL 117 // TIMS次運用発時
#define ATS_BEACON_DIR 119 // TIMS進行方向設定
// #define ATS_BEACON_FSA 120 // TIMS始発発車時刻
// #define ATS_BEACON_FST 121 // TIMS始発発車番線
#define ATS_BEACON_LSA 122 // TIMS終点到着時刻
#define ATS_BEACON_LST 123 // TIMS終点到着番線
#define ATS_BEACON_RST 124 // TIMS行路終点駅名
#define ATS_BEACON_ARW 125 // TIMS行路表矢印

int g_emgBrake; // 非常ノッチ
int g_svcBrake; // 常用最大ノッチ
int g_brakeNotch; // ブレーキノッチ
int g_powerNotch; // 力行ノッチ
int g_reverser; // レバーサ
bool g_pilotlamp; // パイロットランプ
int g_time; // 現在時刻
float g_speed; // 速度計の速度[km/h]
int g_deltaT; // フレーム時間[ms/frame]

ATS_HANDLES g_output; // 出力

CAtssn g_atssn; // ATS-SN
CAtsp g_atsp; // ATS-P
CAtsps g_atsps; // ATS-Ps
CDatc g_datc; // D-ATC
CSpp g_spp; // 停車駅通過防止装置
CEb g_eb; // EB装置
CTims g_tims; // TIMS装置
CDate g_date; // 日付機能
CSub g_sub; // その他機能
CAccel g_accel; // 加速度
Snp2Cfg g_ini; // Iniファイル
