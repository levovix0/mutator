﻿#pragma once

#include <deque>

#include "basic_go.h"

constexpr int rceni = 20; // предложений продажи, предложений покупки ( ВСЕГО = Rceni * 2 );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_test(); // пробное чтение

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _offer // предложение
{
	ushort c; // цена
	int k; // количество
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _date_time // дата и время
{
	uchar month, day, hour, minute, second; // (месяц+год), день, час, мин, сек

	operator int(); // преобразовать в целое число
	int to_minute(); // преобразовать в целое число без секунд
	void operator =(int a); // присвоить число
	void now(); // присвоить текущее время
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _prices // массив спроса предложения
{
	_offer pok[rceni]; // предложение покупки (порядок с самого выгодного)
	_offer pro[rceni]; // предложение продажи (порядок с самого выгодного)
	_date_time time; // время

	inline void clear() { time.second = 200; } // метка пустого прайса
	inline bool empty() { return (time.second == 200); } // проверка на пустоту 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _super_stat // супер статистика цен
{
	struct _info_pak // дополнительная информация для упаковки
	{
		bool ok; // есть данные
		int r; // общий размер
		int r_pro; // размер продаж
		int r_pok; // размер покупок
	};
	i64 size; // количество записей

	_super_stat();
	void add(_prices& c); // добавить цены (сжать)
	void read(i64 n, _prices& c, _info_pak* inf = 0); // прочитать цены (расжать)
	void save_to_file(wstr fn);
	void load_from_file(wstr fn);
	void clear(); // удалить все данные

private:
	_prices last_cc; // последние цены
	_stack data; // сжатые данные
	std::vector<int> u_dd2; // указатель на место сжатых данных кратных StepPakCC

	static const int step_pak_cc = 100;
	_prices read_cc; // последние прочитанные цены
	i64 read_n; // номер последних прочитанных цен
	_info_pak ip_last, ip_n; // дополнительная информация

	void otgruzka(int rez, int Vrez, int* deko); // вспомогательная Pak()
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _element_chart // краткая информация элемента графика
{
	int n; // порядковый номер элемента !!! -1 = нет элемента
	int time; // время
	double min; // минимальное значение
	double max; // максимальное значение
	double middle; // среднее значение
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _basic_curve // база для кривых
{
	virtual int get_n()                                = 0; // количество элементов
	virtual void get_n_info(int n, _element_chart* e)  = 0; // получить краткую информацию n-го элемента
	virtual void get_t_info(int t, _element_chart* e)  = 0; // получить краткую информацию элемента со временем >= t
	virtual void draw(int n, _area area, _bitmap* bm) = 0; // нарисовать 1 элемент
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _mctds_candle : public _basic_curve // источник данных для временного графика - классические свечи
{
	struct _cen_pak // свечка
	{
		ushort min = 0, max = 0; // минимальная и макимальные цены
		ushort first = 0, last = 0; // первая и последние цены
		double cc = 0.0; // средняя цена
		_iinterval ncc; // диапазон цен
		int time = 0; // общее время
	};

	std::vector<_cen_pak> cen1m; // упакованные цены по минутам
	double c_unpak = 0.01; // распаковка цен

	int get_n(); // количество элементов
	void get_n_info(int n, _element_chart* e); // получить краткую информацию n-го элемента
	void get_t_info(int t, _element_chart* e); // получить краткую информацию элемента со временем >= t
	void draw(int n, _area area, _bitmap* bm); // нарисовать 1 элемент
	void recovery(); // выполнить
	void push(_stack* mem);
	void pop(_stack* mem);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _latest_events // последние события
{
	char event[4]; // [0] - последнее событие
	int minute[4]; // на какой минуте случилось
	double x[4]; // значения

	int start(); // ща будет рост в X минут
	bool stop(); // ща будет падение
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _nervous_oracle : public _basic_curve // нервозный предсказатель
{
	struct _element_nervous // краткая информация элемента графика
	{
		int time       = 0; // время
		_iinterval ncc;         // диапазон цен

		ushort min_pok = 0; // минимальная покупка
		ushort max_pok = 0; // максимальная покупка
		ushort min_pro = 0; // минимальная продажа
		ushort max_pro = 0; // максимальная продажа

		int    v_r     = 0; // количество слагаемых
		double r       = 0; // средний размер
		double r_pok   = 0; // средний размер покупки
		double r_pro   = 0; // средний размер продажи

		bool operator < (int a) const noexcept { return (time < a); } // для алгоритма поиска по времени
	};

	std::vector<_element_nervous> zn; // данные
	double c_unpak = 0.01; // распаковка цен

	int get_n(); // количество элементов
	void get_n_info(int n, _element_chart* e); // получить краткую информацию n-го элемента
	void get_t_info(int t, _element_chart* e); // получить краткую информацию элемента со временем >= t
	void draw(int n, _area area, _bitmap* bm); // нарисовать 1 элемент
	void recovery(); // выполнить
	void push(_stack* mem);
	void pop(_stack* mem);
	_latest_events get_latest_events(i64 nn); // получить последние события
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _oracle3 : public _basic_curve // оракул 3-я версия
{
	struct _element_oracle
	{
		int time = 0; // время
		_iinterval ncc; // диапазон цен

		ushort min = 0, max = 0; // разброс по y

		bool operator < (int a) const noexcept { return (time < a); } // для алгоритма поиска по времени
	};
	static const int max_part = 22000; // максимально количество элементов ss

	std::deque<_prices> part_ss; // часть супер-статистики
	i64 begin_ss = 0; // начало куска супер-статистики
	std::vector<_element_oracle> zn; // данные
	double c_unpak = 0.01; // распаковка цен

	int get_n(); // количество элементов
	void get_n_info(int n, _element_chart* e); // получить краткую информацию n-го элемента
	void get_t_info(int t, _element_chart* e); // получить краткую информацию элемента со временем >= t
	void draw(int n, _area area, _bitmap* bm); // нарисовать 1 элемент
	void recovery(); // выполнить
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _g_graph : public _t_go
{
	std::vector<std::unique_ptr<_basic_curve>> curve; // кривая
	bool obn = true; // обновить картинку
	i64 size_el = 6; // размер элемента

	_g_graph();

	uchar type()                     override { return 9; }
	int get_froglif()                override { return 0xF6; } // !!!!!

	void ris2(_trans tr, bool final) override;
	void run(_tetron* tt0, _tetron* tt, u64 flags) override;
	bool mouse_down_left2(_xy r) override; // начало перетаскивания
	void mouse_move_left2(_xy r) override; // процесс перетаскивания

private:
	_bitmap bm;
	i64 x_tani = 0; // предыдущая координата x при перетаскивании
	int v_vib = 0; // диапазон полосы прокрутки

	void draw(_isize size);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _area_string // область со строкой
{
	_iarea area;   // область
	std::wstring s; // строка
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _kusok_bukva // узел дерева кодирования символов
{
	static const int rc = 4; // максимально количество коллизий
	ushort mask = 0; // битовая маска
	std::vector<_kusok_bukva> dalee; // следующий столбец !!!! СЛОЖНО заменить, т.т. использует сортировку
	wchar_t c[rc]{}; // возможные символы
	char f[rc]{}; // соответствующие шрифты
	i64 nbit[rc]{}; // количество бит в символе
	int vc = 0; // количество соответствий

	void cod(ushort* aa, int vaa, wchar_t cc, char nf, i64 nbitt); // кодирование
	bool operator!=(ushort a) const noexcept { return (mask != a); } // сравнить
	bool operator==(ushort a) const noexcept { return (mask == a); } // сравнить
	bool operator< (ushort a) const noexcept { return (mask <  a); } // сравнить
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _recognize // распознавание с экрана
{
	_bitmap image; // картинка для распознавания
	POINT offset = { 0,0 }; // смещение картинки
	std::vector<_area_string> elem; // элементы

	_recognize(); // конструктор
	int read_prices_from_screen(_prices* pr); // прочитать цены с экрана (0 - без ошибок)
	int read_vvod_zaya(); // найти и распознать окно ввода заявки. возращает ошибку
	int read_vnimanie_prodaza(); // найти и распознать окно подтвержения продажи. возращает ошибку
	int read_vnimanie_pokupka(); // найти и распознать окно подтвержения продажи. возращает ошибку
	int read_tablica_zayavok(int a, int& b); // количество чисел в таблице заявок
	int read_okno_soobsenii(); // найти и распознать окно сообщений. возращает ошибку
	void find_text13(uint c); // найти текст высотой 13 с нужным цветом
	void find_text13(uint c, int err); // найти текст высотой 13 с нужным цветом с допустимой погрешностью !!! тест скорости, возможно удалить 2 другие !!!
	void find_red_text13(uint err); // найти красный текст высотой 13
	bool find_window_prices(RECT* rr); // координаты окна цен
	int find_elem(std::wstring_view s); // найти текст среди элементов
	int find_elem_kusok(wstr s); // найти кусок текста среди элементов
	int test_image(_prices* pr); // тестовый поиск на готовой картинке

private:
	_kusok_bukva bu; // дерево алфавита для распознавания

	std::wstring rasp_text(ushort* aa, i64 vaa); // распознать текст
};