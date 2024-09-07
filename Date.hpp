// ŽžŠÔî•ñ˜A“®‘•’u
// Copyright (C) 2009 Unicorn

#ifndef ATS_DATE_HPP_INCLUDED
#define ATS_DATE_HPP_INCLUDED

class CDate
{
public:
	// ŽžŠÔ•Ï”E\‘¢‘Ì
	time_t t;
	struct tm *status;

	// o—Í
	int Cooler; // —â–[
	int CoolerSound; // —â–[‰¹

	CDate(void)
	{
	}

	virtual ~CDate(void)
	{
	}

	// ‰Šú‰»‚·‚é
	void initialize(void)
	{
		m_month = 0; // ŒŽ

		Cooler = 0; // —â–[
		CoolerSound = ATS_SOUND_STOP; // —â–[‰¹

		update();
	}

	// “ú•t‚ðXV‚·‚é
	void update(void)
	{
		time(&t);
		status = localtime(&t);
		m_month = status->tm_mon + 1;

		// —â–[‚Ì§Œä
		if(Cooler == 0)
		{
			Cooler = (m_month >= 6 && m_month <= 9);
		}
	}

private:
	int m_month; // ŒŽ(1`12)
};

#endif	// ATS_DATE_HPP_INCLUDED