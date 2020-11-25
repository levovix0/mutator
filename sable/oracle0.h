#pragma once

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

������������ �������� �����
��� ������������

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "basic_oracle.h"

struct _mctds_candle : public _basic_curve // �������� ������ ��� ���������� ������� - ������������ �����
{
	struct _cen_pak // ������
	{
		ushort min   = 0; // ����������� ����
		ushort max   = 0; // ����������� ����
		ushort first = 0; // ������ ����
		ushort last  = 0; // ��������� ����
		double cc    = 0; // ������� ����
		_iinterval ncc;   // �������� ���
		int time     = 0; // ����� �����

		bool operator < (time_t a) const noexcept { return (time < a); } // ��� ��������� ������ �� �������
	};

	std::vector<_cen_pak> cen1m; // ����������� ���� �� �������

	i64  get_n()                              override { return cen1m.size(); } // ���������� ���������
	void get_n_info(i64 n, _element_chart* e) override; // �������� ������� ���������� n-�� ��������
	void get_t_info(time_t t, _element_chart* e) override; // �������� ������� ���������� �������� �� �������� >= t
	void draw(i64 n, _area area)              override; // ���������� 1 �������
	void recovery()                           override; // ��������
	void save_to_file()                       override;
	void load_from_file()                     override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

