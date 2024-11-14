#define ATS_BEACON_S 0 // S�����O
#define ATS_BEACON_SN 1 // SN����
#define ATS_BEACON_SNRED 2 // SN��o���h�~
#define ATS_BEACON_P 3 // P��~�M��
#define ATS_BEACON_EMG 4 // P����(���)
#define ATS_BEACON_SVC 5 // P����(��p)
#define ATS_BEACON_SPDLIM 6 // P����푬�x����
#define ATS_BEACON_SPDMAX 7 // P�ō����x
#define ATS_BEACON_SPP 8 // ��ԉw�ʉߖh�~���u
#define ATS_BEACON_SX 9 // Sx��_�Ԓʉߎ��ԑ���
#define ATS_BEACON_DATC 10 // D-ATC�X�V
/*
#define ATS_BEACON_PS1 11 // Ps���p�^�[������
#define ATS_BEACON_PS2 12 // Ps���p�^�[������
#define ATS_BEACON_PSC 13 // Ps���E���p�^�[������
#define ATS_BEACON_PSM 14 // Ps�p�^�[����ʔF��
#define ATS_BEACON_PSP 15 // Ps����푬�x����
#define ATS_BEACON_PSR 16 // Ps�Ȑ����x����
#define ATS_BEACON_PSL 17 // Ps������z���x����
#define ATS_BEACON_PST 18 // Ps�Վ����x����
#define ATS_BEACON_PSY 19 // Ps�����p�^�[������
#define ATS_BEACON_PSZ 20 // Ps�U���p�^�[������
*/
#define ATS_BEACON_APP 100 // TIMS���w�ڋ�
#define ATS_BEACON_UPD 101 // TIMS�X�V�錾
#define ATS_BEACON_NEXT 102 // TIMS���w�ݒ�
#define ATS_BEACON_PATH 103 // TIMS�J�ʏ��
#define ATS_BEACON_CRWL 104 // TIMS���s���
#define ATS_BEACON_STA 105 // TIMS�w���\��
#define ATS_BEACON_ARV 106 // TIMS��������
#define ATS_BEACON_LEV 107 // TIMS�o������
#define ATS_BEACON_TRL 108 // TIMS�����Ԑ���������x
#define ATS_BEACON_SPN 109 // TIMS�w�Ԏ���
#define ATS_BEACON_KIND 110 // TIMS��Ԏ��
#define ATS_BEACON_NUM 111 // TIMS��Ԕԍ�
#define ATS_BEACON_LEG 112 // TIMS�^�s���
#define ATS_BEACON_DIST 113 // TIMS�������Z�ݒ�
#define ATS_BEACON_DISDEF 114 // TIMS��Ԉʒu�␳
#define ATS_BEACON_SUBN 115 // TIMS���^�p���
#define ATS_BEACON_SUBA 116 // TIMS���^�p����
#define ATS_BEACON_SUBL 117 // TIMS���^�p����
#define ATS_BEACON_DIR 119 // TIMS�i�s�����ݒ�
// #define ATS_BEACON_FSA 120 // TIMS�n�����Ԏ���
// #define ATS_BEACON_FST 121 // TIMS�n�����ԔԐ�
#define ATS_BEACON_LSA 122 // TIMS�I�_��������
#define ATS_BEACON_LST 123 // TIMS�I�_�����Ԑ�
#define ATS_BEACON_RST 124 // TIMS�s�H�I�_�w��
#define ATS_BEACON_ARW 125 // TIMS�s�H�\���

int g_emgBrake; // ���m�b�`
int g_svcBrake; // ��p�ő�m�b�`
int g_brakeNotch; // �u���[�L�m�b�`
int g_powerNotch; // �͍s�m�b�`
int g_reverser; // ���o�[�T
bool g_pilotlamp; // �p�C���b�g�����v
int g_time; // ���ݎ���
float g_speed; // ���x�v�̑��x[km/h]
int g_deltaT; // �t���[������[ms/frame]

ATS_HANDLES g_output; // �o��

CAtssn g_atssn; // ATS-SN
CAtsp g_atsp; // ATS-P
//CAtsps g_atsps; // ATS-Ps
CDatc g_datc; // D-ATC
CSpp g_spp; // ��ԉw�ʉߖh�~���u
CEb g_eb; // EB���u
CTims g_tims; // TIMS���u
CDate g_date; // ���t�@�\
CSub g_sub; // ���̑��@�\
CAccel g_accel; // �����x
NewSnp2Cfg g_ini; // Ini�t�@�C��