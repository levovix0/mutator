#include <chrono>

#include "mediator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring mmm1 = L"1";
std::wstring mmm2 = L"2";
std::wstring mmm3 = L"3";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void load_mmm(wstr file_name)
{
	_rjson fs(file_name);
	fs.read("mmm1", mmm1);
	fs.read("mmm2", mmm2);
	fs.read("mmm3", mmm3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _kusok_bukva::cod(ushort* aa, int vaa, wchar_t cc, char nf, i64 nbitt)
{
	if (vaa == 0)
	{
		if (vc < rc)
		{
			c[vc] = cc;
			f[vc] = nf;
			nbit[vc] = nbitt;
			vc++;
		}
		return;
	}
	auto fi = std::lower_bound(dalee.begin(), dalee.end(), *aa);
	i64 n = fi - dalee.begin();
	bool nena = false;
	if (fi == dalee.end())
		nena = true;
	else
		if (*fi != *aa)
			nena = true;
	if (nena)
	{
		_kusok_bukva b;
		b.mask = *aa;
		dalee.insert(fi, b);
	}
	/*	uint n;
		if (!dalee.find_sort(*aa, &n))
		{
			_kusok_bukva b;
			b.mask = *aa;
			dalee.insert(n, b);
		}*/

	dalee[n].cod(aa + 1, vaa - 1, cc, nf, nbitt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_recognize::_recognize()
{
	static const int vf_ = 2; // ���������� �������
	_bitmap bm_[vf_]; // ����� ��� ���������
	bm_[0].set_font(L"MS Sans Serif", false);
	bm_[1].set_font(L"MS Sans Serif", true);
	std::wstring nabor = L"0123456789.,:;-()[]><=����������������������������������������������������������������"
		L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int L = (int)nabor.size();
	std::wstring ss_ = L"0";
	ushort aa[20];
	for (int nf = 0; nf < vf_; nf++)
		for (int i = 0; i < L; i++)
		{
			wchar_t c = nabor[i];
			ss_[0] = c;
			_isize size = bm_[nf].size_text(ss_.data(), 8);
			//			if (c == ' ') ShowMessage(IntToStr((int)size.cx));
			//			if (size.cy != 13) ShowMessage(ss_+" "+IntToStr((int)size.cy));
			if (size.x > 20) return;//���-�� �� ��...
			if ((size.x > bm_[nf].size.x) || (size.y > bm_[nf].size.y))
				bm_[nf].resize({ std::max(size.x, bm_[nf].size.x), std::max(size.y, bm_[nf].size.y) });
			bm_[nf].clear(0);
			bm_[nf].text(0, 0, ss_.data(), 8, 0xffffff, 0);
			ZeroMemory(aa, sizeof(ushort) * size.x);
			for (i64 j = size.y - 1; j >= 0; j--)
			{
				uint* sl = bm_[nf].sl(j);
				for (int i_ = 0; i_ < size.x; i_++) aa[i_] = (aa[i_] << 1) + (sl[i_] > 0);
			}
			int na = 0;
			int ko = (int)size.x - 1;
			while ((aa[na] == 0) && (na < ko)) na++;
			while ((aa[ko] == 0) && (ko > na)) ko--;
			i64 nbit = 0;
			for (int j = na; j <= ko; j++) nbit += bit16(aa[j]);
			bu.cod(aa + na, ko - na + 1, c, nf, nbit);
		}
}

int _recognize::read_okno_soobsenii()
{
	HWND w = FindWindow(0, mmm2.c_str());
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	return 0;
}

int _recognize::read_vnimanie_pokupka()
{
	HWND w = FindWindow(0, L"��������");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 17) || (elem.size() > 18)) return 2; // ������������ ���������� ���������
	if (find_elem(L"�������������-���������������������������") < 0) return 3;
	if (find_elem(L"���������������������") < 0) return 4;
	return 0;
}

int _recognize::read_vnimanie_prodaza()
{
	HWND w = FindWindow(0, L"��������");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 17) || (elem.size() > 18)) return 2; // ������������ ���������� ���������
	if (find_elem(L"�������������-���������������������������") < 0) return 3;
	if (find_elem(L"���������������������") < 0) return 4;
	return 0;
}

int _recognize::read_vvod_zaya()
{
	HWND w = FindWindow(0, L"�� ��: �+ ����� � �� ���� ������");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 20) || (elem.size() > 28)) return 2; // ������������ ���������� ���������
	return 0;
}

struct FindWnd
{
	std::wstring class_name_; // ��� ������
	std::wstring window_name_; // ��� ����
	HWND hwnd_{}; // ��������� ����
};

BOOL CALLBACK PoiskOkna(HWND hwnd, LPARAM lParam)
{
	FindWnd* aa = (FindWnd*)lParam;
	wchar_t str[255];
	GetWindowText(hwnd, str, 255);
	if (aa->window_name_ != str) return TRUE;
	GetClassName(hwnd, str, 255);
	if (aa->class_name_ != str) return TRUE;
	aa->hwnd_ = hwnd;
	return FALSE;


	//FindWnd* aa = (FindWnd*)lParam;
	//wchar_t str[255];
	//wstring s;
	//GetWindowText(hwnd, str, 255);
	//if (aa->window_name_ != str) return TRUE;
	//s = str;
	//GetClassName(hwnd, str, 255);
	//s = s + L" : " + str;
	//MessageBox(0, s.c_str(), L"���", MB_OK | MB_TASKMODAL);

	//if (aa->class_name_ != str) return TRUE;
	//aa->hwnd_ = hwnd;
	//return FALSE;

}

HWND FindSubWindow(HWND w, const wchar_t* classname, const wchar_t* windowname)
{
	//		HWND w2 = FindWindowEx(w, 0, L"HostWindow", 0);
	FindWnd aa;
	aa.class_name_ = classname;
	aa.window_name_ = windowname;
	aa.hwnd_ = 0;
	//	EnumWindows(fnEnumWindowProc, 0);
	EnumChildWindows(w, PoiskOkna, (LPARAM)&aa);
	return aa.hwnd_;
}

int _recognize::read_tablica_zayavok(int a, int& b)
{
	b = 0;
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return 1;
	HWND w2 = FindSubWindow(w, L"InfoMDITableCommon", L"������� ������ �������� �����"); // InfoPriceTable HostWindow
	if (!w2) return 2;
	RECT rr;
	GetWindowRect(w2, &rr);

	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	image.grab_ecran_oo2(w2);
	//	image_.SaveToFile(L"err.bmp");
	find_text13(0xFF0040FF, 10); // ����� ������ 

	std::wstring ss_ = std::to_wstring(a);
	for (uint i = 0; i < elem.size(); i++)
		if (elem[i].s == ss_) b++;
	return 0;
}

bool _recognize::find_window_prices(RECT* rr)
{
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return false;
	HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"�������� [�� ��: �+ ����� � ��] ���������"); // InfoPriceTable HostWindow
	if (!w2) return false;
	GetWindowRect(w2, rr);
	return true;
}

i64 to_int(const std::wstring& s) // �������������� � ����� � �������������� ���������� ��������
{
	i64 r = 0;
	bool znak = false;
	int l = (int)s.size();
	const wchar_t* data = s.data();
	for (int i = 0; i < l; i++)
	{
		ushort delta = data[i] - L'0';
		if (delta <= 9) r = r * 10 + delta;
		if (data[i] == L'-') znak = !znak;
	}
	if (znak) r = -r;
	return r;
}

int _recognize::test_image(_prices* pr)
{
	find_text13(0xFF0000FF); // ����� ������ �������
	if (elem.size() != roffer * 2) return 3;
	i64 pre = 0;
	for (int i = 0; i < roffer; i++)
	{
		i64 a = to_int(elem[i * 2i64].s);
		if (a <= pre) return 4;
		pre = a;
		if ((a < 1) || (a > 65000)) return 5;
		pr->buy[roffer - 1 - i].value = static_cast<ushort>(a);
		a = to_int(elem[i * 2i64 + 1i64].s);
		if ((a < 1) || (a > 2000000000)) return 6;
		pr->buy[roffer - 1 - i].number = static_cast<int>(a);
	}
	find_red_text13(24); // ������� ������ �������
	if (elem.size() != roffer * 2) return 7;
	for (int i = 0; i < roffer; i++)
	{
		i64 a = to_int(elem[i * 2i64].s);
		if (a <= pre) return 8;
		pre = a;
		if ((a < 1) || (a > 65000)) return 9;
		pr->sale[i].value = static_cast<ushort>(a);
		a = to_int(elem[i * 2i64 + 1i64].s);
		if ((a < 1) || (a > 2000000000)) return 10;
		pr->sale[i].number = static_cast<int>(a);
	}
	return 0;
}

int _recognize::read_prices_from_screen(_prices* pr)
{
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return 1;
	HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"�������� [�� ��: �+ ����� � ��] ���������"); // InfoPriceTable HostWindow
	if (!w2) return 2;
	image.clear(0xFFFFFFFF); // �.�. ���� ���� ��������, �� �� ��������
	pr->time = time(0);
	image.grab_ecran_oo2(w2);
	find_text13(0xFF0000FF); // ����� ������ �������
	if (elem.size() != roffer * 2) return 3;
	i64 pre = 0;
	for (i64 i = 0; i < roffer; i++)
	{
		std::wstring swe = elem[i * 2].s;
		i64 a = to_int(swe);
		if (a <= pre) return 4;
		pre = a;
		if ((a < 1) || (a > 65000)) return 5;
		pr->buy[roffer - 1 - i].value = static_cast<ushort>(a);
		a = to_int(elem[i * 2 + 1].s);
		if ((a < 1) || (a > 2000000000)) return 6;
		pr->buy[roffer - 1 - i].number = static_cast<int>(a);
	}
	find_red_text13(24); // ������� ������ �������
	if (elem.size() != roffer * 2) return 7;
	for (i64 i = 0; i < roffer; i++)
	{
		i64 a = to_int(elem[i * 2].s);
		if (a <= pre) return 8;
		pre = a;
		if ((a < 1) || (a > 65000)) return 9;
		pr->sale[i].value = static_cast<ushort>(a);
		a = to_int(elem[i * 2 + 1].s);
		if ((a < 1) || (a > 2000000000)) return 10;
		pr->sale[i].number = static_cast<int>(a);
	}
	return 0;
}

void _recognize::find_red_text13(uint err)
{
	elem.clear();
	i64 rx = image.size.x;
	ushort* lin = new ushort[rx];
	ZeroMemory(lin, sizeof(ushort) * rx);
	for (i64 j = image.size.y - 1; j >= 0; j--)
	{
		uint* sl = image.sl(j);
		i64 first = -1;
		i64 last = -100;
		bool norm = true;
		for (int i = 0; i < rx; i++) {
			uint cc = sl[i];
			uint e2 = (255 - ((cc >> 16) & 255)) + ((cc >> 8) & 255) + (cc & 255);
			lin[i] = (lin[i] << 1) + (e2 <= err);
			if (lin[i])
			{
				if (first < 0)
				{
					first = i;
					norm = false;
				}
				last = i;
				if (lin[i] & 1) norm = true;
				continue;
			}
			if (i - last == 8)
			{ // ����������� �������!!!!
				if (!norm)
				{
					_area_string aa;
					ushort s = 0;
					for (i64 k = first; k <= last; k++) s |= lin[k];
					aa.area = { {first, last + 1}, {j + 1, j + bit16(s) + 1} };
					aa.s = rasp_text(lin + first, last - first + 1);
					elem.push_back(aa);
					ZeroMemory(lin + first, sizeof(ushort) * (last + 1i64 - first));
				}
				first = -1;
			}
		}
	}
	delete[] lin;
}

void _recognize::find_text13(uint c, int err)
{
	int c0 = c & 255;
	int c1 = (c >> 8) & 255;
	int c2 = (c >> 16) & 255;

	elem.clear();
	i64 rx = image.size.x;
	ushort* lin = new ushort[rx];
	ZeroMemory(lin, sizeof(ushort) * rx);
	for (i64 j = image.size.y - 1; j >= 0; j--)
	{
		uint* sl = image.sl(j);
		i64 first = -1;
		i64 last = -100;
		bool norm = true;
		for (int i = 0; i < rx; i++)
		{
			uint cc = sl[i];
			int e0 = (int)(cc & 255) - c0;
			int e1 = (int)((cc >> 8) & 255) - c1;
			int e2 = (int)((cc >> 16) & 255) - c2;
			if (e0 < 0) e0 = -e0;
			if (e1 < 0) e1 = -e1;
			if (e2 < 0) e2 = -e2;
			lin[i] = (lin[i] << 1) + ((e0 + e1 + e2) <= err);
			if (lin[i])
			{
				if (first < 0)
				{
					first = i;
					norm = false;
				}
				last = i;
				if (lin[i] & 1) norm = true;
				continue;
			}
			if (i - last == 8)
			{ // ����������� �������!!!!
				if (!norm)
				{
					_area_string aa;
					ushort s = 0;
					for (i64 k = first; k <= last; k++) s |= lin[k];
					aa.area = { {first, last + 1}, {j + 1, j + bit16(s) + 1} };
					aa.s = rasp_text(lin + first, last - first + 1);
					elem.push_back(aa);
					ZeroMemory(lin + first, sizeof(ushort) * (last + 1i64 - first));
				}
				first = -1;
			}
		}
	}
	delete[] lin;
}

void _recognize::find_text13(uint c)
{
	elem.clear();
	i64 rx = image.size.x;
	ushort* lin = new ushort[rx];
	ZeroMemory(lin, sizeof(ushort) * rx);
	for (i64 j = image.size.y - 1; j >= 0; j--)
	{
		uint* sl = image.sl(j);
		i64 first = -1;
		i64 last = -100;
		bool norm = true;
		for (int i = 0; i < rx; i++) {
			lin[i] = (lin[i] << 1) + (sl[i] == c);
			if (lin[i])
			{
				if (first < 0)
				{
					first = i;
					norm = false;
				}
				last = i;
				if (lin[i] & 1) norm = true;
				continue;
			}
			if (i - last == 8)
			{ // ����������� �������!!!!
				if (!norm)
				{
					_area_string aa;
					ushort s = 0;
					for (i64 k = first; k <= last; k++) s |= lin[k];
					aa.area = { {first, last + 1}, {j + 1, j + bit16(s) + 1} };
					aa.s = rasp_text(lin + first, last - first + 1);
					elem.push_back(aa);
					ZeroMemory(lin + first, sizeof(ushort) * (last + 1i64 - first));
				}
				first = -1;
			}
		}
	}
	delete[] lin;
}

std::wstring _recognize::rasp_text(ushort* aa, i64 vaa)
{
	std::wstring s;
	wchar_t* s2_ = new wchar_t[vaa + 1i64];
	ushort b = 0;
	for (int k = 0; k < vaa; k++) b |= aa[k];
	i64 vb = bit16(b);
	i64 bestbit = 0;
	for (i64 ii = 0; ii < 3; ii++)
	{
		i64 sm = 12 - vb - ii;
		if (sm < -1) break;
		int L = 0;
		int i = 0;
		i64 nbit = 0;
		while (i < vaa)
		{
			if (aa[i] == 0)
			{
				i++;
				continue;
			}
			wchar_t c = 0;
			int rc = 0;
			i64 bbb = 0;
			_kusok_bukva* bb = &bu;
			for (int j = i; j < vaa; j++)
			{
				ushort m = (sm >= 0) ? (aa[j] << sm) : ((aa[j] >> (-sm)));

				auto fi = std::lower_bound(bb->dalee.begin(), bb->dalee.end(), m);
				if (fi == bb->dalee.end()) break;
				if (*fi != m) break;
				bb = &*fi;
				//				uint n;
				//				if (!bb->dalee.find_sort(m, &n)) break;
				//				bb = &bb->dalee.data[n];

				if (bb->vc)
				{
					c = bb->c[0];
					bbb = bb->nbit[0];
					rc = j - i + 1;
				}
			}
			if (rc == 0)
			{
				i++;
				continue;
				// break;
			}
			i += rc;
			s2_[L++] = c;
			nbit += bbb;
		}
		//if (L > s.size())
		//{
		//	s2_[L] = 0;
		//	s = s2_;
		//}
		if (nbit > bestbit)
		{
			s2_[L] = 0;
			s = s2_;
			bestbit = nbit;
		}

	}
	delete[] s2_;
	return s;
}

int _recognize::find_elem(std::wstring_view s)
{
	int l = (int)elem.size();
	for (int i = 0; i < l; i++)
		if (elem[i].s == s) return i;
	return -1;
}

int _recognize::find_elem_kusok(wstr s)
{
	int l = (int)elem.size();
	for (int i = 0; i < l; i++)
		if (elem[i].s.find(s) != std::string::npos) return i;
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

