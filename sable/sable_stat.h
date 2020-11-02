#pragma once

#include "basic.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr i64 roffer = 20; // ����������� �������, ����������� ������� ( ����� = roffer * 2 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _offer // �����������
{
	ushort c; // ����
	int k; // ����������

	bool operator!=(_offer p) const noexcept { return (c != p.c) || (k != p.k); }
};

struct _one_stat // ������� ����������
{
	i64 value;   // �������� / ����
	i64 number;  // ����������

	void operator=(_offer a) noexcept { value = a.c; number = a.k; }
	bool operator!=(_one_stat a) const noexcept { return ((value != a.value) || (number != a.number)); }
	bool operator!=(_offer a) const noexcept { return ((value != a.c) || (number != a.k)); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _date_time // ���� � �����
{
	uchar month, day, hour, minute, second; // (�����+���), ����, ���, ���, ���

	operator int() const noexcept; // ������������� � ����� ����� (� ��������)
	int to_minute(); // ������������� � ����� ����� ��� ������ (� ��������)
	void operator=(int a); // ��������� �����
	void now(); // ��������� ������� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _prices // ������ ������ �����������
{
	_offer pok[roffer]; // ����������� ������� (������� � ������ ���������)
	_offer pro[roffer]; // ����������� ������� (������� � ������ ���������)
	_date_time time;    // �����

	void clear()       noexcept { time.second = 200; } // ����� ������� ������
	bool empty() const noexcept { return (time.second == 200); } // �������� �� �������
	bool operator==(const _prices& p) const noexcept; // ����� �� ����������� ��� ���������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _prices2 // ������ ������ ����������� � �������� ������
{
	_one_stat buy[roffer];  // ����������� ������� (������� � ������ ���������)
	_one_stat sale[roffer]; // ����������� ������� (������� � ������ ���������)
	time_t    time;         // �����

	_prices2() = default;
	_prices2(const _prices& a);

	void clear()       noexcept { time = 0; } // ����� ������� ������
	bool empty() const noexcept { return (time == 0); } // �������� �� ������� 
	bool operator==(const _prices2& p) const noexcept; // ����� �� ����������� ��� ���������
	bool operator!=(const _prices& p) const noexcept;
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
	_prices2 last_cc{}; // ��������� ����
	_bit_vector data; // ������ ������
	static constexpr time_t old_dtime = 160; // �������� �������, ����� �������� ���� ��������� �����������
	static constexpr double c_unpak = 0.01; // ���������� ���

	bool add(const _prices2& c); // �������� ���� (�����)
	bool read(i64 n, _prices2& c, _info_pak* inf = nullptr); // ��������� ���� (�������)
	void save_to_file(wstr fn);
	void load_from_file(wstr fn);

private:
	std::vector<i64> udata; // ��������� �� ����� ������ ������ ������� step_pak_cc
	std::vector<_one_stat> base_buy; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_sale; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_buy_r; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	std::vector<_one_stat> base_sale_r; // ���� ������� ��� ������ (������ 20 - ��������� ����)
	static constexpr i64 step_pak_cc = 100; // ������ �������� ���
	_prices2 read_cc{}; // ��������� ����������� ����
	i64 read_n = -666; // ����� ��������� ����������� ���
	_info_pak ip_last, ip_n; // �������������� ����������

	bool add0(const _prices2& c); // �� ������!
	bool add1(const _prices2& c); // ������
	bool add12(const _one_stat* v1, std::vector<_one_stat>& v0, i64 izm);
	bool coding_delta_number(i64 a, i64 b);
	i64  decoding_delta_number(i64 a);
	bool read0(_prices2& c);
	bool read1(_prices2& c);
	bool read12(_one_stat* v1, std::vector<_one_stat>& v0);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _basic_statistics // ������� ����������� ���������� � ������
{
	std::vector<i64> data;
	i64 start = 0;

	void push(i64 x); // �������� ����� � ����������
	i64 number() const noexcept;
	i64 number(i64 be, i64 en) const noexcept; // ���������� � ��������� [be,en)
	i64 operator[](i64 x) const noexcept;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _statistics // ������ ����������
{
	std::vector<_one_stat> data;
	typedef std::vector<_one_stat>::iterator _it;
	typedef std::vector<_one_stat>::const_iterator _cit;

	_statistics() = default;
	_statistics(const _basic_statistics& a) { *this = a; }

	void clear() { data.clear(); }
	i64 min_value() const noexcept { return (data.empty()) ? 0 : data.front().value; }
	i64 max_value() const noexcept { return (data.empty()) ? 0 : data.back().value; }
	i64 number(_cit be, _cit en) const noexcept; // ���������� � ��������� [be,en)
	i64 number(i64 be, i64 en) const noexcept; // ���������� � ��������� [be,en)
	i64 number() const noexcept { return number(data.begin(), data.end()); } // ����� ����������

	i64 first_zero(); // ����� ������� �������� �������� ������� �� start (-1 ���� �� �������)
	i64 number_not_zero() { return data.size(); } // ���������� �������� � ��������� �����������
	double arithmetic_size(_it be, _it en); // �������������� ������ � �����
	double arithmetic_size() { return arithmetic_size(data.begin(), data.end()); }

	void operator=(const _basic_statistics& a);
	void operator+=(const _statistics& a);
	bool operator==(const _statistics& a) const noexcept;

	i64 operator[](i64 n) const noexcept; // value -> number

	_matrix to_matrix(); // ��� ������� - x � y
	_matrix to_matrix(i64 mi, i64 ma); // ��� ������� - x � y
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
