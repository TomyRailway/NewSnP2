// ���ԏ��A�����u
// Copyright (C) 2024-2025 TomyRailway

#ifndef ATS_DATE_HPP_INCLUDED
#define ATS_DATE_HPP_INCLUDED

class CDate
{
public:
	// ���ԕϐ��E�\����
	time_t t;
	struct tm *status;

	// �o��
	int Cooler; // ��[
	int CoolerSound; // ��[��

	CDate(void)
	{
	}

	virtual ~CDate(void)
	{
	}

	// ����������
	void initialize(void)
	{
		m_month = 0; // ��

		Cooler = 0; // ��[
		CoolerSound = ATS_SOUND_STOP; // ��[��

		update();
	}

	// ���t���X�V����
	void update(void)
	{
		time(&t);
		status = localtime(&t);
		m_month = status->tm_mon + 1;

		// ��[�̐���
		if(Cooler == 0)
		{
			Cooler = (m_month >= 6 && m_month <= 9);
		}
	}

private:
	int m_month; // ��(1�`12)
};

#endif	// ATS_DATE_HPP_INCLUDED