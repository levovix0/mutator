#pragma once

#include "mathematics.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr i64 roffer = 20; // ����������� �������, ����������� ������� ( ����� = roffer * 2 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _prices // ������ ������ ����������� � �������� ������
{
	_one_stat buy[roffer];  // ����������� ������� (������� � ������ ���������)
	_one_stat sale[roffer]; // ����������� ������� (������� � ������ ���������)
	time_t    time;         // �����

	void clear()       noexcept { time = 0; } // ����� ������� ������
	bool empty() const noexcept { return (time == 0); } // �������� �� ������� 
	bool operator==(const _prices& p) const noexcept; // ����� �� ����������� ��� ���������
	bool operator!=(const _prices& p) const noexcept; // ����� �� ����������� ��� ���������

	time_t time_to_minute() { return time - (time % 60); } // �������� �������
	i64 time_hour();
	i64 time_minute();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _bit_vector // ������ � ��������� ������� / �������
{
	std::vector<u64> data;
	uchar bit = 64; // ������� ��� ��������� � ��������� �����
	i64 bit_read = 0; // ������� ���� ��� ������

	void push1(u64 a) noexcept; // �������� 1 ���
	void pushn(u64 a, uchar n) noexcept; // �������� n ���
	void pushn1(u64 a) noexcept; // �������� ������������ ���������� ���, 1xxxxxxxx
	u64 pop1() noexcept; // ��������� 1 ���
	u64 popn(uchar n) noexcept; // ��������� n ���
	i64 size() const noexcept { return (i64)data.size() * 64 - (64 - bit); }
	void resize(i64 v);
	void clear() { data.clear(); bit = 64; bit_read = 0; }

	void save(_stack &mem);
	void load(_stack &mem);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _sable_stat // ���������� ���, ������  (� 2 ���� ������, � 3 ���� ��������� ���������� ������)
{
	struct _info_pak // �������������� ���������� ��� ��������
	{
		bool ok = false;   // ���� ������
		int r = 0;     // ����� ������
		int r_pro = 0; // ������ ������
		int r_pok = 0; // ������ �������
	};

	i64 size = 0; // ���������� �������
	_prices back{}; // ��������� ����
	_bit_vector data; // ������ ������
	static constexpr time_t old_dtime = 160; // �������� �������, ����� �������� ���� ��������� �����������
	static constexpr double c_unpak = 0.01; // ���������� ���

	bool add(const _prices& c); // �������� ���� (�����)
	bool read(i64 n, _prices& c, _info_pak* inf = nullptr); // ��������� ���� (�������)
	void save_to_file(wstr fn);
	void load_from_file(wstr fn);
	void clear(); // ������� ��� ������

private:
	std::vector<i64> udata; // ��������� �� ����� ������ ������ ������� step_pak_cc
	std::vector<_one_stat> base_buy; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_sale; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_buy_r; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_sale_r; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	static constexpr i64 step_pak_cc = 100; // ������ �������� ���
	_prices read_cc{}; // ��������� ����������� ����
	i64 read_n = -666; // ����� ��������� ����������� ���
	_info_pak ip_last, ip_n; // �������������� ����������

	bool add0(const _prices& c); // �� ������!
	bool add1(const _prices& c); // ������
	bool add12(const _one_stat* v1, std::vector<_one_stat>& v0, i64 izm);
	bool coding_delta_number(i64 a, i64 b);
	i64  decoding_delta_number(i64 a);
	bool read0(_prices& c);
	bool read1(_prices& c);
	bool read12(_one_stat* v1, std::vector<_one_stat>& v0);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _cdf // ��������� ������ ��� ������ ����� � ���������� ����������� ���
{
	struct _frequency
	{
		i64   first = 0; // ������ ����� ����������
		uchar bit = 0; // ���������� �������������� ���
		u64   prefix = 0; // ������������ ���� (���������� 1)
	};

	struct _mapf
	{
		i64    first = 0;  // ������ ����� ����������
		uchar  bit = 0;  // ���������� �������������� ���
		_mapf* next[2] = {}; // ��������� �����

		~_mapf() { delete next[0]; delete next[1]; }
	};

	std::vector<_frequency> fr; // ������������ ����������
	_mapf frd; // ������ �������������
	_basic_statistics* bst = nullptr; // ��� �����������

	_cdf() = default;
	_cdf(const std::vector<_frequency>& a, _basic_statistics* b = nullptr);

	void clear() { fr.clear(); }
	bool coding(i64 a, _bit_vector& bs) const noexcept; // ������������ ����� � ������� ����� (return false ���� ������)
	i64  decoding(_bit_vector& bs) const noexcept; // ������������ ����� �� �������� ������
	void calc(const _statistics& st, i64 n, i64 min_value, i64 max_value); // n - ���������� ����������
	void to_clipboard(); // ����������� � ������ ������
//	double calc_size1(const _statistics& st); // ������� � ����� (� �������) ����� ������ ���� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _cdf3 // ��������� ������ ��� ������ ������ ���������� ����� � ���������� ����������� ���
{
	struct _mapf
	{
		i64    first = 0;  // ����� ����������
		_mapf* next[2] = {}; // ��������� �����

		~_mapf() { delete next[0]; delete next[1]; }
	};

	i64 start = 0;
	std::vector<u64> prefix; // ������ �������
	_mapf frd; // ������ �������������
	_basic_statistics* bst = nullptr; // ��� �����������

	_cdf3() = default;
	_cdf3(i64 start_, const std::vector<u64>& a, _basic_statistics* b = nullptr);

	void clear() { prefix.clear(); }
	bool coding(i64 a, _bit_vector& bs) const noexcept; // ������������ ����� � ������� ����� (return false ���� ������)
	i64  decoding(_bit_vector& bs) const noexcept; // ������������ ����� �� �������� ������
	void calc(const _statistics& st, i64 min_value, i64 max_value); // ��������� ������ ��������
	void to_clipboard(); // ����������� � ������ ������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline _sable_stat sss;// ������ ����

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
