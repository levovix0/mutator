#pragma once

#include "basic_go.h"

constexpr int rceni = 20; // ����������� �������, ����������� ������� ( ����� = Rceni * 2 );

struct _offer // �����������
{
	ushort c; // ����
	int k; // ����������
};

struct _date_time // ���� � �����
{
	uchar month, day, hour, minute, second; // (�����+���), ����, ���, ���, ���

	operator int(); // ������������� � ����� �����
	int to_minute(); // ������������� � ����� ����� ��� ������
	void operator =(int a); // ��������� �����
	void now(); // ��������� ������� �����
};

struct _prices // ������ ������ �����������
{
	_offer pok[rceni]; // ����������� ������� (������� � ������ ���������)
	_offer pro[rceni]; // ����������� ������� (������� � ������ ���������)
	_date_time time; // �����

	inline void clear() { time.second = 200; } // ����� ������� ������
	inline bool empty() { return (time.second == 200); } // �������� �� ������� 
};

struct _super_stat // ����� ���������� ���
{
	struct _info_pak // �������������� ���������� ��� ��������
	{
		bool ok; // ���� ������
		int r; // ����� ������
		int r_pro; // ������ ������
		int r_pok; // ������ �������
	};
	int size; // ���������� �������

	_super_stat();
	void add(_prices& c); // �������� ���� (�����)
	void read(int n, _prices& c, _info_pak* inf = 0); // ��������� ���� (�������)
	void save_to_file(wstr fn);
	void load_from_file(wstr fn);
	void clear(); // ������� ��� ������

private:
	_prices last_cc; // ��������� ����
	_stack data; // ������ ������
	std::vector<int> u_dd2; // ��������� �� ����� ������ ������ ������� StepPakCC

	static const int step_pak_cc = 100;
	_prices read_cc; // ��������� ����������� ����
	int read_n; // ����� ��������� ����������� ���
	_info_pak ip_last, ip_n; // �������������� ����������

	void otgruzka(int rez, int Vrez, int* deko); // ��������������� Pak()
};

struct _g_graph : public _t_go
{
	_g_graph() { local_area = { {0, 100}, {0, 100} }; }

	uchar type()                     override { return 9; }
	int get_froglif()                override { return 0xF6; } // !!!!!

	void ris2(_trans tr, bool final) override;

};
