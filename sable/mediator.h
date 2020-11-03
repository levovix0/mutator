#pragma once

#include "sable_stat.h"
#include "tetron.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _area_string // ������� �� �������
{
	_iarea area;    // �������
	std::wstring s; // ������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _kusok_bukva // ���� ������ ����������� ��������
{
	static const int rc = 4; // ����������� ���������� ��������
	ushort mask = 0; // ������� �����
	std::vector<_kusok_bukva> dalee; // ��������� ������� !!!! ������ ��������, �.�. ���������� ����������
	wchar_t c[rc]{}; // ��������� �������
	char f[rc]{}; // ��������������� ������
	i64 nbit[rc]{}; // ���������� ��� � �������
	int vc = 0; // ���������� ������������

	void cod(ushort* aa, int vaa, wchar_t cc, char nf, i64 nbitt);   // �����������
	bool operator!=(ushort a) const noexcept { return (mask != a); } // ��������
	bool operator==(ushort a) const noexcept { return (mask == a); } // ��������
	bool operator< (ushort a) const noexcept { return (mask < a); } // ��������
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _recognize // ������������� � ������
{
	_bitmap image; // �������� ��� �������������
	POINT offset = { 0,0 }; // �������� ��������
	std::vector<_area_string> elem; // ��������

	_recognize(); // �����������
	int read_prices_from_screen(_prices* pr); // ��������� ���� � ������ (0 - ��� ������)
	int read_vvod_zaya(); // ����� � ���������� ���� ����� ������. ��������� ������
	int read_vnimanie_prodaza(); // ����� � ���������� ���� ������������ �������. ��������� ������
	int read_vnimanie_pokupka(); // ����� � ���������� ���� ������������ �������. ��������� ������
	int read_tablica_zayavok(int a, int& b); // ���������� ����� � ������� ������
	int read_okno_soobsenii(); // ����� � ���������� ���� ���������. ��������� ������
	void find_text13(uint c); // ����� ����� ������� 13 � ������ ������
	void find_text13(uint c, int err); // err - ����������� !!! ���� ��������, �������� ������� 2 ������ !!!
	void find_red_text13(uint err); // ����� ������� ����� ������� 13
	bool find_window_prices(RECT* rr); // ���������� ���� ���
	int find_elem(std::wstring_view s); // ����� ����� ����� ���������
	int find_elem_kusok(wstr s); // ����� ����� ������ ����� ���������
	int test_image(_prices* pr); // �������� ����� �� ������� ��������

private:
	_kusok_bukva bu; // ������ �������� ��� �������������

	std::wstring rasp_text(ushort* aa, i64 vaa); // ���������� �����
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline _recognize recognize;
inline bool zamok_pokupki = false; // ������� ����������

void load_mmm();
void buy_stock(_tetron* tt, bool buy);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
