#include "mgraphics.h"
#include "mult.h"

_bitmap mult(1920, 1080);

constexpr double radius    = 20.0; // ������ �������
constexpr double dd        = 2.5;  // ������� ������
constexpr double ddl       = 1.5;  // ������� ����� ������
constexpr i64 v_type        = 16;   // ���������� ����� �������

constexpr i64 start_element = 400;  // ������������� ���������� �������
constexpr i64 start_link = start_element * 2; // ������������� ���������� ������
constexpr double best_dist = radius * 4; // ����������� ���������

constexpr uint color[32] =
{ 
	0xFF0080FD, 0xFFEF0000, 0xFF9E3BFF, 0xFF938700, 0xFF12AA00, 0xFFD34E0D, 0xFF7470DC,	0xFF4D9682,
	0xFFC80FCE, 0xFFB06381,	0xFFE22653, 0xFF009D93, 0xFFC4439D, 0xFF258ECB, 0xFF659B00, 0xFF3F77FF,
	0xFF887E87, 0xFF9659DD, 0xFFB57000, 0xFF39A337, 0xFF5D84BE, 0xFFAF1FFB, 0xFFDE028A, 0xFF768F51,
	0xFFB400FC,	0xFFE23700, 0xFFCB29B2, 0xFFA3755A, 0xFFC45D37, 0xFFEB171B, 0xFF966DA8, 0xFFAC4ACF
};

constexpr uint fr[16] =
{
	0xBE3B0D3E, 0xD8B09960, 0x8BC79403, 0x590FF05C, 0x8E149C5D,	0xFE3A1270, 0xB16E7870, 0xC6663333,
	0x8FD23C3A,	0xEE817919,	0x9894BA4F, 0x4AE53B30, 0xBD9AA006, 0x4FB3F6FB, 0xBA015048,	0x320CDAD2
};

double delta_signal1 = 0.125;
double delta_signal2 = 0.125;

double k_signal = 0.0; // ��������� ������� ������ �������� (0 ... 2]

uint color_signal = 0xffffffff;

struct _mult_tetron;

struct _mult_link
{
	_mult_tetron* a; // ��������� ������
	i64 f; // ��� �����
};

struct _mult_tetron
{
	_xy p{}; // �����
	i64 type{}; // ���
	std::vector<_mult_link> link; // �����
	i64 temp{};
};

struct _signal
{
	_mult_tetron* a = nullptr; // �������� ������
	std::vector<i64> li; // ����� �������� �����
};

std::vector<_mult_tetron*> element; // �������
std::vector<_signal> signal; // �������� ������

void init_mult()
{
	for (i64 i = 0; i < start_element; i++)
	{
		_mult_tetron *a = new _mult_tetron;
		a->type = rnd(v_type);
	restart:
		a->p = { rnd(mult.size.x - (i64)radius * 2) + radius, rnd(mult.size.y - (i64)radius * 2) + radius };
		for (i64 j = 0; j < i; j++)
			if ((a->p - element[j]->p).len2() < radius * radius * 6) goto restart;
		a->temp = 0;
		element.push_back(a);
	}
	for (i64 i = 0; i < start_link; i++)
	{
		_mult_tetron* a = element[rnd(element.size())];
		double min_r2 = 1e8;
		_mult_tetron* b = nullptr;
		for (auto j : element)
		{
			if (j == a) continue;
			bool ok = true;
			for (auto jj: a->link)
				if (jj.a == j)
				{
					ok = false;
					break;
				}
			if (!ok) continue;
			double r2 = (a->p - j->p).len2();
			if (r2 < min_r2)
			{
				b = j;
				min_r2 = r2;
			}
		}
		if (!b) continue;
		a->temp = 1;
		b->temp = 1;
		a->link.push_back({ b, rnd(16) });
	}
}

void distance()
{
	for (auto i : element)
	{
		_xy f = { 0, 0 };
		for (auto j : element)
		{
			if (i == j) continue;
			_xy r = j->p - i->p;
			double rr = r.len();
			double k = 0;
			if (rr > best_dist)
				k = (rr - best_dist) / (rr * rr * rr);
			else
				k = 100*(rr - best_dist) / (rr * rr * rr);
			r *= k / r.len();
			f += r;
		}
		i->p += f*100;
	}
}

void move_signal()
{
	if (signal.empty())
	{
		_signal a;
		a.a = element[rnd(element.size())];
		signal.push_back(a);
		k_signal = 0.0;
		color_signal = (uint)(rnd() | 0xff808080);
	}
	if (k_signal < 1.0)
		k_signal += delta_signal1;
	else
		k_signal += delta_signal2;
	if (k_signal == 1.0)
	{
		for (auto& i : signal)
			for (u64 j = 0; j < i.a->link.size(); j++)
				if (rnd(2)) i.li.push_back(j);
	}
	if (k_signal > 2.0)
	{
		std::vector<_mult_tetron*> element2;
		for (auto& i : signal)
			for (auto j : i.li)
			{
				_mult_tetron* a = i.a->link[j].a;
				bool est = false;
				for (auto k : element2) if (k == a) { est = true; break; }
				if (!est) element2.push_back(a);
			}
		signal.clear();
		for (auto k : element2)
		{
			_signal a;
			a.a = k;
			signal.push_back(a);
		}
		k_signal = 0.0;
	}
}

void move_mult(i64 dt)
{
	move_signal();
	distance();
}

_bitmap* draw_mult()
{
	static i64 t_pr = 0;
	i64 t = GetTickCount64();
	if (t_pr == 0) init_mult(); else move_mult(t - t_pr);
	t_pr = t;
	mult.clear();
	double y = 0.7 * (radius - dd) * 2;
	for (auto i : element)
	{
		mult.ring(i->p, radius, dd, color[i->type]);
		mult.froglif(i->p - _xy{ y / 2, y / 2 }, y, (uchar*)&fr[i->type], 2, color[i->type]);
	}
	for (auto i : element)
		for (auto j : i->link)
		{
			_xy p1 = i->p;
			_xy p2 = j.a->p;
			_xy v1 = p2 - p1;
			_xy e = -v1;
			v1 *= radius / v1.len();
			p1 += v1.rotation(-0.2);
			p2 += (-v1).rotation(0.2);
			mult.lines(p1, p2, ddl, color[16 + j.f]);
			e *= 5.0 / e.len();
			_xy e1 = e.rotation(0.3);
			_xy e2 = e.rotation(-0.3);
			mult.lines(p2, p2 + e1, ddl, color[16 + j.f]);
			mult.lines(p2, p2 + e2, ddl, color[16 + j.f]);
		}
	for (auto& i : signal)
		if (k_signal <= 1.0)
		{
			mult.ring(i.a->p, radius * k_signal, dd, color_signal);
		}
		else
			for (auto j : i.li)
			{
				_xy p1 = i.a->p;
				_xy p2 = i.a->link[j].a->p;
				_xy v1 = p2 - p1;
				_xy e = -v1;
				v1 *= radius / v1.len();
				p1 += v1.rotation(-0.2);
				p2 += (-v1).rotation(0.2);
				mult.fill_circle(p1 + (p2 - p1) * (k_signal - 1.0), ddl*2, color_signal);
			}
	return &mult;
};