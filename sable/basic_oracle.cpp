#include "basic_oracle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _latest_events::start()
{
	if ((minute[2] == 2) && (event[0] == event[1]) && (event[0] == event[2])) // �������
	{
		if (event[0] == 1) return 70;
		if (event[0] == 2) // ����������
			if ((x[0] > x[1]) && (x[1] > x[2]))	return 13;
		if (event[0] == 3)
		{
			if ((x[0] > x[1]) && (x[1] > x[2]))	return 40;
			if ((x[0] < x[1]) && (x[1] < x[2]))	return 90;
		}
		//		if (event_[0] == 4) return 120;  //�������
		if (event[0] == 6) return 60;   //�������
		return 0;
	}
	if ((minute[1] == 1) && (event[0] == event[1])) // ������
	{
		if (event[0] == 2) // ����������
		{
			if (event[2] == 5) return 100; //��������
			if ((event[2] == 4) && (event[3] == 4)) return 40;
		}
		return 0;
	}
	return 0;
}

bool _latest_events::stop()
{
	if ((event[0] == 5) && (event[1] == 5) && (minute[1] == 1)) return true; // ��������
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

