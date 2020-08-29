﻿#pragma once

#define NOMINMAX
#include <windows.h>

#include "mgeometry.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _picture
{
	uint* data;
	_size2i size;
	bool transparent = false; // как бы от него избавиться???

	explicit _picture(int rx3 = 0, int ry3 = 0);
	_picture(const _picture&) = delete;
	_picture(_picture&& move) noexcept;
	virtual ~_picture() { delete[] data; }

	_picture& operator=(_picture&& move) noexcept;

	uint* sl(int64 y) const noexcept { return &data[y * size.x]; }

	void set_area(const _area2i &q) { area = q & size; }
	bool resize(_size2i wh);
	void set_transparent(); // узнать, есть ли прозрачные пиксели
	void invert_alpha(); // инвертировать альфа канал

	void clear(uint c = 0xFF000000);
	void line(_num2 p1, _num2 p2, uint c, bool rep = false); // линия rep - полное замещение цвета
	void lines(_coo2 p1, _coo2 p2, double l, uint c); // точная линия заданной толщины
	void text16(int64 x, int64 y, astr s, uint c); // простой текст высотой 16
	void text16n(int64 x, int64 y, astr s, int64 n, uint c); // простой текст высотой 16*n
	static _size2i size_text16(std::string_view s, int64 n = 1); // размер текста *n
	void froglif(_coo2 p, double r, uchar* f, int rf, uint c, uint c2 = 0);

	void fill_circle(double x, double y, double r, uint c);
	void fill_ring(_coo2 p, double r, double d, uint c, uint c2);
	void ring(_coo2 p, double r, double d, uint c);

	void fill_rectangle(_area2i r, uint c, bool rep = false);
	void fill_rect_d(double x1, double y1, double x2, double y2, uint c); // полупрозрачный пр-к на !!непр-й!! подложке

	void rectangle(_area2i oo, uint c);

	void draw(_num2 r, _picture &bm);
	void stretch_draw(_picture* bm, int64 x, int64 y, double m);
	void stretch_draw_speed(_picture* bm, int64 nXDest, int64 nYDest, double m);

//	void text0(int x, int y, std::string_view s, int h, uint c, uint bg);

protected:
	_area2i area; // разрешенная область для рисования

	void line_vert_rep_speed(_num2 p, _num y2, uint c); // вертикальная линия замещения без проверок диапазона

	void fill_rect_rep_speed(_area2i r, uint c); // прямоугольник - просто замена цвета без проверок диапазона
	void fill_rect_transparent_speed(_area2i r, uint c);
	void fill_rect_speed(_area2i r, uint c);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _bitmap: public _picture
{
	HBITMAP bitmap2;
	HDC hdc = 0;

	LOGFONT font;
	bool izm_font = true;// шрифт был изменен
	HFONT hfont = 0;
	uint f_c = 0; // цвет шрифта
	uint f_cf = 0; // цвет фона шрифта

	void set_font(wstr name, bool bold);
	void podg_font(int r); // подготовка шрифта в выводу
	void podg_cc(uint c, uint cf); // подготовка цветов к выводу

	explicit _bitmap(int rx3 = 0, int ry3 = 0);
	~_bitmap();

	bool resize(_size2i wh);

	void text(int x, int y, std::wstring_view s, int h, uint c, uint bg);
	void text(int x, int y, std::string_view s, int h, uint c, uint bg);
	_size2i size_text(std::wstring_view s, int h);
	_size2i size_text(std::string_view s, int h);

	void grab_ecran_oo2(HWND hwnd); // украсть часть экрана
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _hsva // цвет в формате hsv
{
	double h; // цветовой тон
	double s; // насыщенность
	double v; // яркость
	double a; // прозрачность

	_hsva() = default;
	_hsva(uint c) { *this = c; }
	_hsva(const _hsva& c) = default;
	
	void operator = (uint c);
	_hsva& operator = (const _hsva& c) = default;
	operator uint();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern _bitmap temp_bmp;

_stack& operator<<(_stack& o, _picture const& p);
_stack& operator>>(_stack& o, _picture& p);

inline uint cclow(uint c) {	return ((c >> 2) & 0x3F3F3F) + (c & 0xFF000000); }
uint brighten(uint c);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
