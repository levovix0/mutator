#pragma once

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

������

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <deque>

#include "basic_oracle.h"
#include "sable_stat.h"

struct _oracle3 : public _basic_curve
{
	struct _element_oracle
	{
		int time = 0; // �����
		_iinterval ncc; // �������� ���

		ushort min = 0; // ������� �� y
		ushort max = 0; // ������� �� y

		bool operator < (int a) const noexcept { return (time < a); } // ��� ��������� ������ �� �������
	};
	static const int max_part = 22000; // ����������� ���������� ��������� ss

	std::deque<_prices> part_ss; // ����� �����-����������
	i64 begin_ss = 0; // ������ ����� �����-����������
	std::vector<_element_oracle> zn; // ������

	i64  get_n()                              override { return zn.size(); } // ���������� ���������
	void get_n_info(i64 n, _element_chart* e) override; // �������� ������� ���������� n-�� ��������
	void get_t_info(int t, _element_chart* e) override; // �������� ������� ���������� �������� �� �������� >= t
	void draw(i64 n, _area area)              override; // ���������� 1 �������
	void recovery()                           override; // ���������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

