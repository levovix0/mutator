#pragma once

#include <deque>

#include "basic_go.h"

constexpr int rceni = 20; // ����������� �������, ����������� ������� ( ����� = Rceni * 2 );

void start_set(std::filesystem::path fn); // ������������� ��������
						  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _offer // �����������
{
	ushort c; // ����
	int k; // ����������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _date_time // ���� � �����
{
	uchar month, day, hour, minute, second; // (�����+���), ����, ���, ���, ���

	operator int(); // ������������� � ����� �����
	int to_minute(); // ������������� � ����� ����� ��� ������
	void operator =(int a); // ��������� �����
	void now(); // ��������� ������� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _prices // ������ ������ �����������
{
	_offer pok[rceni]; // ����������� ������� (������� � ������ ���������)
	_offer pro[rceni]; // ����������� ������� (������� � ������ ���������)
	_date_time time; // �����

	inline void clear() { time.second = 200; } // ����� ������� ������
	inline bool empty() { return (time.second == 200); } // �������� �� ������� 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _super_stat // ����� ���������� ���
{
	struct _info_pak // �������������� ���������� ��� ��������
	{
		bool ok; // ���� ������
		int r; // ����� ������
		int r_pro; // ������ ������
		int r_pok; // ������ �������
	};
	int64 size; // ���������� �������

	_super_stat();
	void add(_prices& c); // �������� ���� (�����)
	void read(int64 n, _prices& c, _info_pak* inf = 0); // ��������� ���� (�������)
	void save_to_file(wstr fn);
	void load_from_file(std::wstring_view fn);
	void clear(); // ������� ��� ������

private:
	_prices last_cc; // ��������� ����
	_stack data; // ������ ������
	std::vector<int> u_dd2; // ��������� �� ����� ������ ������ ������� StepPakCC

	static const int step_pak_cc = 100;
	_prices read_cc; // ��������� ����������� ����
	int64 read_n; // ����� ��������� ����������� ���
	_info_pak ip_last, ip_n; // �������������� ����������

	void otgruzka(int rez, int Vrez, int* deko); // ��������������� Pak()
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _element_chart // ������� ���������� �������� �������
{
	int n; // ���������� ����� �������� !!! -1 = ��� ��������
	int time; // �����
	double min; // ����������� ��������
	double max; // ������������ ��������
	double middle; // ������� ��������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _basic_curve // ���� ��� ������
{
	virtual int get_n()                                = 0; // ���������� ���������
	virtual void get_n_info(int n, _element_chart* e)  = 0; // �������� ������� ���������� n-�� ��������
	virtual void get_t_info(int t, _element_chart* e)  = 0; // �������� ������� ���������� �������� �� �������� >= t
	virtual void draw(int n, _area2 area, _bitmap* bm) = 0; // ���������� 1 �������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _mctds_candle : public _basic_curve // �������� ������ ��� ���������� ������� - ������������ �����
{
	struct _cen_pak // ������
	{
		ushort min = 0, max = 0; // ����������� � ����������� ����
		ushort first = 0, last = 0; // ������ � ��������� ����
		double cc = 0.0; // ������� ����
		_areai ncc; // �������� ���
		int time = 0; // ����� �����
	};

	std::vector<_cen_pak> cen1m; // ����������� ���� �� �������
	double c_unpak = 0.01; // ���������� ���

	int get_n(); // ���������� ���������
	void get_n_info(int n, _element_chart* e); // �������� ������� ���������� n-�� ��������
	void get_t_info(int t, _element_chart* e); // �������� ������� ���������� �������� �� �������� >= t
	void draw(int n, _area2 area, _bitmap* bm); // ���������� 1 �������
	void recovery(); // ���������
	void push(_stack* mem);
	void pop(_stack* mem);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _latest_events // ��������� �������
{
	char event[4]; // [0] - ��������� �������
	int minute[4]; // �� ����� ������ ���������
	double x[4]; // ��������

	int start(); // �� ����� ���� � X �����
	bool stop(); // �� ����� �������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _nervous_oracle : public _basic_curve // ��������� �������������
{
	struct _element_nervous // ������� ���������� �������� �������
	{
		int time       = 0; // �����
		_areai ncc;         // �������� ���

		ushort min_pok = 0; // ����������� �������
		ushort max_pok = 0; // ������������ �������
		ushort min_pro = 0; // ����������� �������
		ushort max_pro = 0; // ������������ �������

		int    v_r     = 0; // ���������� ���������
		double r       = 0; // ������� ������
		double r_pok   = 0; // ������� ������ �������
		double r_pro   = 0; // ������� ������ �������

		bool operator < (int a) const noexcept { return (time < a); } // ��� ��������� ������ �� �������
	};

	std::vector<_element_nervous> zn; // ������
	double c_unpak = 0.01; // ���������� ���

	int get_n(); // ���������� ���������
	void get_n_info(int n, _element_chart* e); // �������� ������� ���������� n-�� ��������
	void get_t_info(int t, _element_chart* e); // �������� ������� ���������� �������� �� �������� >= t
	void draw(int n, _area2 area, _bitmap* bm); // ���������� 1 �������
	void recovery(); // ���������
	void push(_stack* mem);
	void pop(_stack* mem);
	_latest_events get_latest_events(int64 nn); // �������� ��������� �������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _oracle3 : public _basic_curve // ������ 3-� ������
{
	struct _element_oracle
	{
		int time = 0; // �����
		_areai ncc; // �������� ���

		ushort min = 0, max = 0; // ������� �� y

		bool operator < (int a) const noexcept { return (time < a); } // ��� ��������� ������ �� �������
	};
	static const int max_part = 22000; // ����������� ���������� ��������� ss

	std::deque<_prices> part_ss; // ����� �����-����������
	int64 begin_ss = 0; // ������ ����� �����-����������
	std::vector<_element_oracle> zn; // ������
	double c_unpak = 0.01; // ���������� ���

	int get_n(); // ���������� ���������
	void get_n_info(int n, _element_chart* e); // �������� ������� ���������� n-�� ��������
	void get_t_info(int t, _element_chart* e); // �������� ������� ���������� �������� �� �������� >= t
	void draw(int n, _area2 area, _bitmap* bm); // ���������� 1 �������
	void recovery(); // ���������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _g_graph : public _t_go
{
	std::vector<std::unique_ptr<_basic_curve>> curve; // ������
	bool obn = true; // �������� ��������

	_g_graph();

	uchar type()                     override { return 9; }
	int get_froglif()                override { return 0xF6; } // !!!!!

	void ris2(_trans tr, bool final) override;

private:
	_bitmap bm;
	int size_el = 6; // ������ ��������
	int x_tani = 0; // ���������� ���������� x ��� ��������������
	int v_vib = 0; // �������� ������ ���������

	void draw(_size2i size);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
