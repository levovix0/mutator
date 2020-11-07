#pragma once

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

����� ��������� �������������

/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "basic_oracle.h"

struct _nervous_oracle2 : public _basic_curve
{
	struct _element_nervous // ������� ���������� �������� �������
	{
		time_t time = 0; // �����
		_iinterval ncc;     // �������� ���

		ushort min_pok = 0; // ����������� �������
		ushort max_pok = 0; // ������������ �������
		ushort min_pro = 0; // ����������� �������
		ushort max_pro = 0; // ������������ �������

		int    v_r = 0; // ���������� ���������
		double r = 0; // ������� ������
		double r_pok = 0; // ������� ������ �������
		double r_pro = 0; // ������� ������ �������

		bool operator < (int a) const noexcept { return (time < a); } // ��� ��������� ������ �� �������
	};

	std::vector<_element_nervous> zn; // ������

	i64 get_n() override { return zn.size(); } // ���������� ���������
	void get_n_info(i64 n, _element_chart* e) override; // �������� ������� ���������� n-�� ��������
	void get_t_info(int t, _element_chart* e) override; // �������� ������� ���������� �������� �� �������� >= t
	void draw(i64 n, _area area)              override; // ���������� 1 �������
	void recovery() override; // ���������
	void push(_stack* mem);
	void pop(_stack* mem);
	_latest_events get_latest_events(i64 nn); // �������� ��������� �������
	i64 prediction() override;                // ������� ������� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

