﻿#include "t_function.h"
#include "go.h"
#include "mjson.h"
#include "set.h"

constexpr wchar_t ss_file[]  = L"..\\..\\set\\baza.cen";
constexpr wchar_t mmm_file[] = L"..\\..\\set\\mmm.txt";
constexpr _prices cena_zero_ = { {}, {}, { 1,1,1,1,1 } };

_super_stat      ss;                  // сжатые цены
_g_graph*        graph     = nullptr; // график

_nervous_oracle *oracle    = nullptr; // оракул
_mctds_candle   *sv        = nullptr;
_oracle3        *o3        = nullptr;

_recognize       recognize;

std::wstring mmm1 = L"1";
std::wstring mmm2 = L"2";
std::wstring mmm3 = L"3";

int kkk2 = 13; // количество продаваемых акций

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void load_mmm(std::filesystem::path file_name)
{
	_rjson fs(file_name);
	fs.read("mmm1", mmm1);
	fs.read("mmm2", mmm2);
	fs.read("mmm3", mmm3);
}

void fun13(_tetron* tt0, _tetron* tt, u64 flags)
{
	static bool first = true; if (!first) return; first = false;
	ss.load_from_file(exe_path.wstring() + ss_file);
	if (!graph) return;
	if (!graph->find1<_g_scrollbar>(flag_part))
	{
		_g_scrollbar* sb = new _g_scrollbar;
		sb->vid = 2;
		graph->add_flags(sb, flag_sub_go + flag_part + (flag_run << 32));
	}
	constexpr bool ora3 = false;
	sv = new _mctds_candle;
	oracle = new _nervous_oracle;
	o3 = (ora3) ? new _oracle3 : nullptr;
	graph->curve.push_back(std::unique_ptr<_basic_curve>(sv));
	graph->curve.push_back(std::unique_ptr<_basic_curve>(oracle));
	if (ora3) graph->curve.push_back(std::unique_ptr<_basic_curve>(o3));
	sv->recovery();
	oracle->recovery();
	if (ora3) o3->recovery();
	graph->cha_area();
	graph->obn = true;
	load_mmm(exe_path.wstring() + mmm_file);
}

void fun15(_tetron* tt0, _tetron* tt, u64 flags)
{
	for (_frozen i(n_timer1000, flag_run); i; i++)
	{
		_t_function* f = *i;
		if (!f) continue;
		if (f->a == 16) delete f;
	}
	_g_button* b = *tt0;
	if (b->checked) n_timer1000->add_flags(new _t_function(16), flag_run);
}

bool can_trade     = false; // разрешение на торговлю
bool zamok_pokupki = false; // простой блокиратор
int popitok_prodaz = 2;     // сколько раз можно купить/продать
int gotovo_prodaz  = 0;     // сколько сделок
int vrema_prodat   = 0;     // время когда нужно продать

void fun16(_tetron* tt0, _tetron* tt, u64 flags)
{
	if (zamok_pokupki) return;
	_prices a;
	int ok = recognize.read_prices_from_screen(&a);
	if (ok != 0)
	{
		/*#ifdef DEBUG_MMM
				wstring fn = exe_path + L"errorscr.bmp";
				if (!FileExists(fn.c_str()))
				{
					recognize.image_.SaveToFile(fn.c_str());
					ofstream file(exe_path + L"errorscr.txt");
					file << ok;
					file.close();
				}
		#endif // DEBUG_MMM*/
		return;
	}
	ss.add(a);
	sv->recovery();
	oracle->recovery();
	if (o3) o3->recovery();

	graph->run(nullptr, graph, flag_run);
	// всякие проверки на начало покупки !!!!

	if (!can_trade) return;
	if (oracle->zn.size() < 10) return;
	if (oracle->zn.back().time + 60 != a.time.to_minute()) return;


	if (gotovo_prodaz & 1) // была покупка, но небыло продажи
	{
		if ((a.time >= vrema_prodat) || ((a.time.hour == 18) && (a.time.minute > 30)) || oracle->get_latest_events(oracle->zn.size() - 1).stop())
		{
			/*			int b;
						recognize.ReadTablicaZayavok(0, b);
						if (b != gotovo_prodaz)// лажа, не все купилось, прекратить
						{
							gotovo_prodaz = 0;
							popitok_prodaz = 0;
							return;
						}*/
			zamok_pokupki = true;
			gotovo_prodaz++;
			_t_function* fu = new _t_function(36);
			fu->run(0, fu, flag_run);
		}
		return;
	}

	if (popitok_prodaz < 1) return;
	if (a.time.hour >= 18) return; // слишком поздно
	int ti = oracle->get_latest_events(oracle->zn.size() - 1).start();


	if (ti == 0) return;
	// купить акции
/*	int b;
	recognize.ReadTablicaZayavok(0, b);
	if (b != gotovo_prodaz)// лажа, не все купилось, прекратить
	{
		gotovo_prodaz = 0;
		popitok_prodaz = 0;
		return;
	}*/
	zamok_pokupki = true;
	vrema_prodat = a.time + ti * 60;
	popitok_prodaz--;
	gotovo_prodaz++;
	_t_function* fu = new _t_function(35);
	fu->run(0, fu, flag_run);
}

void fun19(_tetron* tt0, _tetron* tt, u64 flags)
{
	_g_button* b = *tt0;
	can_trade = b->checked;
}

void fun20(_tetron* tt0, _tetron* tt, u64 flags)
{
	if (zamok_pokupki) return;
	if (mmm1 == L"1") load_mmm(exe_path.wstring() + mmm_file);
	zamok_pokupki = true;
	_t_function* fu = new _t_function(35);
	fu->run(0, fu, flag_run);
}

void fun21(_tetron* tt0, _tetron* tt, u64 flags)
{
	if (zamok_pokupki) return;
	if (mmm1 == L"1") load_mmm(exe_path.wstring() + mmm_file);
	zamok_pokupki = true;
	_t_function* fu = new _t_function(36);
	fu->run(0, fu, flag_run);
}

void fun22(_tetron* tt0, _tetron* tt, u64 flags)
{
	graph->size_el++;
	graph->obn = true;
	graph->cha_area();
}

void fun30(_tetron* tt0, _tetron* tt, u64 flags)
{
	if (graph->size_el > 1) graph->size_el--;
	graph->obn = true;
	graph->cha_area();
}

void MouseMoveClick(i64 x, i64 y)
{
	SetCursorPos((int)x, (int)y);
	//	Mouse->CursorPos = a;
	//	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0,GetMessageExtraInfo());
	//	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0,GetMessageExtraInfo());
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//down press up
	//		keybd_event('8', 0, 0, 0);
	//		keybd_event('8', 0, KEYEVENTF_KEYUP, 0);

}

void MouseClick()
{
	//	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0,GetMessageExtraInfo());
	//	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0,GetMessageExtraInfo());
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

int bad_string_to_int(std::wstring& s)
{
	int r = 0;
	int ii;
	int l = (int)s.size();
	wstr ss = s.data();
	for (ii = 0; ii < l; ii++) if ((ss[ii] >= L'0') && (ss[ii] <= L'9')) break;
	for (int i = ii; i < l; i++)
		if ((ss[i] >= L'0') && (ss[i] <= L'9'))
			r = r * 10 + (ss[i] - L'0');
		else
			break;
	return r;
}

void buy_stock(_tetron* tt, bool buy)
{
	int otst_20 = 1;                                                      // 20 >= x >= 1, 1 - лучшая цена
	static int KKK;
	static bool win8 = false;
	static __int64 n = 0;
	// шаг 01: внедрение и инициализация
	if (tt->link.empty())
	{
		n_timer250->add_flags(tt, flag_run);
		return;
	}
	// пауза
	if (n < 1)
	{
		n = n + 1;
		return;
	}
	// шаг 01: двойной клик по цене
	if (n == 1)
	{
		n = 1000;
		RECT rr;
		if (!recognize.find_window_prices(&rr)) return;
		if (buy)
			MouseMoveClick(rr.left + 20ll, 6ll + rr.top + 15 * (4ll - otst_20));
		else
			MouseMoveClick(rr.left + 20ll, 6ll + rr.top + 15 * (37ll + otst_20));
		MouseClick();
		n = 2;
		return;
	}
	// шаг 02: поиск окна и нажатие на код клиента
	if (n < 13)
	{
		int er = recognize.read_vvod_zaya();
		if (er)
		{
			n = n + 1;
			if (n == 13)
			{
				n = 1000;
				//        ra2.kar_->bitmap_->SaveToFile("xxx.bmp");
				//				MessageBox(0, (L"не найдено окно, ошибка " + to_wstring(er)).data(), L"упс..", MB_OK | MB_TASKMODAL);
			}
			return;
		}
		n = 1000;
		int n_pok = recognize.find_elem(L"Покупка");
		int n_pro = recognize.find_elem(L"Продажа");
		if ((n_pok < 0) || (n_pro < 0))
		{
			//			MessageBox(0, L"не найдена покупка/продажа", L"упс..", MB_OK | MB_TASKMODAL);
			return;
		}
		uint c = recognize.image.sl(recognize.elem[n_pok].area.y.min)[recognize.elem[n_pok].area.x.min - 1];
		uchar* cc = (uchar*)& c;
		bool cfpok = (cc[0] != cc[1]) || (cc[0] != cc[2]); // не серый цвет
		c = recognize.image.sl(recognize.elem[n_pro].area.y.min)[recognize.elem[n_pro].area.x.min - 1];
		cc = (uchar*)& c;
		bool cfpro = (cc[0] != cc[1]) || (cc[0] != cc[2]); // не серый цвет
//		bool cfpok = (recognize.image.sl(recognize.elem[n_pok].area.y.min)[recognize.elem[n_pok].area.x.min - 1] != recognize.image.data[0]);
//		bool cfpro = (recognize.image.sl(recognize.elem[n_pro].area.y.min)[recognize.elem[n_pro].area.x.min - 1] != recognize.image.data[0]);
		if (cfpok == cfpro)
		{
			//			MessageBox(0, L"одинаковые цвета покупка/продажа", L"упс..", MB_OK | MB_TASKMODAL);
			return;
		}
		if (buy)
		{
			if (!cfpok)
			{
				//				MessageBox(0, L"НЕ покупка", L"упс..", MB_OK | MB_TASKMODAL);
				return;
			}
		}
		else
		{
			if (!cfpro)
			{
				//				MessageBox(0, L"НЕ продажа", L"упс..", MB_OK | MB_TASKMODAL);
				return;
			}

		}
		int kk = recognize.find_elem(L"КодКлиента");
		int kk2 = recognize.find_elem(L"Поручение");
		if ((kk < 0) || (kk2 < 0))
		{
			//			MessageBox(0, L"странно нет кода клиента", L"упс..", MB_OK | MB_TASKMODAL);
			return;
		}
		if (abs(recognize.elem[kk].area.x.min - recognize.elem[kk2].area.x.min) < 5) win8 = true;
		if (win8)
			MouseMoveClick(recognize.elem[kk].area.x.min + 260 + recognize.offset.x, recognize.elem[kk].area.y.min + 3 + recognize.offset.y);
		else
			MouseMoveClick(recognize.elem[kk].area.x.min + 110 + recognize.offset.x, recognize.elem[kk].area.y.min + 30 + recognize.offset.y);
		n = 13;
		return;
	}
	// шаг 03: выбор кода клиента 
	if (n == 13)
	{
		int kk = recognize.find_elem(L"КодКлиента");
		if (kk < 0)
		{
			n = 1000;
			return;
		}
		if (win8)
			MouseMoveClick(recognize.elem[kk].area.x.min + 220 + recognize.offset.x, recognize.elem[kk].area.y.min + 20 + recognize.offset.y);
		else
			MouseMoveClick(recognize.elem[kk].area.x.min + 70 + recognize.offset.x, recognize.elem[kk].area.y.min + 47 + recognize.offset.y);
		n = 14;
		return;
	}
	// шаг 04: щелканье кода клиента
	if (n == 14)
	{
		int kk = recognize.find_elem(L"КодКлиента");
		if (kk < 0)
		{
			n = 1000;
			return;
		}
		if (win8)
			MouseMoveClick(recognize.elem[kk].area.x.min + 220 + recognize.offset.x, recognize.elem[kk].area.y.min + 3 + recognize.offset.y);
		else
			MouseMoveClick(recognize.elem[kk].area.x.min + 70 + recognize.offset.x, recognize.elem[kk].area.y.min + 30 + recognize.offset.y);
		n = 15;
		return;
	}
	// шаг 05: поиск кода клиента, щелчок по количеству
	if (n == 15)
	{
		int er = recognize.read_vvod_zaya();
		if (er)
		{
			n = 1000;
			return;
		}
		if (recognize.find_elem(mmm1) < 0)
		{
			n = 1000;
			return;
		}
		int kk = recognize.find_elem_kusok(L"Кол-во");
		if (kk < 0)
		{
			n = 1000;
			return;
		}
		if (win8)
			MouseMoveClick(recognize.elem[kk].area.x.min + 100 + recognize.offset.x, recognize.elem[kk].area.y.min + 3 + recognize.offset.y);
		else
			MouseMoveClick(recognize.elem[kk].area.x.min + 70 + recognize.offset.x, recognize.elem[kk].area.y.min + 30 + recognize.offset.y);
		n = 16;
		return;
	}
	// шаг 06: ввод количества
	if (n == 16)
	{
		// выделение
		keybd_event(0x11, 0, 0, 0);
		keybd_event('A', 0, 0, 0);
		keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(0x11, 0, KEYEVENTF_KEYUP, 0);
		// удаление
		keybd_event(0x2E, 0, 0, 0);
		keybd_event(0x2E, 0, KEYEVENTF_KEYUP, 0);
		if (!win8)
		{
			// определение количества
			int kmax = recognize.find_elem_kusok(L"mах:");
			int klot = recognize.find_elem_kusok(L"Кол-во(лот");
			if ((kmax < 0) || (klot < 0))
			{
				n = 1000;
				return;
			}
			kmax = bad_string_to_int(recognize.elem[kmax].s);
			klot = bad_string_to_int(recognize.elem[klot].s);
			if (klot == 0)
			{                                                                 // на всякий случай
				n = 1000;
				return;
			}
		}
		//		KKK = KKK2 / klot;
		KKK = kkk2;
		if (KKK == 0) /*||(KKK > kmax)*/
		{                                                                 // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			n = 1000;
			return;
		}
		// ввод количества
		std::string s = std::to_string(KKK);
		for (int i = 0; i < s.size(); i++)
		{
			keybd_event(s[i], 0, 0, 0);
			keybd_event(s[i], 0, KEYEVENTF_KEYUP, 0);
		}
		n = 17;
		return;
	}
	// шаг 07: проверка и нажатие на Да
	if (n == 17)
	{
		int er = recognize.read_vvod_zaya();
		if (er)
		{
			n = 1000;
			return;
		}
		if ((recognize.find_elem(mmm1) < 0) || (recognize.find_elem(std::to_wstring(KKK).c_str()) < 0))
		{
			n = 1000;
			return;
		}
		int kk = recognize.find_elem(L"Да");
		if (kk < 0)
		{
			n = 1000;
			return;
		}
		SetCursorPos((int)(recognize.elem[kk].area.x.min + 20 + recognize.offset.x), (int)(recognize.elem[kk].area.y.min + 5 + recognize.offset.y));


		MouseClick();                                                     // НЕЛЬЗЯ
		n = 18;
		return;
	}
	if (n < 29)
	{
		int er = (buy) ? recognize.read_vnimanie_pokupka() : recognize.read_vnimanie_prodaza();
		if (er)
		{
			n = n + 1;
			if (n == 29)
			{
				n = 1000;
				//        ra2.kar_->bitmap_->SaveToFile("xxx.bmp");
				//				MessageBox(0, (L"не найдено окно внимание, ошибка " + to_wstring(er)).data(), L"упс..", MB_OK | MB_TASKMODAL);
			}
			return;
		}
		int kk = recognize.find_elem(L"ОК");
		if (kk < 0)
		{
			n = 1000;
			return;
		}
		SetCursorPos((int)(recognize.elem[kk].area.x.min + 20 + recognize.offset.x), (int)(recognize.elem[kk].area.y.min + 5 + recognize.offset.y));
		MouseClick();                                                     // НЕЛЬЗЯ
		n = 29;
		return;
	}
	if (n < 40)
	{
		int er = recognize.read_okno_soobsenii();
		if (er)
		{
			n = n + 1;
			if (n == 40)
			{
				n = 1000;
				//        ra2.kar_->bitmap_->SaveToFile("xxx.bmp");
				//				MessageBox(0, (L"не найдено окно сообщений, ошибка " + to_wstring(er)).data(), L"упс..", MB_OK | MB_TASKMODAL);
			}
			return;
		}
		SetCursorPos((int)(recognize.offset.x + recognize.image.size.x - 23), (int)(recognize.offset.y - 15));
		MouseClick();                                                     // НЕЛЬЗЯ
		n = 40;
		return;
	}
	delete tt;
	n = 0;
	zamok_pokupki = false;
}

void fun35(_tetron* tt0, _tetron* tt, u64 flags)
{
	buy_stock(tt, true);
}

void fun36(_tetron* tt0, _tetron* tt, u64 flags)
{
	buy_stock(tt, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _date_time::now()
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	month  = t.wMonth + (t.wYear - 2017) * 12;
	day    = (uchar)t.wDay;
	hour   = (uchar)t.wHour;
	minute = (uchar)t.wMinute;
	second = (uchar)t.wSecond;
}

void _date_time::operator =(int a)
{
	month  = (uchar)(a / 2764800L);
	day    = (uchar)((a -= month * 2764800L) / 86400L);
	hour   = (uchar)((a -= day * 86400L) / 3600L);
	minute = (uchar)((a -= hour * 3600L) / 60L);
	second = (uchar)(a - minute * 60L);
}

int _date_time::to_minute()
{
	return (((month * 32L + day) * 24L + hour) * 60L + minute) * 60L;
}

_date_time::operator int()
{
	return (((month * 32L + day) * 24L + hour) * 60L + minute) * 60L + second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _super_stat::save_to_file(wstr fn)
{
	_stack mem;
	mem << data;
	mem << (int)size;
	mem.push_data(&last_cc, sizeof(last_cc));
	mem << u_dd2;
	mem.save_to_file(fn);
}

void _super_stat::load_from_file(std::wstring_view fn)
{
	_stack mem;
	if (!mem.load_from_file(fn)) return;
	mem >> data;
	int sisi;
	mem >> sisi; // !!! пересохранить на 64
	size = sisi;
	mem >> last_cc;
	mem >> u_dd2;
	read_cc = cena_zero_;
	read_n = -1;
	ip_last.ok = false;
	ip_n.ok = false;
}

void _super_stat::clear()
{
	data.clear();
	u_dd2.clear();
	last_cc = cena_zero_;
	read_cc = cena_zero_;
	size = 0;
	read_n = -1;
	ip_last.ok = false;
	ip_n.ok = false;
}

void _super_stat::otgruzka(int rez, int Vrez, int* deko)
{
	if (Vrez == 0) return;
	uchar a = (rez << 6) + Vrez - 1;
	data << a;
	if (rez == 1)
		for (int i = 0; i < Vrez; i++)
		{
			char b = deko[i];
			data << b;
		}
	if (rez == 2)
		for (int i = 0; i < Vrez; i++)
		{
			short b = deko[i];
			data << b;
		}
	if (rez == 3)
		for (int i = 0; i < Vrez; i++)
			data.push_int24(deko[i]);
}

void _super_stat::read(i64 n, _prices& c, _info_pak* inf)
{
	if (inf) inf->ok = false;
	if ((n < 0) || (n >= size)) return;
	if (n == read_n)
	{
		c = read_cc;
		if (inf)* inf = ip_n;
		return;
	}
	if (n == size - 1)
	{
		if (inf)* inf = ip_last;
		c = last_cc;
		return;
	}
	if (read_n + 1 != n)
	{
		i64 k = n / step_pak_cc;
		if ((read_n > n) || (read_n <= k * step_pak_cc - 1))
		{
			data.adata = u_dd2[k];
			read_n = k * step_pak_cc - 1;
		}
		while (read_n < n) read(read_n + 1, c, inf);
		return;
	}
	if (n % step_pak_cc == 0)
	{
		read_cc = cena_zero_;
		ip_n.ok = false;
	}
	else
		ip_n.ok = true;
	size_t aa0 = data.adata;
	uchar dt2;
	data >> dt2;
	if (dt2 < 255)
		c.time = (int)read_cc.time + dt2;
	else
		data >> c.time;
	uchar a;
	data >> a;
	int delta = a & 7;
	int delta2 = a >> 3;
	if (delta2 == 31)
		data >> c.pok[0].c;
	else
		c.pok[0].c = read_cc.pok[0].c + delta2 - 15;
	if (delta == 7)
	{
		data >> a;
		delta = a;
	}
	c.pro[0].c = c.pok[0].c + delta + 1;
	size_t aa1 = data.adata;
	// декодирование продажи
	for (int j = c.pro[0].c, n2 = 0, n = 0; n2 < rceni;)
	{
		data >> a;
		int rez = a >> 6;
		int Vrez = (a & 63) + 1;
		for (int i = 1; i <= Vrez; i++)
		{
			int kk;
			if (rez == 0)
			{
				kk = 0;
			}
			else if (rez == 1)
			{
				char x;
				data >> x;
				kk = x;
			}
			else if (rez == 2)
			{
				short x;
				data >> x;
				kk = x;
			}
			else
			{
				data.pop_int24(kk);
			}
			while ((j > read_cc.pro[n].c) && (n < rceni - 1)) n++;//
			int kk3 = (j == read_cc.pro[n].c) ? read_cc.pro[n].k : 0;//
			int kk2 = kk + kk3;//
			if (kk2 > 0)
			{
				if (n2 >= rceni)
				{
					throw 1;
				}
				c.pro[n2].c = j;
				c.pro[n2].k = kk2;
				n2++;
			}
			j++;
		}
	}
	size_t aa2 = data.adata;
	// декодирование покупки
	for (int j = c.pok[0].c, n2 = 0, n = 0; n2 < rceni;)
	{
		data >> a;
		int rez = a >> 6;
		int Vrez = (a & 63) + 1;
		for (int i = 1; i <= Vrez; i++)
		{
			int kk;
			if (rez == 0)
			{
				kk = 0;
			}
			else if (rez == 1)
			{
				char x;
				data >> x;
				kk = x;
			}
			else if (rez == 2)
			{
				short x;
				data >> x;
				kk = x;
			}
			else
			{
				data.pop_int24(kk);
			}
			while ((j < read_cc.pok[n].c) && (n < rceni - 1)) n++;//
			int kk3 = (j == read_cc.pok[n].c) ? read_cc.pok[n].k : 0;//
			int kk2 = kk + kk3;//
			if (kk2 > 0)
			{
				if (n2 >= rceni)
				{
					throw 1;
				}
				c.pok[n2].c = j;
				c.pok[n2].k = kk2;
				n2++;
			}
			j--;
		}
	}
	read_cc = c;
	read_n = n;

	ip_n.r = int(data.adata - aa0);
	ip_n.r_pro = int(aa2 - aa1);
	ip_n.r_pok = int(data.adata - aa2);
	if (inf)* inf = ip_n;
}

void _super_stat::add(_prices& c)
{
	// настройка, чтобы наверняка не совпало 
	int delta = c.pro[0].c - c.pok[0].c - 1;
	if (delta > 255) return; // !!!!!!!!!!!!!!!!!!!!
	//  *TEMP.Add() = c;
	if (size % step_pak_cc == 0)
	{
		ip_last.ok = false;
		last_cc = cena_zero_;
		u_dd2.push_back((int)data.size);
	}
	else
		ip_last.ok = true;
	size++;
	// кодирование времени
	size_t aa0 = data.size;
	int dt = (int)c.time - (int)last_cc.time;
	if (dt < 0) dt = 0; // время может идти назад!
	if (dt >= 255)
	{
		uchar dt2 = 255;
		data << dt2;
		data << c.time;
	}
	else
	{
		uchar dt2 = dt;
		data << dt2;
	}
	// кодирование прослойки
	uchar a = (delta <= 6) ? delta : 7;
	int delta2 = c.pok[0].c - last_cc.pok[0].c;
	// 0..30   31 +2
	if ((delta2 >= -15) && (delta2 <= 15))
	{
		a += (delta2 + 15) << 3;
		data << a;
	}
	else
	{
		a += (31) << 3;
		data << a;
		data << c.pok[0].c;
	}
	if (delta > 6)
	{
		a = delta;
		data << a;
	}
	size_t aa1 = data.size;
	int deko[64]; // дельта кодируемая
	int reko[64]; // режим кодирования
	// кодирование продажи
	int rez = 0; // количество байт на дельту
	int Vrez = 0; // количество отсчетов
	for (int j = c.pro[0].c, n2 = 0, n = 0; j <= c.pro[rceni - 1].c; j++)
	{
		if (Vrez == 64)
		{
			otgruzka(rez, Vrez, deko);
			Vrez = 0;
			rez = 0;
		}
		if (j > c.pro[n2].c) n2++;
		while ((j > last_cc.pro[n].c) && (n < rceni - 1)) n++; //
		int kk2 = (j == c.pro[n2].c) ? c.pro[n2].k : 0;
		int kk3 = (j == last_cc.pro[n].c) ? last_cc.pro[n].k : 0; //
		int kk = kk2 - kk3; //
		deko[Vrez] = kk;
		if (kk == 0)
		{
			reko[Vrez] = 0;
			if (rez == 0)
			{
				Vrez++;
				continue;
			}
			if (rez == 1)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 1) || (reko[Vrez - 2] == 1))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[1] = reko[2] = 0;
				deko[0] = deko[1] = deko[2] = 0;
				Vrez = 3;
				rez = 0;
				continue;
			}
			if (rez == 2)
			{
				if (reko[Vrez - 1] == 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 1] == 0)
				{
					otgruzka(rez, Vrez - 1, deko);
					reko[0] = reko[1] = 0;
					deko[0] = deko[1] = 0;
					Vrez = 2;
					rez = 0;
					continue;
				}
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 2] == 2)
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 1;
				continue;
			}
			if (rez == 3)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = 0;
				deko[0] = 0;
				Vrez = 1;
				rez = 0;
				continue;
			}
			continue;
		}
		if ((kk >= -128) && (kk <= 127))
		{
			reko[Vrez] = 1;
			if (rez == 0)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 1;
				continue;
			}
			if (rez == 1)
			{
				Vrez++;
				continue;
			}
			if (rez == 2)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 2) || (reko[Vrez - 2] == 2))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 1;
				continue;
			}
			if (rez == 3)
			{
				if (reko[Vrez - 1] == 3)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 1] < 2)
				{
					otgruzka(rez, Vrez - 1, deko);
					reko[0] = reko[Vrez - 1];
					reko[1] = reko[Vrez];
					deko[0] = deko[Vrez - 1];
					deko[1] = deko[Vrez];
					Vrez = 2;
					rez = 1;
					continue;
				}
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 2] == 3)
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 2;
				continue;
			}
			continue;
		}
		if ((kk >= -32768) && (kk <= 32767))
		{
			reko[Vrez] = 2;
			if (rez < 2)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 2;
				continue;
			}
			if (rez == 2)
			{
				Vrez++;
				continue;
			}
			if (rez == 3)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 3) || (reko[Vrez - 2] == 3))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 2;
				continue;
			}
			continue;
		}
		if ((kk < -32768) || (kk > 32767))
		{
			reko[Vrez] = 3;
			if (rez < 3)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 3;
				continue;
			}
			if (rez == 3)
			{
				Vrez++;
				continue;
			}
			continue;
		}
	}
	otgruzka(rez, Vrez, deko);
	// кодирование покупки
	size_t aa2 = data.size;
	rez = 0; // количество байт на дельту
	Vrez = 0; // количество отсчетов
	for (int j = c.pok[0].c, n2 = 0, n = 0; j >= c.pok[rceni - 1].c; j--)
	{
		if (Vrez == 64)
		{
			otgruzka(rez, Vrez, deko);
			Vrez = 0;
			rez = 0;
		}
		if (j < c.pok[n2].c) n2++;
		while ((j < last_cc.pok[n].c) && (n < rceni - 1)) n++; //
		int kk2 = (j == c.pok[n2].c) ? c.pok[n2].k : 0;
		int kk3 = (j == last_cc.pok[n].c) ? last_cc.pok[n].k : 0; //
		int kk = kk2 - kk3; //
		deko[Vrez] = kk;
		if (kk == 0)
		{
			reko[Vrez] = 0;
			if (rez == 0)
			{
				Vrez++;
				continue;
			}
			if (rez == 1)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 1) || (reko[Vrez - 2] == 1))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[1] = reko[2] = 0;
				deko[0] = deko[1] = deko[2] = 0;
				Vrez = 3;
				rez = 0;
				continue;
			}
			if (rez == 2) {
				if (reko[Vrez - 1] == 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 1] == 0)
				{
					otgruzka(rez, Vrez - 1, deko);
					reko[0] = reko[1] = 0;
					deko[0] = deko[1] = 0;
					Vrez = 2;
					rez = 0;
					continue;
				}
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 2] == 2)
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 1;
				continue;
			}
			if (rez == 3)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = 0;
				deko[0] = 0;
				Vrez = 1;
				rez = 0;
				continue;
			}
			continue;
		}
		if ((kk >= -128) && (kk <= 127))
		{
			reko[Vrez] = 1;
			if (rez == 0)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 1;
				continue;
			}
			if (rez == 1)
			{
				Vrez++;
				continue;
			}
			if (rez == 2)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 2) || (reko[Vrez - 2] == 2))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 1;
				continue;
			}
			if (rez == 3)
			{
				if (reko[Vrez - 1] == 3)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 1] < 2)
				{
					otgruzka(rez, Vrez - 1, deko);
					reko[0] = reko[Vrez - 1];
					reko[1] = reko[Vrez];
					deko[0] = deko[Vrez - 1];
					deko[1] = deko[Vrez];
					Vrez = 2;
					rez = 1;
					continue;
				}
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if (reko[Vrez - 2] == 3)
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 2;
				continue;
			}
			continue;
		}
		if ((kk >= -32768) && (kk <= 32767))
		{
			reko[Vrez] = 2;
			if (rez < 2)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 2;
				continue;
			}
			if (rez == 2)
			{
				Vrez++;
				continue;
			}
			if (rez == 3)
			{
				if (Vrez < 2)
				{
					Vrez++;
					continue;
				}
				if ((reko[Vrez - 1] == 3) || (reko[Vrez - 2] == 3))
				{
					Vrez++;
					continue;
				}
				otgruzka(rez, Vrez - 2, deko);
				reko[0] = reko[Vrez - 2];
				reko[1] = reko[Vrez - 1];
				reko[2] = reko[Vrez];
				deko[0] = deko[Vrez - 2];
				deko[1] = deko[Vrez - 1];
				deko[2] = deko[Vrez];
				Vrez = 3;
				rez = 2;
				continue;
			}
			continue;
		}
		if ((kk < -32768) || (kk > 32767))
		{
			reko[Vrez] = 3;
			if (rez < 3)
			{
				otgruzka(rez, Vrez, deko);
				reko[0] = reko[Vrez];
				deko[0] = deko[Vrez];
				Vrez = 1;
				rez = 3;
				continue;
			}
			if (rez == 3)
			{
				Vrez++;
				continue;
			}
			continue;
		}
	}
	otgruzka(rez, Vrez, deko);
	last_cc = c;
	ip_last.r = int(data.size - aa0);
	ip_last.r_pro = int(aa2 - aa1);
	ip_last.r_pok = int(data.size - aa2);
}

_super_stat::_super_stat()
{
	last_cc = cena_zero_;
	read_cc = cena_zero_;
	size = 0;
	read_n = -666;
	ip_last.ok = false;
	ip_n.ok = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool _g_graph::mouse_down_left2(_xy r)
{
	x_tani = (i64)r.x;
	return true;
}

void _g_graph::mouse_move_left2(_xy r)
{
	i64 dx = ((i64)r.x - x_tani) / size_el;
	if (dx == 0) return;
	x_tani += dx * size_el;

	_g_scrollbar* polz = find1<_g_scrollbar>(flag_part); 
	if (!polz) return;
	double* ii = &polz->position;
	if (!ii) return;
	*ii -= double(dx) / v_vib;
	if (*ii < 0) *ii = 0;
	if (*ii > 1) *ii = 1;
	polz->cha_area();
	polz->run(0, polz, flag_run);
}

void _g_graph::run(_tetron* tt0, _tetron* tt, u64 flags)
{
	obn = true;
	cha_area();
}

_g_graph::_g_graph()
{
	graph = this;
	local_area = { {0, 200}, {0, 100} };
}

void _g_graph::ris2(_trans tr, bool final)
{
	_iarea a = tr(local_area);
	draw(a.size());
	master_bm.draw({ a.x.min, a.y.min }, bm);
}

// вынести в общий модуль?
void OSpordis(double min, double max, i64 maxN, double& mi, double& step)
{
	i64 n;
	double step2;
	if (maxN < 2) maxN = 2;
	step = exp(round(log((max - min) / maxN) / log(10)) * log(10));
	do
	{
		{
			mi = (i64(min / step)) * step;
			if (mi < min) mi += step;
			n = (i64((max - mi) / step)) + 1;
		}
		if (n < maxN) step = step * 0.1; else break;
	} while (true);
	while (n > maxN)
	{
		step = step * 10;
		{
			mi = (i64(min / step)) * step;
			if (mi < min) mi += step;
			n = (i64((max - mi) / step)) + 1;
		}
	}
	step2 = step;
	step = step2 * 0.2;
	{
		mi = (i64(min / step)) * step;
		if (mi < min) mi += step;
		n = (i64((max - mi) / step)) + 1;
	}
	if (n <= maxN) return;
	step = step2 * 0.5;
	{
		mi = (i64(min / step)) * step;
		if (mi < min) mi += step;
		n = (i64((max - mi) / step)) + 1;
	}
	if (n <= maxN) return;
	step = step2;
	{
		mi = (i64(min / step)) * step;
		if (mi < min) mi += step;
		n = (i64((max - mi) / step)) + 1;
	}
}

std::string date_to_ansi_string(int time)
{
	std::string res = "22.12.20";
	_date_time a;
	a = time;
	res[0] = (a.day / 10) + '0';
	res[1] = (a.day % 10) + '0';
	int m = ((a.month - 1) % 12) + 1;
	res[3] = (m / 10) + '0';
	res[4] = (m % 10) + '0';
	int g = ((a.month - 1) / 12) + 17;
	res[6] = (g / 10) + '0';
	res[7] = (g % 10) + '0';
	return res;
}

void _g_graph::draw(_isize size)
{
	if (size.x > 8000) size.x = 8000;
	if (size.y > 4000) size.y = 4000;
	if (bm.resize(size)) obn = true;
	if (!obn) return;
	obn = false;
	bm.clear(c_background);

	_interval y_; // диапазон у (grid)
	static std::vector<int> time_; // отсчеты времени (grid)

	double polzi_ = 0; // !! ползунок

	_g_scrollbar* sb = find1<_g_scrollbar>(flag_part);
	if (sb)	polzi_ = sb->position;

	int ll = (int)curve.size();
	if (ll == 0) return;

	i64 k_el = bm.size.x / size_el;
	if (k_el < 1) return;
	double r_el = (double)bm.size.x / k_el;

	int n = curve[0]->get_n();
	if (n == 0) return;
	//	v_vib_ = n - k_el;
	v_vib = n - 1;
	if (v_vib < 0) v_vib = 0;
	int vib = (int)(polzi_ * v_vib + 0.5); // !! ползунок

	int period = 60;
	int pause_max = 3;
	_element_chart* al = new _element_chart[ll]; // элементы линий
	// 1-й проход - вычисление zmin, zmax
	double zmin = 1E100;
	double zmax = -1E100;
	curve[0]->get_n_info(vib, &al[0]);
	int timelast = al[0].time;
	for (int i = 1; i < ll; i++) curve[i]->get_t_info(timelast, &al[i]);
	timelast -= period;
	int ke = 0; // количество построенных элементов
	while (ke < k_el)
	{
		int timenext = 2000000000; // следующее время
		for (int i = 0; i < ll; i++)
			if (al[i].n >= 0)
				if (al[i].time < timenext) timenext = al[i].time;
		if (timenext == 2000000000) break;

		//int dt = (timenext - timelast) / period;
		//ke += (dt <= (pause_max + 1)) ? dt : 2;
		//if (ke > k_el) break;
		ke++;
		timelast = timenext;

		for (int i = 0; i < ll; i++)
		{
			if (al[i].n < 0) continue;
			if (al[i].time == timelast)
			{ // сработало
				if (al[i].min < zmin) zmin = al[i].min;
				if (al[i].max > zmax) zmax = al[i].max;
				curve[i]->get_n_info(al[i].n + 1, &al[i]);
			}
		}
	}
	if (zmin == zmax) zmax = zmin + 1.0;
	y_ = { zmin, zmax };
	time_.clear();
	// 2-й проход - рисование
	curve[0]->get_n_info(vib, &al[0]);
	timelast = al[0].time;
	for (int i = 1; i < ll; i++) curve[i]->get_t_info(timelast, &al[i]);
	timelast -= period;
	ke = 0; // количество построенных элементов
	while (ke < k_el)
	{
		int timenext = 2000000000; // следующее время
		for (int i = 0; i < ll; i++)
			if (al[i].n >= 0)
				if (al[i].time < timenext) timenext = al[i].time;
		if (timenext == 2000000000) break;

		//int dt = (timenext - timelast) / period;
		//ke += (dt <= (pause_max + 1)) ? dt : 2;
		//if (ke > k_el) break;
		ke++;
		timelast = timenext;
		time_.push_back(timelast);

		for (int i = 0; i < ll; i++)
		{
			if (al[i].n < 0) continue;
			if (al[i].time == timelast)
			{ // сработало
				double ymi = bm.size.y - (al[i].min - zmin) * bm.size.y / (zmax - zmin);
				double yma = bm.size.y - (al[i].max - zmin) * bm.size.y / (zmax - zmin);
				double x = r_el * (ke - 1i64);
				curve[i]->draw(al[i].n, { {x, x + r_el}, {yma, ymi} }, &bm);
				curve[i]->get_n_info(al[i].n + 1, &al[i]);
			}
		}
	}
	delete[] al;

	// рисование сетки

	uint col_setka = c_max - 0xE0000000; // цвет сетки
	uint col_setka_font = c_def; // цвет подписи сетки
	if (time_.size() < 1)
	{
		bm.line({ 0, 0 }, { bm.size.x - 1, bm.size.y - 1 }, 0xFF800000);
		bm.line({ 0, bm.size.y - 1 }, { bm.size.x - 1 , 0 }, 0xFF800000);
		return;
	}
	// рисование горизонтальных линий сетки с подписями
	i64 dex = 35; // длина подписи
	i64 maxN = bm.size.y / 15;
	if (maxN > 1)
	{
		double mi, step;
		OSpordis(y_.min, y_.max, maxN, mi, step);
		for (double y = mi; y < y_.max; y += step)
		{
			double yy = bm.size.y - 0.5 - (y - y_.min) * bm.size.y / (y_.max - y_.min);
			bm.line({ dex, (int)(yy) }, { bm.size.x - dex, (int)(yy) }, col_setka);
			bm.text16(0, (int)(yy - 7), double_to_astring(y, 2).c_str(), col_setka_font);
			bm.text16(bm.size.x - dex, (int)(yy - 7), double_to_astring(y, 2).data(), col_setka_font);
		}
	}
	// рисование вертикальных линий сетки с подписями
	static int g_delta_time[] = {
		1, 2, 3, 5, 10, 15, 20, 30, // секунды
		60, 120, 180, 300, 600, 900, 1200, 1800, // минуты
		3600, 7200, 10800, 14400, 21600, 28800, 43200, // часы
		86400, 172800, 345600, 691200, 1382400, // дни
		2764800, 5529600, 8294400, 11059200, 16588800, // месяца
		33177600 }; // год

	double rel = r_el;
	int stept = (((int)(dex / rel)) + 1) * period;
	int ks = sizeof(g_delta_time) / sizeof(g_delta_time[0]);
	for (int i = 0; i < ks; i++)
		if (g_delta_time[i] >= stept)
		{
			stept = g_delta_time[i];
			break;
		}
	int dele[] = { 1, 60, 3600, 86400, 2764800, 33177600 };
	int ost[] = { 60, 60, 24, 32, 12, 1000 };
	int ido = 0;
	if (stept % 33177600) ido = 4;
	if (stept % 2764800) ido = 3;
	if (stept % 86400) ido = 2;
	if (stept % 3600) ido = 1;
	if (stept % 60) ido = 0;
	std::string s = "00:00";
	int mintime = 0;
	int pr_time = 0;
	for (uint i = 0; i < time_.size(); i++)
	{
		if (time_[i] == 0) continue;
		if (mintime == 0) mintime = time_[i];
		bool sca = ((pr_time > 0) && (time_[i] - pr_time > 36000));
		pr_time = time_[i];
		if (time_[i] % stept == 0)
		{ // вертикальная линия с подписью
			double x = rel * i;
			if ((x <= dex) || (x >= (i64)bm.size.x - dex)) continue;
			uint cl = (sca) ? (0x80FF0000) : col_setka;
			bm.line({ (int)x, 0 }, { (int)x, bm.size.y - 1 }, cl);
			if ((x - 13 <= dex) || (x + 13 >= (i64)bm.size.x - dex)) continue;
			int ii = (time_[i] / dele[ido]) % ost[ido];
			s[4] = '0' + (ii % 10);
			s[3] = '0' + (ii / 10);
			ii = (time_[i] / dele[ido + 1]) % ost[ido + 1];
			s[1] = '0' + (ii % 10);
			s[0] = '0' + (ii / 10);
			bm.text16((int)(x - 13), bm.size.y - 11, s.data(), col_setka_font);
			bm.text16((int)(x - 13), -2, s.data(), col_setka_font);
			continue;
		}
		if (sca)
		{
			double x = rel * i;
			if ((x <= dex) || (x >= (i64)bm.size.x - dex)) continue;
			bm.line({ (int)x, 0 }, { (int)x, bm.size.y - 1 }, 0x80FF0000);
		}
	}
	// рисование даты
	bm.text16n(dex + 10, 10, date_to_ansi_string(mintime).data(), 4, c_max - 0x80000000);
	// рисование количества элементов
	bm.text16n(dex + 10, 60, std::to_string(ss.size).data(), 2, 0x60ff0000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _mctds_candle::get_n()
{
	return (int)cen1m.size();
}

void _mctds_candle::get_n_info(int n, _element_chart* e)
{
	if (n >= (int)cen1m.size())
	{
		e->n = -1;
		return;
	}
	e->n = n;
	e->time = cen1m[n].time;
	e->min = cen1m[n].min * c_unpak;
	e->max = cen1m[n].max * c_unpak;
	e->middle = cen1m[n].cc * c_unpak;
}

void _mctds_candle::get_t_info(int t, _element_chart* e)
{
	e->n = -1; // !! написать, когда потребуется!!
}

void _mctds_candle::push(_stack* mem)
{
	*mem << cen1m;
}

void _mctds_candle::pop(_stack* mem)
{
	*mem >> cen1m;
}

void _mctds_candle::draw(int n, _area area, _bitmap* bm)
{
	double min_ = cen1m[n].min * c_unpak;
	double max_ = cen1m[n].max * c_unpak;
	double first_ = cen1m[n].first * c_unpak;
	double last_ = cen1m[n].last * c_unpak;

	_area oo = area;
	int x1 = (int)(oo.x.min + 1);
	int x2 = (int)(oo.x.max - 1);
	if (x2 < x1) return;

	uint col_rost = 0xFF28A050; // цвет ростущей свечки
	uint col_pade = 0xFF186030; // цвет падающей свечки
	double yfi, yla;
	if (min_ < max_)
	{
		yfi = oo.y.max - oo.y.length() * (first_ - min_) / (max_ - min_);
		yla = oo.y.max - oo.y.length() * (last_ - min_) / (max_ - min_);
	}
	else
	{
		yfi = yla = oo.y.min;
	}
	if (first_ <= last_)
	{
		bm->fill_rectangle({ {x1, x2}, {(int)yla, (int)yfi} }, col_rost);
		bm->line({ (x1 + x2) >> 1, (int)oo.y.max }, { (x1 + x2) >> 1, (int)oo.y.min }, col_rost);
	}
	else
	{
		bm->fill_rectangle({ {x1, x2}, {(int)yfi, (int)yla} }, col_pade);
		bm->line({ (x1 + x2) >> 1, (int)oo.y.max }, { (x1 + x2) >> 1, (int)oo.y.min }, col_pade);
	}

}

void _mctds_candle::recovery()
{
	i64 vcc = 0;
	if (cen1m.size()) vcc = cen1m.back().ncc.max;
	i64 ssvcc = ss.size;
	if (ssvcc == vcc) return; // ничего не изменилось
	if (vcc < ssvcc) // добавились несколько цен
	{
		_prices cc;
		int t = 0;
		_cen_pak* cp = 0;
		if (cen1m.size())
		{
			cp = &cen1m.back();
			cp->cc *= ((double)cp->ncc.max - cp->ncc.min);
			t = cp->time;
		}
		for (i64 i = vcc; i < ssvcc; i++)
		{
			if (i > 572737) // i > 572738
			{
				if (cp == 0) continue;
			}
			ss.read(i, cc);
			int t2 = cc.time.to_minute();
			if (t2 != t)
			{
				t = t2;
				if (cp)	cp->cc /= ((double)cp->ncc.max - cp->ncc.min);
				_cen_pak we;
				we.time = t;
				we.ncc.min = i;
				we.ncc.max = i + 1;
				we.first = ((int)cc.pok[0].c + (int)cc.pro[0].c) / 2;
				we.last = we.first;
				we.min = we.first;
				we.max = we.first;
				we.cc = we.first;
				cen1m.push_back(we);
				cp = &cen1m.back();
			}
			else
			{
				if (cp == 0) continue; // для паранойи компилятора
				int aa = ((int)cc.pok[0].c + (int)cc.pro[0].c) / 2;
				cp->cc += aa;
				if (aa < cp->min) cp->min = aa;
				if (aa > cp->max) cp->max = aa;
				cp->ncc.max++;
				cp->last = aa;
			}
		}
		if (cp)	cp->cc /= ((double)cp->ncc.max - cp->ncc.min);
		return;
	}
	_prices cc; // уменьшились цены, полный пересчет
	cen1m.clear();
	int t = 0;
	_cen_pak* cp = 0;
	for (i64 i = 0; i < ssvcc; i++)
	{
		ss.read(i, cc);
		int t2 = cc.time.to_minute();
		if (t2 != t)
		{
			t = t2;
			if (cp)	cp->cc /= ((double)cp->ncc.max - cp->ncc.min);
			_cen_pak we;
			we.time = t;
			we.ncc.min = i;
			we.ncc.max = i + 1;
			we.first = ((int)cc.pok[0].c + (int)cc.pro[0].c) / 2;
			we.last = we.first;
			we.min = we.first;
			we.max = we.first;
			we.cc = we.first;
			cen1m.push_back(we);
			cp = &cen1m.back();
		}
		else
		{
			if (cp == 0) continue; // для паранойи компилятора
			int aa = ((int)cc.pok[0].c + (int)cc.pro[0].c) / 2;
			cp->cc += aa;
			if (aa < cp->min) cp->min = aa;
			if (aa > cp->max) cp->max = aa;
			cp->ncc.max++;
			cp->last = aa;
		}
	}
	if (cp)	cp->cc /= ((double)cp->ncc.max - cp->ncc.min);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _latest_events::start()
{
	if ((minute[2] == 2) && (event[0] == event[1]) && (event[0] == event[2])) // триплет
	{
		if (event[0] == 1) return 70;
		if (event[0] == 2) // фиолетовый
			if ((x[0] > x[1]) && (x[1] > x[2]))	return 13;
		if (event[0] == 3)
		{
			if ((x[0] > x[1]) && (x[1] > x[2]))	return 40;
			if ((x[0] < x[1]) && (x[1] < x[2]))	return 90;
		}
		//		if (event_[0] == 4) return 120;  //голубой
		if (event[0] == 6) return 60;   //зеленый
		return 0;
	}
	if ((minute[1] == 1) && (event[0] == event[1])) // дуплет
	{
		if (event[0] == 2) // фиолетовый
		{
			if (event[2] == 5) return 100; //песочный
			if ((event[2] == 4) && (event[3] == 4)) return 40;
		}
		return 0;
	}
	return 0;
}

bool _latest_events::stop()
{
	if ((event[0] == 5) && (event[1] == 5) && (minute[1] == 1)) return true; // песочный
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _nervous_oracle::push(_stack* mem)
{
	*mem << zn;
}

void _nervous_oracle::pop(_stack* mem)
{
	*mem >> zn;
}

int _nervous_oracle::get_n()
{
	return (int)zn.size();
}

void _nervous_oracle::get_n_info(int n, _element_chart* e)
{
	if (n >= (int)zn.size())
	{
		e->n = -1;
		return;
	}
	e->n = n;
	e->time = zn[n].time;
	e->middle = ((double)zn[n].max_pro + (double)zn[n].min_pok) * 0.5 * c_unpak;
	e->min = e->middle;
	e->max = e->middle;
}

void _nervous_oracle::get_t_info(int t, _element_chart* e)
{
	auto x = lower_bound(zn.begin(), zn.end(), t);
	if (x == zn.end())
	{
		e->n = -1;
		return;
	}
	int xx = (int)(x - zn.begin());
	e->n = xx;
	e->time = zn[xx].time;
	e->middle = ((double)zn[xx].max_pro + (double)zn[xx].min_pok) * 0.5 * c_unpak;
	e->min = e->middle;
	e->max = e->middle;
}

_latest_events _nervous_oracle::get_latest_events(i64 nn)
{
	const i64 k = 4;
	_latest_events e;
	e.event[0] = e.event[1] = e.event[2] = e.event[3] = 0;
	e.minute[0] = e.minute[1] = e.minute[2] = e.minute[3] = 0;
	e.x[0] = e.x[1] = e.x[2] = e.x[3] = 0.0;
	if (nn < 10) return e;
	i64 ii = std::max(k, nn - 40);
	int ee = 0;
	for (i64 n = nn; n >= ii; n--)
	{
		if ((i64)zn[n].time - zn[n - k].time != k * 60) continue;
		bool rost_pro = true;
		bool rost_pok = true;
		bool pade_pro = true;
		bool pade_pok = true;
		for (i64 i = n - k; i < n; i++)
		{
			if (zn[i].r_pro >= zn[i + 1].r_pro) rost_pro = false;
			if (zn[i].r_pok >= zn[i + 1].r_pok) rost_pok = false;
			if (zn[i].r_pro <= zn[i + 1].r_pro) pade_pro = false;
			if (zn[i].r_pok <= zn[i + 1].r_pok) pade_pok = false;
		}
		char a = 0;
		if ((rost_pro || rost_pok) && (pade_pro || pade_pok))
			a = 7;
		else
		{
			if (rost_pok) a += 1;
			if (rost_pro) a += 2;
			if (pade_pro || pade_pok) a += 3;
			if (pade_pok) a += 1;
			if (pade_pro) a += 2;
		}
		if (a == 0) continue;
		e.event[ee] = a;
		e.minute[ee] = (int)(nn - n);
		e.x[ee] = ((double)zn[n].max_pro + (double)zn[n].min_pok) * 0.5 * c_unpak;
		ee++;
		if (ee == 4) break;
	}
	return e;
}

void _nervous_oracle::draw(int n, _area area, _bitmap* bm)
{
	const i64 k = 4;
	if (n < k) return;
	if ((i64)zn[n].time - zn[n - k].time != k * 60) return;
	bool rost_pro = true;
	bool rost_pok = true;
	bool pade_pro = true;
	bool pade_pok = true;
	for (i64 i = n - k; i < n; i++)
	{
		if (zn[i].r_pro >= zn[i + 1].r_pro) rost_pro = false;
		if (zn[i].r_pok >= zn[i + 1].r_pok) rost_pok = false;
		if (zn[i].r_pro <= zn[i + 1].r_pro) pade_pro = false;
		if (zn[i].r_pok <= zn[i + 1].r_pok) pade_pok = false;
	}
	_element_nervous& a = zn[n];
	uint c = 0xFF808080;

	if (rost_pro) c += 0x70;
	if (rost_pok) c += 0x700000;
	if (pade_pro) c -= 0x70;
	if (pade_pok) c -= 0x700000;

	double r = area.x.length() * 0.5 * 2;
	if (get_latest_events(n).start())
	{
		c = 0xFFFF0000;
		r *= 2;
	}

	if (c == 0xFF808080) return;

	bm->fill_ring(area.center(), r, r * 0.1, c, c);
}

void _nervous_oracle::recovery()
{
	i64 vcc = 0;
	if (zn.size()) vcc = zn.back().ncc.max;
	i64 ssvcc = ss.size;
	if (ssvcc == vcc) return; // ничего не изменилось
	if (vcc < ssvcc) // добавились несколько цен
	{
		_prices cc;
		_super_stat::_info_pak inf;
		int t = 0;
		_element_nervous* cp = 0;
		if (zn.size())
		{
			cp = &zn.back();
			if (cp->v_r)
			{
				cp->r *= cp->v_r;
				cp->r_pok *= cp->v_r;
				cp->r_pro *= cp->v_r;
			}
			t = cp->time;
		}
		for (i64 i = vcc; i < ssvcc; i++)
		{
			ss.read(i, cc, &inf);
			int t2 = cc.time.to_minute();
			if (t2 != t)
			{
				t = t2;
				if (cp)
				{
					if (cp->v_r)
					{
						cp->r /= cp->v_r;
						cp->r_pok /= cp->v_r;
						cp->r_pro /= cp->v_r;
					}
				}
				_element_nervous we;
				we.time = t;
				we.ncc.min = i;
				we.ncc.max = i + 1;
				we.max_pok = we.min_pok = cc.pok[0].c;
				we.max_pro = we.min_pro = cc.pro[0].c;
				if (inf.ok)
				{
					we.v_r = 1;
					we.r = inf.r;
					we.r_pok = inf.r_pok;
					we.r_pro = inf.r_pro;
				}
				else
					we.v_r = 0;
				zn.push_back(we);
				cp = &zn.back();
			}
			else
			{
				if (cp == 0) continue; // для паранойи компилятора
				if (cc.pok[0].c < cp->min_pok) cp->min_pok = cc.pok[0].c;
				if (cc.pok[0].c > cp->max_pok) cp->max_pok = cc.pok[0].c;
				if (cc.pro[0].c < cp->min_pro) cp->min_pro = cc.pro[0].c;
				if (cc.pro[0].c > cp->max_pro) cp->max_pro = cc.pro[0].c;
				cp->ncc.max++;
				if (inf.ok)
				{
					if (cp->v_r)
					{
						cp->v_r++;
						cp->r += inf.r;
						cp->r_pok += inf.r_pok;
						cp->r_pro += inf.r_pro;
					}
					else
					{
						cp->v_r = 1;
						cp->r = inf.r;
						cp->r_pok = inf.r_pok;
						cp->r_pro = inf.r_pro;
					}
				}
			}
		}
		if (cp)
		{
			if (cp->v_r)
			{
				cp->r /= cp->v_r;
				cp->r_pok /= cp->v_r;
				cp->r_pro /= cp->v_r;
			}
		}
		return;
	}
	_prices cc; // уменьшились цены, полный пересчет
	_super_stat::_info_pak inf;
	zn.clear();
	int t = 0;
	_element_nervous* cp = 0;
	for (i64 i = 0; i < ssvcc; i++)
	{
		ss.read(i, cc, &inf);
		int t2 = cc.time.to_minute();
		if (t2 != t)
		{
			t = t2;
			if (cp)
			{
				if (cp->v_r)
				{
					cp->r /= cp->v_r;
					cp->r_pok /= cp->v_r;
					cp->r_pro /= cp->v_r;
				}
			}
			_element_nervous we;
			we.time = t;
			we.ncc.min = i;
			we.ncc.max = i + 1;
			we.max_pok = we.min_pok = cc.pok[0].c;
			we.max_pro = we.min_pro = cc.pro[0].c;
			if (inf.ok)
			{
				we.v_r = 1;
				we.r = inf.r;
				we.r_pok = inf.r_pok;
				we.r_pro = inf.r_pro;
			}
			else
				we.v_r = 0;
			zn.push_back(we);
			cp = &zn.back();
		}
		else
		{
			if (cp == 0) continue; // для паранойи компилятора
			if (cc.pok[0].c < cp->min_pok) cp->min_pok = cc.pok[0].c;
			if (cc.pok[0].c > cp->max_pok) cp->max_pok = cc.pok[0].c;
			if (cc.pro[0].c < cp->min_pro) cp->min_pro = cc.pro[0].c;
			if (cc.pro[0].c > cp->max_pro) cp->max_pro = cc.pro[0].c;
			cp->ncc.max++;
			if (inf.ok)
			{
				if (cp->v_r)
				{
					cp->v_r++;
					cp->r += inf.r;
					cp->r_pok += inf.r_pok;
					cp->r_pro += inf.r_pro;
				}
				else
				{
					cp->v_r = 1;
					cp->r = inf.r;
					cp->r_pok = inf.r_pok;
					cp->r_pro = inf.r_pro;
				}
			}
		}
	}
	if (cp)
	{
		if (cp->v_r)
		{
			cp->r /= cp->v_r;
			cp->r_pok /= cp->v_r;
			cp->r_pro /= cp->v_r;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _oracle3::get_n_info(int n, _element_chart* e)
{
	if (n >= (int)zn.size())
	{
		e->n = -1;
		return;
	}
	e->n = n;
	e->time = zn[n].time;
	e->min = (zn[n].min - 1) * c_unpak;
	e->max = zn[n].max * c_unpak;
	e->middle = (e->min + e->max) * 0.5;
}

void _oracle3::get_t_info(int t, _element_chart* e)
{
	auto x = lower_bound(zn.begin(), zn.end(), t);
	if (x == zn.end())
	{
		e->n = -1;
		return;
	}
	int xx = (int)(x - zn.begin());
	e->n = xx;
	e->time = zn[xx].time;
	e->min = (zn[xx].min - 1) * c_unpak;
	e->max = zn[xx].max * c_unpak;
	e->middle = (e->min + e->max) * 0.5;
}

int _oracle3::get_n()
{
	return (int)zn.size();
}

void _oracle3::recovery()
{
	i64 vcc = 0;
	if (zn.size()) vcc = zn.back().ncc.max;
	i64 ssvcc = ss.size;
	if (ssvcc == vcc) return; // ничего не изменилось
	if (vcc < ssvcc) // добавились несколько цен
	{
		_prices cc;
		int t = 0;
		_element_oracle* cp = 0;
		if (zn.size())
		{
			cp = &zn.back();
			t = cp->time;
		}
		for (i64 i = vcc; i < ssvcc; i++)
		{
			ss.read(i, cc);
			int t2 = cc.time.to_minute();
			if (t2 != t)
			{
				t = t2;
				_element_oracle we;
				we.time = t;
				we.ncc.min = i;
				we.ncc.max = i + 1;
				we.max = cc.pro[rceni - 1].c;
				we.min = cc.pok[rceni - 1].c;
				zn.push_back(we);
				cp = &zn.back();
			}
			else
			{
				if (cp == 0) continue; // для паранойи компилятора
				if (cc.pok[rceni - 1].c < cp->min) cp->min = cc.pok[rceni - 1].c;
				if (cc.pro[rceni - 1].c > cp->max) cp->max = cc.pro[rceni - 1].c;
				cp->ncc.max++;
			}
		}
		return;
	}
	_prices cc; // уменьшились цены, полный пересчет
	zn.clear();
	int t = 0;
	_element_oracle* cp = 0;
	for (i64 i = 0; i < ssvcc; i++)
	{
		ss.read(i, cc);
		int t2 = cc.time.to_minute();
		if (t2 != t)
		{
			t = t2;
			_element_oracle we;
			we.time = t;
			we.ncc.min = i;
			we.ncc.max = i + 1;
			we.max = cc.pro[rceni - 1].c;
			we.min = cc.pok[rceni - 1].c;
			zn.push_back(we);
			cp = &zn.back();
		}
		else
		{
			if (cp == 0) continue; // для паранойи компилятора
			if (cc.pok[rceni - 1].c < cp->min) cp->min = cc.pok[rceni - 1].c;
			if (cc.pro[rceni - 1].c > cp->max) cp->max = cc.pro[rceni - 1].c;
			cp->ncc.max++;
		}
	}
}

void _oracle3::draw(int n, _area area, _bitmap* bm)
{
	static _prices pri[61]; // цены
	static int min, max; // разброс по y
	min = 0;
	max = 1;
	for (auto& i : pri) i.clear();

	for (i64 i = zn[n].ncc.min; i < zn[n].ncc.max; i++)
	{
		if (i < begin_ss)
		{
			i64 delta = begin_ss - i;
			if (delta >= max_part)
				part_ss.clear();
			else
			{
				_prices w;
				w.clear();
				for (int i = 0; i < delta; i++)
				{
					part_ss.push_front(w);
					if (part_ss.size() > max_part) part_ss.pop_back();
				}
			}
			begin_ss = i;
		}
		if (i >= begin_ss + (i64)part_ss.size())
		{
			_prices w;
			w.clear();
			i64 delta = i - (begin_ss + (int)part_ss.size()) + 1;
			if (delta >= max_part)
			{
				part_ss.clear();
				part_ss.push_back(w);
				begin_ss = i;
			}
			else
				for (int i = 0; i < delta; i++)
				{
					part_ss.push_back(w);
					if (part_ss.size() > max_part)
					{
						part_ss.pop_front();
						begin_ss++;
					}
				}
		}
		i64 ii = i - begin_ss;
		if (part_ss[ii].empty()) ss.read(i, part_ss[ii]);
		pri[part_ss[ii].time.second] = part_ss[ii];
		min = zn[n].min - 1;
		max = zn[n].max;
	}

	_area oo = area;
	int x1 = (int)oo.x.min;
	int x2 = (int)oo.x.max;
	int dx = x2 - x1;
	if (dx < 2) return;
	int step = 60;
	if (dx >= 4) step = 30;
	if (dx >= 6) step = 20;
	if (dx >= 8) step = 15;
	if (dx >= 10) step = 12;
	if (dx >= 12) step = 10;
	if (dx >= 20) step = 6;
	if (dx >= 24) step = 5;
	if (dx >= 30) step = 4;
	if (dx >= 40) step = 3;
	if (dx >= 60) step = 2;
	if (dx >= 120) step = 1;
	int kol = 60 / step;
	int dd = max - min;
	double ddy = oo.y.max - oo.y.min;
	for (int i = 0; i < kol; i++)
	{
		int ss = i * step;
		while (pri[ss].empty())
		{
			if (ss + 1 >= (i + 1) * step) break;
			ss++;
		}
		if (pri[ss].empty()) continue;
		int xx1 = x1 + (x2 - x1) * i / kol;
		int xx2 = x1 + (x2 - x1) * (i + 1) / kol - 1;
		for (int j = rceni - 1; j >= 0; j--)
		{
			i64 ce = pri[ss].pro[j].c;
			int yy1 = (int)(oo.y.min + (max - ce) * ddy / dd);
			int yy2 = (int)(oo.y.min + (max - ce + 1) * ddy / dd) - 1;
			if (yy2 < yy1) continue;
			uint q = (uint)sqrt(pri[ss].pro[j].k) + 32;
			if (q > 255) q = 255;
			uint cc = (q << 8) + (q << 16) + 0xA0000000;
			bm->fill_rectangle({ {xx1, xx2}, {yy1, yy2} }, cc);
		}
		for (int j = 0; j < rceni; j++)
		{
			i64 ce = pri[ss].pok[j].c;
			int yy1 = (int)(oo.y.min + (max - ce) * ddy / dd);
			int yy2 = (int)(oo.y.min + (max - ce + 1) * ddy / dd) - 1;
			if (yy2 < yy1) continue;
			uint q = (uint)sqrt(pri[ss].pok[j].k) + 32;
			if (q > 255) q = 255;
			uint cc = q + (q << 8) + 0xA0000000;
			bm->fill_rectangle({ {xx1, xx2}, {yy1, yy2} }, cc);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_recognize::_recognize()
{
	static const int vf_ = 2; // количество шрифтов
	_bitmap bm_[vf_]; // холст для рисования
	bm_[0].set_font(L"MS Sans Serif", false);
	bm_[1].set_font(L"MS Sans Serif", true);
	std::wstring nabor = L"0123456789.,:;-()[]><=абвгдежзийклмнопрстуфхцчшщъыьэюяАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
		L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int L = (int)nabor.size();
	std::wstring ss = L"0";
	ushort aa[20];
	for (int nf = 0; nf < vf_; nf++)
		for (int i = 0; i < L; i++)
		{
			wchar_t c = nabor[i];
			ss[0] = c;
			_isize size = bm_[nf].size_text(ss.data(), 8);
			//			if (c == ' ') ShowMessage(IntToStr((int)size.cx));
			//			if (size.cy != 13) ShowMessage(ss+" "+IntToStr((int)size.cy));
			if (size.x > 20) return;//что-то не то...
			if ((size.x > bm_[nf].size.x) || (size.y > bm_[nf].size.y))
				bm_[nf].resize({ std::max(size.x, bm_[nf].size.x), std::max(size.y, bm_[nf].size.y) });
			bm_[nf].clear(0);
			bm_[nf].text(0, 0, ss.data(), 8, 0xffffff, 0);
			ZeroMemory(aa, sizeof(ushort) * size.x);
			for (i64 j = size.y - 1; j >= 0; j--)
			{
				uint* sl = bm_[nf].sl(j);
				for (int i = 0; i < size.x; i++) aa[i] = (aa[i] << 1) + (sl[i] > 0);
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
	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	return 0;
}

int _recognize::read_vnimanie_pokupka()
{
	HWND w = FindWindow(0, L"Внимание");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 17) || (elem.size() > 18)) return 2; // неправильное количество элементов
	if (find_elem(L"Выдействителы-ожелаетевыполнитьтранзакцию") < 0) return 3;
	if (find_elem(L"ЛимитированнаяПокупка") < 0) return 4;
	return 0;
}

int _recognize::read_vnimanie_prodaza()
{
	HWND w = FindWindow(0, L"Внимание");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 17) || (elem.size() > 18)) return 2; // неправильное количество элементов
	if (find_elem(L"Выдействителы-ожелаетевыполнитьтранзакцию") < 0) return 3;
	if (find_elem(L"ЛимитированнаяПродажа") < 0) return 4;
	return 0;
}

int _recognize::read_vvod_zaya()
{
	HWND w = FindWindow(0, L"МБ ФР: Т+ Акции и ДР Ввод заявки");
	if (!w) return 1;
	offset = { 0, 0 };
	ClientToScreen(w, &offset);
	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	image.grab_ecran_oo2(w);
	find_text13(0xFF000000);
	if ((elem.size() < 20) || (elem.size() > 28)) return 2; // неправильное количество элементов
	return 0;
}

struct FindWnd
{
	std::wstring class_name_; // имя класса
	std::wstring window_name_; // имя окна
	HWND hwnd_{}; // указатель окна
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
	//MessageBox(0, s.c_str(), L"упс", MB_OK | MB_TASKMODAL);

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
	EnumChildWindows(w, PoiskOkna, (LPARAM)& aa);
	return aa.hwnd_;
}

int _recognize::read_tablica_zayavok(int a, int& b)
{
	b = 0;
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return 1;
	HWND w2 = FindSubWindow(w, L"InfoMDITableCommon", L"Таблица заявок Основной рынок"); // InfoPriceTable HostWindow
	if (!w2) return 2;
	RECT rr;
	GetWindowRect(w2, &rr);

	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	image.grab_ecran_oo2(w2);
	//	image_.SaveToFile(L"err.bmp");
	find_text13(0xFF0040FF, 10); // синим цветом 

	std::wstring ss = std::to_wstring(a);
	for (uint i = 0; i < elem.size(); i++)
		if (elem[i].s == ss) b++;
	return 0;
}

bool _recognize::find_window_prices(RECT* rr)
{
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return false;
	HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"Сбербанк [МБ ФР: Т+ Акции и ДР] Котировки"); // InfoPriceTable HostWindow
	if (!w2) return false;
	GetWindowRect(w2, rr);
	return true;
}

i64 to_int(const std::wstring& s) // преобразование в число с игнорированием нечисловых символов
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
	find_text13(0xFF0000FF); // синим цветом покупки
	if (elem.size() != rceni * 2) return 3;
	i64 pre = 0;
	for (int i = 0; i < rceni; i++)
	{
		i64 a = to_int(elem[i * 2i64].s);
		if (a <= pre) return 4;
		pre = a;
		if ((a < 1) || (a > 65000)) return 5;
		pr->pok[rceni - 1 - i].c = static_cast<ushort>(a);
		a = to_int(elem[i * 2i64 + 1i64].s);
		if ((a < 1) || (a > 2000000000)) return 6;
		pr->pok[rceni - 1 - i].k = static_cast<int>(a);
	}
	find_red_text13(24); // красным цветом продажи
	if (elem.size() != rceni * 2) return 7;
	for (int i = 0; i < rceni; i++)
	{
		i64 a = to_int(elem[i * 2i64].s);
		if (a <= pre) return 8;
		pre = a;
		if ((a < 1) || (a > 65000)) return 9;
		pr->pro[i].c = static_cast<ushort>(a);
		a = to_int(elem[i * 2i64 + 1i64].s);
		if ((a < 1) || (a > 2000000000)) return 10;
		pr->pro[i].k = static_cast<int>(a);
	}
	return 0;
}

int _recognize::read_prices_from_screen(_prices* pr)
{
	HWND w = FindWindow(0, mmm3.c_str());
	if (!w) return 1;
	HWND w2 = FindSubWindow(w, L"InfoPriceTable", L"Сбербанк [МБ ФР: Т+ Акции и ДР] Котировки"); // InfoPriceTable HostWindow
	if (!w2) return 2;
	image.clear(0xFFFFFFFF); // т.к. если окно свернуто, то не грабится
	pr->time.now();
	image.grab_ecran_oo2(w2);
	find_text13(0xFF0000FF); // синим цветом покупки
	size_t ww = elem.size();
	if (elem.size() != rceni * 2) return 3;
	i64 pre = 0;
	for (i64 i = 0; i < rceni; i++)
	{
		std::wstring swe = elem[i * 2].s;
		i64 a = to_int(swe);
		if (a <= pre) return 4;
		pre = a;
		if ((a < 1) || (a > 65000)) return 5;
		pr->pok[rceni - 1 - i].c = static_cast<ushort>(a);
		a = to_int(elem[i * 2 + 1].s);
		if ((a < 1) || (a > 2000000000)) return 6;
		pr->pok[rceni - 1 - i].k = static_cast<int>(a);
	}
	find_red_text13(24); // красным цветом продажи
	if (elem.size() != rceni * 2) return 7;
	for (i64 i = 0; i < rceni; i++)
	{
		i64 a = to_int(elem[i * 2].s);
		if (a <= pre) return 8;
		pre = a;
		if ((a < 1) || (a > 65000)) return 9;
		pr->pro[i].c = static_cast<ushort>(a);
		a = to_int(elem[i * 2 + 1].s);
		if ((a < 1) || (a > 2000000000)) return 10;
		pr->pro[i].k = static_cast<int>(a);
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
		bool norm;
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
			{ // КРИТИЧЕСКАЯ РАЗНИЦА!!!!
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
		bool norm;
		for (int i = 0; i < rx; i++) {
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
			{ // КРИТИЧЕСКАЯ РАЗНИЦА!!!!
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
		bool norm;
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
			{ // КРИТИЧЕСКАЯ РАЗНИЦА!!!!
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
	u64 n = fi - dalee.begin();
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

void set_test()
{
	if (mmm1 == L"1") load_mmm(exe_path.wstring() + mmm_file);
	_prices a;
	int ok = recognize.read_prices_from_screen(&a);
	MessageBox(0, std::to_wstring(ok).c_str(), L"упс", MB_OK | MB_TASKMODAL);
//	_g_picture* g = new _g_picture;
//	g->set_pic(recognize.image);
//	for (int i = 0; i < recognize.elem.size(); i++)	g->pic.rectangle(recognize.elem[i].area, c_def - 0x80000000);
//	_t_trans* tr = n_ko->operator _t_basic_go *()->set_t_trans(g, flag_sub_go + flag_part);
}
