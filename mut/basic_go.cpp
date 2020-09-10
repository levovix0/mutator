﻿#include <chrono>

#include "basic_go.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_trans    master_trans_go;     // трансформация тяни-толкай объекта, или объекта под мышкой
_chain_go master_chain_go;      // активная цепочка графических объектов
bool      time_ris = false;     // отображать время рисования
_trans   master_trans_go_move; // трансформация n_go_move
_xy       par_koo1;             // .....вспомогательная переменная  !!!ИЗБАВИТЬСЯ!!!

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_area _t_basic_go::calc_area()
{ // рекурсия невозможна
	if (area_definite) return area;
	area_definite = true;
	if (_t_go* tgo = *this)
		area = tgo->local_area;
	else
		area.clear();
	for (auto i : link)
	{
		_tetron* a = (*i)(this);
		if (!(i->get_flags(this) & (flag_parent + flag_sub_go))) continue;
		_t_basic_go* b = *a; if (b == nullptr) continue;
		b->calc_area();
		if (i->test_flags(this, flag_parent))
			area += b->area;
		else
		{
			if (_t_trans* ttr = *b)
				area += ttr->trans(b->area);
			else
				area += b->area;
		}
	}
	return area;
}

void _t_basic_go::add_area(_area a, bool first)
{
	static _speed<_hash_table_tetron> hash(false);
	bool start = (hash.a == nullptr);
	if (start) hash.start();
	_t_go* tgo = *this;
	_t_trans* ttr = *this;
	if (first)
		a = calc_area();
	else
	{
		if (!(a <= area)) area_definite = false;
		if (n_ko == this)
		{
			add_obl_izm((tgo) ? a : ttr->trans(a));
			if (start) hash.stop(); // лишнее (предохранитель на будущее)
			return;
		}
	}
	hash->insert(this);
	for (auto i : link)
	{
		_tetron* t = (*i)(this);
		_t_basic_go* b = *t; if (b == nullptr) continue;
		if (hash->find(t)) continue; // рекурсия не допускается
		if (i->test_flags(t, flag_sub_go)) b->add_area((tgo) ? a : ttr->trans(a), false); // можно ли
		if (i->test_flags(t, flag_parent)) b->add_area(a, false); // И то И то?
	}
	hash->erase(this);
	if (start) hash.stop();
}

void _t_basic_go::cha_area(_trans tr)
{
	_t_go* tgo = *this;
	if (!tgo) return;
	add_obl_izm(tr(tgo->local_area));
}

void _t_basic_go::cha_area(_area a, bool first)
{
	static _speed<_hash_table_tetron> hash(false);
	bool start = (hash.a == nullptr);
	if (start) hash.start();
	_t_go* tgo = *this;
	_t_trans* ttr = *this;
	if (n_ko == this)
	{
		add_obl_izm((tgo) ? a : ttr->trans(a));
		if (start) hash.stop(); // лишнее (предохранитель на будущее)
		return;
	}
	if (first)
		if (tgo) a = tgo->local_area;
	hash->insert(this);
	for (auto i : link)
	{
		_tetron* t = (*i)(this);
		_t_basic_go* b = *t; if (b == nullptr) continue;
		if (hash->find(t)) continue; // рекурсия не допускается
		if (i->test_flags(t, flag_sub_go)) b->cha_area((tgo) ? a : ttr->trans(a), false); // можно ли
		if (i->test_flags(t, flag_parent)) b->cha_area(a, false); // И то И то?
	}
	hash->erase(this);
	if (start) hash.stop();
}

void _t_basic_go::del_area(_area a, bool first)
{
	static _speed<_hash_table_tetron> hash(false);
	bool start = (hash.a == nullptr);
	if (start) hash.start();
	_t_go* tgo = *this;
	_t_trans* ttr = *this;
	if (first)
		a = calc_area();
	else
	{
		if (!(a < area)) area_definite = false;
		if (n_ko == this)
		{
			add_obl_izm((tgo) ? a : ttr->trans(a));
			if (start) hash.stop(); // лишнее (предохранитель на будущее)
			return;
		}
	}
	hash->insert(this);
	for (auto i : link)
	{
		_tetron* t = (*i)(this);
		_t_basic_go* b = *t; if (b == nullptr) continue;
		if (hash->find(t)) continue; // рекурсия не допускается
		if (i->test_flags(t, flag_sub_go)) b->del_area((tgo) ? a : ttr->trans(a), false); // можно ли
		if (i->test_flags(t, flag_parent)) b->del_area(a, false); // И то И то?
	}
	hash->erase(this);
	if (start) hash.stop();
}

_trans _t_basic_go::oko_trans(bool* ko)
{
	_trans a;
	_tetron* b = this;
	_speed<_hash_table_tetron> hash;
	bool nai = true;
	while (nai)
	{
		if (_t_trans* bb = *b) a = bb->trans * a;
		hash->insert(b);
		nai = false;
		for (auto i : b->link)
		{
			_tetron* aa = (*i)(b);
			if (!i->test_flags(aa, flag_sub_go)) continue;
			if (hash->find(aa)) continue;
			if (!aa->operator _t_basic_go * ()) continue;
			b = aa;
			nai = true;
			break;
		}
	}
	if (ko) *ko = (n_ko == b);
	return a;
}

void _t_basic_go::after_create_link(_link* li)
{
	_tetron::after_create_link(li);
	if (li->test_flags(this, inverted_flags(flag_sub_go)))
		if ((*li)(this)->operator _t_basic_go * ()) add_area();
}

void _t_basic_go::before_delete_link(_link* li)
{
	_tetron::before_delete_link(li);
	if (li->test_flags(this, inverted_flags(flag_sub_go)))
		if ((*li)(this)->operator _t_basic_go * ()) del_area();
}

void _t_basic_go::set_layer(double n)
{
	_id tt = master_layers[n];
	_t_double* t;
	if (!tt)
	{
		t = new _t_double;
		t->a = n;
		t->add_flags(n_go_layer, flag_parent);
		master_layers[n] = t;
	}
	else
		t = *tt;
	t->add_flags(this, flag_information);
}

bool _t_basic_go::mouse_down_left(_trans tr)
{
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_sub_go)) continue;
		if (_t_basic_go* aa = *a)
		{
			_trans tr2;
			if (_t_go* tgo = *a)
				tr2 = tr;
			else
				tr2 = tr * a->operator _t_trans * ()->trans;
			if (!aa->area.test(tr2.inverse(mouse_xy))) continue;
			if (aa->mouse_down_left(tr2)) return true;
		}
	}
	_xy r = tr.inverse(mouse_xy);
	if (_t_go* tgo = *this)
		if (tgo->test_local_area(r))
		{
			if (tgo->key_fokus)
			{
				_tetron* b = 0;
				if (n_act_key)
					if (n_act_key != this)
						b = n_act_key;
				n_act_key = this;
				if (b) b->operator _t_basic_go* ()->cha_area();
				cha_area();
			}
			if (tgo->mouse_down_left2(r))
			{
				n_tani = this;
				master_trans_go = tr;
				return true;
			}
			if (tgo->key_fokus) return true;
		}
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_parent)) continue;
		if (_t_basic_go* aa = *a)
		{
			if (!aa->area.test(r)) continue;
			if (aa->mouse_down_left(tr)) return true;
		}
	}
	return false;
}

void _t_basic_go::find_pot_act(_xy r)
{
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_sub_go)) continue;
		if (_t_basic_go* aa = *a)
		{
			if (n_ramk == aa) continue;//?только здесь
			if (n_ramk2 == aa) continue;
			_xy r2;
			if (_t_go* tgo = *aa)
				r2 = r;
			else
				r2 = aa->operator _t_trans * ()->trans.inverse(r);
			if (!aa->calc_area().test(r2)) continue;
			aa->find_pot_act(r2);
			if (n_pot_act) return;
			continue;
		}
	}
	if (_t_go* tgo = *this)
		if (tgo->test_local_area(r))
			n_pot_act = this;
}

bool _t_basic_go::mouse_wheel(_trans tr)
{
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_sub_go)) continue;
		if (_t_basic_go* aa = *a)
		{
			_trans tr2;
			if (_t_go* tgo = *a)
				tr2 = tr;
			else
				tr2 = tr * a->operator _t_trans * ()->trans;
			if (!aa->area.test(tr2.inverse(mouse_xy))) continue;
			if (aa->mouse_wheel(tr2)) return true;
		}
	}
	_xy r = tr.inverse(mouse_xy);
	if (_t_go* tgo = *this)
		if (tgo->test_local_area(r)) // ДЕЙСТВИЕ
		{
			master_trans_go = tr;
			if (tgo->mouse_wheel2(r)) return true;
		}
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_parent)) continue;
		if (_t_basic_go* aa = *a)
		{
			if (!aa->area.test(r)) continue;
			if (aa->mouse_wheel(tr)) return true;
		}
	}
	return false;
}

bool _t_basic_go::final_fractal(const _trans& tr)
{
	if (tr(calc_area()).radius() < final_radius()) return true;
	auto h = master_chain_go.hash.find(this);
	if (!h) return false;
	return (tr.scale >= h->a.tr.scale);
}

void _t_basic_go::priem_gv()
{
	_t_go* c = *n_act;
	if (c == this) return;
	_trans tr = c->oko_trans();
	c->clear_go_rod();
	_t_trans* trr = set_t_trans(c, flag_part + flag_sub_go);
	trr->del_area();
	trr->trans = oko_trans().inverse() * tr;
	trr->area_definite = false;
	trr->add_area();
}

_t_trans* _t_basic_go::set_t_trans(_tetron* go, u64 flags)
{
	_t_trans* ttr = nullptr;
	if (link.size() <= go->link.size())
	{
		for (auto i : link)
			if (_t_trans* b = *(*i)(this))
				if (b->get_flags(go))
				{
					ttr = b;
					break;
				}
	}
	else
	{
		for (auto i : go->link)
			if (_t_trans* b = *(*i)(go))
				if (b->get_flags(this))
				{
					ttr = b;
					break;
				}
	}
	if (flags == 0)
	{
		delete ttr;
		return nullptr;
	}
	if (!ttr) ttr = new _t_trans;
	ttr->add_flags(go, flags);
	add_flags(ttr, flags);
	return ttr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _t_trans::ris(_trans tr, bool final)
{
	tr *= trans;
	if (((tr(calc_area())) & master_obl_izm).empty()) return;
	if (!final) final = final_fractal(tr);
	master_chain_go.push(this, tr);
	for (_frozen a(this, flag_parent); a; a++)
	{
		if (_t_go * b = *a)
			b->ris(tr, final);
		else
			if (_t_trans * b = *a)
				b->ris(tr / b->trans, final);
	}
	for (_layers_go a(this); a; a++) a->ris(tr, final);
	master_chain_go.pop();
}

void _t_trans::clear_go_rod()
{
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		//		if (!a->operator TGO*()) continue;
		if (!link[i]->test_flags(a, flag_sub_go)) continue;
		link[i]->~_link();
	}
}

bool _t_trans::mouse_move(_trans tr, bool final)
{
	if (!final) final = final_fractal(tr);
	master_chain_go.push(this, tr);
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_sub_go)) continue;
		if (_t_basic_go * aa = *a)
		{
			_trans tr2;
			if (_t_go * tgo = *a)
				tr2 = tr;
			else
				tr2 = tr * a->operator _t_trans * ()->trans;
			if (!aa->calc_area().test(tr2.inverse(mouse_xy))) continue;
			if (aa->mouse_move(tr2, final)) return true;
		}
	}
	_xy r = tr.inverse(mouse_xy);
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_parent)) continue;
		if (_t_basic_go * aa = *a)
		{
			if (!aa->calc_area().test(r)) continue;
			if (aa->mouse_move(tr, final)) return true;
		}
	}
	master_chain_go.pop();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _t_go::mouse_finish_move()
{
	static _vector_id var; // ??
	var.clear();
	find_all_intermediate(n_finish_mouse_move, flag_information, flag_parent, var);
	for (auto& i : var)
	{
		if (!i) continue;
		i->run(this, i, flag_run);
	}
}

bool _t_go::mouse_move2(_xy r)
{
	static _vector_id var; // ??
	if (test_flags(n_mouse_inactive, flag_information)) return false;

	if (n_go_move != this) // первое перемещение
	{
		var.clear();
		find_all_intermediate(n_start_mouse_move, flag_information, flag_parent, var);
		for (auto& i : var)
		{
			if (!i) continue;
			i->run(this, i, flag_run);
		}
	}

	return true;
}

uint _t_go::get_c()
{
	_tetron* a = find_intermediate<_tetron>(n_color_line, flag_information, flag_parent);
	if (a == nullptr) return c_def;
	if (i64 * c = *a) return (uint)* c;
	return c_def;
}

uint _t_go::get_c2()
{
	_tetron* a = find_intermediate<_tetron>(n_color_bg, flag_information, flag_parent);
	if (a == nullptr) return c2_default;
	if (i64 * c = *a) return (uint)* c;
	return c2_default;
}

void _t_go::set_c(uint c)
{
	_tetron* a = find_intermediate<_tetron>(n_color_line, flag_information, flag_parent);
	if (a != nullptr)
	{
		if (i64 * cc = *a)* cc = c;
		return;
	}
	if (c == c_def) return;
	_t_int* ti = new _t_int;
	ti->a = c;
	ti->add_flags(n_color_line, flag_parent);
	add_flags(ti, flag_information | flag_part);
}

void _t_go::set_c2(uint c)
{
	_tetron* a = find_intermediate<_tetron>(n_color_bg, flag_information, flag_parent);
	if (a != nullptr)
	{
		if (i64 * cc = *a)* cc = c;
		return;
	}
	if (c == c2_default) return;
	_t_int* ti = new _t_int;
	ti->a = c;
	ti->add_flags(n_color_bg, flag_parent);
	add_flags(ti, flag_information | flag_part);
}

_t_go::_t_go() : _t_basic_go(), key_fokus(false)
{
}

void _t_go::ris(_trans tr, bool final)
{
	if (((tr(calc_area())) & master_obl_izm).empty()) return;
	if (!final) final = final_fractal(tr);
	master_chain_go.push(this, tr);
	for (_frozen a(this, flag_parent); a; a++)
	{
		if (_t_go * b = *a)
			b->ris(tr, final);
		else
			if (_t_trans * b = *a)
				b->ris(tr / b->trans, final);
	}
	auto ris_all = [&]() {
		ris2(tr, final);
		if (!final)
			for (_layers_go a(this); a; a++) a->ris(tr, final);
	};

	if (!time_ris)
	{
		ris_all();
		master_chain_go.pop();
	}
	else
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		ris_all();
		auto t = std::chrono::high_resolution_clock::now() - t0;
		master_chain_go.pop();
		_area oo = tr(local_area);
		if (oo.x.min < 0) oo.x.min = 0;
		if (oo.y.min < 0) oo.y.min = 0;
		std::wstring s = double_to_string(t.count() / 1000000.0, 2);
		master_bm.text((int)(oo.x.min + 1), (int)oo.y.min, s.c_str(), 13, c_max, 0x00000000);
	}
}

_t_trans* _t_go::ttrans()
{
	for (auto i : link)
	{
		_tetron* a = (*i)(this);
		if (!i->test_flags(a, flag_sub_go)) continue;
		if (_t_trans * ttg = *a) return ttg;
	}
	return nullptr;
}

void _t_go::clear_go_rod()
{
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_t_trans* tr = *(*link[i])(this);
		if (!tr) continue;
		if (!link[i]->test_flags(tr, flag_sub_go + flag_part)) continue;
		delete tr;
	}
}

void _t_go::resize()
{
	_g_scrollbar* po = find1<_g_scrollbar>(flag_part);
	if (!po) return;
	po->prilip(this);
}

void _t_go::mouse_up_middle()
{
	static _vector_id var; // ??
	var.clear();
	find_all_intermediate(n_fun_up_middle, flag_information, flag_parent, var);
	for (auto& i : var)	if (i) i->run(this, i, flag_run);
}

bool _t_go::mouse_move(_trans tr, bool final)
{
	if (!final) final = final_fractal(tr);
	master_chain_go.push(this, tr);
	if (!final)
		for (int i = (int)link.size() - 1; i >= 0; i--)
		{
			_tetron* a = (*link[i])(this);
			if (!link[i]->test_flags(this, flag_sub_go)) continue;
			if (_t_basic_go * aa = *a)
			{
				_trans tr2;
				if (_t_go * tgo = *a)
					tr2 = tr;
				else
					tr2 = tr * a->operator _t_trans * ()->trans;
				if (!aa->calc_area().test(tr2.inverse(mouse_xy))) continue;
				if (aa->mouse_move(tr2, final)) return true;
			}
		}
	_xy r = tr.inverse(mouse_xy);
	if (test_local_area(r)) // ДЕЙСТВИЕ
	{
		master_trans_go = tr;
		if (mouse_move2(r))
		{
			if ((n_go_move != this) || (master_trans_go_move != master_trans_go))
			{
				if (n_go_move)
				{
					_t_go* xx = *n_go_move;
					if (xx)
					{
						if (n_ramk2 == xx) set_cursorx(_cursor::normal); else xx->mouse_finish_move();
					}
				}
				n_go_move = this;
				master_trans_go_move = master_trans_go;
			}
			return true;
		}
	}
	for (int i = (int)link.size() - 1; i >= 0; i--)
	{
		_tetron* a = (*link[i])(this);
		if (!link[i]->test_flags(this, flag_parent)) continue;
		if (_t_basic_go * aa = *a)
		{
			if (!aa->calc_area().test(r)) continue;
			if (aa->mouse_move(tr, final)) return true;
		}
	}
	master_chain_go.pop();
	return false;
}

bool _t_go::mouse_down_left2(_xy r)
{
	_tetron* a = find_intermediate<_tetron>(n_fun_tani0, flag_information, flag_parent);
	if (!a) return false;
	par_koo1 = r;
	a->run(this, a, flag_run);
	return true;
}

void _t_go::mouse_move_left2(_xy r)
{
	_tetron* a = find_intermediate<_tetron>(n_fun_tani, flag_information, flag_parent);
	par_koo1 = r;
	if (a) a->run(this, a, flag_run);
}

void _t_go::mouse_up_left2(_xy r)
{
	_tetron* a = find_intermediate<_tetron>(n_fun_tani1, flag_information, flag_parent);
	par_koo1 = r;
	if (a) a->run(this, a, flag_run);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _chain_go::push(_t_basic_go* a, _trans& tr)
{
	chain.push_back(a);
	auto n = hash.find(a);
	if (!n)
	{
		*n = { a, { 1, tr } };
		n.life();
	}
	else
		n->a.k++;
}

void _chain_go::pop()
{
	_t_basic_go* a = chain.back();
	chain.pop_back();
	auto n = hash.find(a);
	n->a.k--;
	if (n->a.k == 0) hash.erase(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _layers_go::operator++(int)
{
	for (++i_m; i_m != map_.end(); ++i_m)
	{
		_tetron* t2 = *i_m->second;
		if (!t2) continue;
		if (!tetron->test_flags(t2, flag_sub_go)) continue;
		tetron2 = (_t_basic_go*)t2;
		return;
	}
	tetron2 = nullptr;
}

_layers_go::_layers_go(_tetron* t) : tetron(t)
{
	for (auto j : t->link)
	{
		_tetron* tt = (*j)(t);
		if (!j->test_flags(t, flag_sub_go)) continue;
		if (tt->operator _t_basic_go *() == nullptr) continue;
		_t_double* ti = tt->find_intermediate<_t_double>(n_go_layer, inverted_flags(flag_information), flag_parent);
		double n = (ti) ? ti->a : 0;
		map_.insert({n, tt});
	}
	i_m = map_.begin();
	if (i_m != map_.end()) tetron2 = *i_m->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_circle::push(_wjson& b)
{
	_t_go::push(b);
	b.add("center", center);
	b.add("radius", radius);
	b.add("width", width);
}

void _g_circle::pop(_rjson& b)
{
	_t_go::pop(b);
	b.read("center", center);
	b.read("radius", radius);
	b.read("width", width);
}

void _g_circle::run(_tetron* tt0, _tetron* tt, u64 flags)
{
	_one_tetron* a = find_intermediate<_one_tetron>(n_center, flag_information, flag_parent);
	if (a) center = { a->d[0], a->d[1] };
	a = find_intermediate<_one_tetron>(n_radius, flag_information, flag_parent);
	if (a) radius = a->d[0];
	a = find_intermediate<_one_tetron>(n_width, flag_information, flag_parent);
	if (a) width = a->d[0];
	del_area();
	calc_local_area();
	area_definite = false;
	add_area();
	_t_go::run(tt0, tt, flags);
}

void _g_circle::calc_local_area()
{
	local_area = { {center.x - radius, center.x + radius}, {center.y - radius, center.y + radius} };
}

void _g_circle::ris2(_trans tr, bool final)
{
	master_bm.fill_ring(tr(center), tr(radius), tr(width), get_c(), get_c2());
}

bool _g_circle::test_local_area(_xy b)
{
	if (!local_area.test(b)) return false;
	return ((b - center).len2() <= radius * radius);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_froglif::ris2(_trans tr, bool final)
{
	_area a = tr(local_area);
	master_bm.froglif(a.top_left(), a.min_length(), f, r_f, get_c(), get_c2());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_line::run(_tetron* tt0, _tetron* tt, u64 flags)
{
	_one_tetron* a = find_intermediate<_one_tetron>(n_begin, flag_information, flag_parent);
	if (a) p1 = { a->d[0], a->d[1] };
	a = find_intermediate<_one_tetron>(n_end, flag_information, flag_parent);
	if (a) p2 = { a->d[0], a->d[1] };
	a = find_intermediate<_one_tetron>(n_width, flag_information, flag_parent);
	if (a) width = a->d[0];
	del_area();
	calc_local_area();
	area_definite = false;
	add_area();
	_t_go::run(tt0, tt, flags);
}

void _g_line::calc_local_area()
{
	local_area = (_area(p1) + p2).expansion(width * 0.5);
}

void _g_line::ris2(_trans tr, bool final)
{
	master_bm.lines(tr(p1), tr(p2), tr(width), get_c());
}

bool _g_line::test_local_area(_xy b)
{
	if (!local_area.test(b)) return false;
	_xy n = { p1.y - p2.y,p2.x - p1.x };
	n *= 1.0 / n.len();
	_xy p11 = p1 + n * (width * 0.75);
	_xy p12 = p1 - n * (width * 0.75);
	_xy p21 = p2 + n * (width * 0.75);
	_xy p22 = p2 - n * (width * 0.75);
	if (((test_line(p11, p12, b) + test_line(p21, p22, b) + test_line(p11, p21, b) + test_line(p12, p22, b)) & 1) == 0) return false;
	return true;
}

void _g_line::push(_wjson& b)
{
	_t_go::push(b);
	b.add("p1", p1);
	b.add("p2", p2);
	b.add("width", width);
}

void _g_line::pop(_rjson& b)
{
	_t_go::pop(b);
	b.read("p1", p1);
	b.read("p2", p2);
	b.read("width", width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_picture::ris2(_trans tr, bool final)
{
	if (pic.size.x * pic.size.y == 0)
	{
		int rr = 100;
		master_bm.line({ (int)tr.offset.x, (int)tr.offset.y }, { (int)(tr.offset.x + rr * tr.scale), (int)tr.offset.y },
			c_def);
		master_bm.line({ (int)(tr.offset.x + rr * tr.scale), (int)tr.offset.y },
			{ (int)(tr.offset.x + rr * tr.scale), (int)(tr.offset.y + rr * tr.scale) }, c_def);
		master_bm.line({ (int)(tr.offset.x + rr * tr.scale), (int)(tr.offset.y + rr * tr.scale) },
			{ (int)tr.offset.x, (int)(tr.offset.y + rr * tr.scale) }, c_def);
		master_bm.line({ (int)tr.offset.x, (int)(tr.offset.y + rr * tr.scale) }, { (int)tr.offset.x, (int)tr.offset.y },
			c_def);
		master_bm.line({ (int)tr.offset.x, (int)tr.offset.y },
			{ (int)(tr.offset.x + rr * tr.scale), (int)(tr.offset.y + rr * tr.scale) }, c_def);
		master_bm.line({ (int)(tr.offset.x + rr * tr.scale), (int)tr.offset.y },
			{ (int)tr.offset.x, (int)(tr.offset.y + rr * tr.scale) }, c_def);
		return;
	}
	_area oo = tr(local_area);
	master_bm.stretch_draw(&pic, (int)oo.x.min, (int)oo.y.min, tr.scale);
}

void _g_picture::new_size(int rx3, int ry3)
{
	if (!pic.resize({ rx3, ry3 })) return;
	del_area();
	local_area = { {0, (double)pic.size.x}, {0, (double)pic.size.y} };
	area_definite = false;
	add_area();
}

void _g_picture::set_pic(const _picture& pic2)
{
	pic = pic2;
	del_area();
	local_area = { {0, (double)pic.size.x}, {0, (double)pic.size.y} };
	area_definite = false;
	add_area();
}

bool _g_picture::load_from_file(wstr fn)
{
	/*	del_area();
		_stack fs;
		fs.load_from_file(fn);
		_bitmap_file bf;
		fs >> bf;
		if (!bf) return false;
		pic = std::move(bf);
		local_area = _area_old(0, (double)pic.size.x, 0, (double)pic.size.y);
		area_definite = false;
		add_area();
		return true;*/
	return true;
}

bool _g_picture::save_to_file(wstr fn) const
{
	/*	_stack fs((size_t)pic.size.x * pic.size.y * sizeof(_color));
		fs << _bitmap_file(pic);
		return fs.save_to_file(fn);*/
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_rect::ris2(_trans tr, bool final)
{
	_area oo = tr(local_area);
	uint c2 = get_c2();
	master_bm.fill_rectangle({ {(i64)oo.x.min, (i64)oo.x.max + 1}, {(i64)oo.y.min, (i64)oo.y.max + 1} }, c2);
	uint c0 = get_c();
	if (((c0 >> 24) != 0x00) && (c0 != c2)) master_bm.rectangle(oo, c0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_text::set_text(std::wstring_view s2_)
{
	del_area();
	s = s2_;
	_isize size = master_bm.size_text(s2_, 13);
	local_area = { {-1, (double)size.x}, {0, (double)size.y} };
	area_definite = false;
	add_area();
}

void _g_text::ris2(_trans tr, bool final)
{
	int sf = (int)(13 * tr.scale + 0.5);
	if (sf < 5) return;
	master_bm.text((int)tr.offset.x, (int)tr.offset.y, s.c_str(), sf, get_c(), get_c2());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void del_hint()
{
	if (!n_hint) return;
	delete_hvost(n_hint->operator _t_basic_go * ()->ttrans(), true);
}

void add_hint(std::wstring_view hint, _t_go* g)
{
	del_hint();
	if (hint.empty()) return;
	_t_trans* ko = *n_ko;
	_trans tr = master_trans_go;
	_isize siz = master_bm.size_text(hint, 13);
	tr.offset += _xy{ -siz.x * 0.5, -15.0 } +_xy{ g->local_area.x(0.5), g->local_area.y.min } *tr.scale;
	tr.scale = 1;
	_g_text* go = new _g_text;
	go->set_c(c_maxx);
	go->set_c2(c_background);
	_t_trans* ttr = new _t_trans;
	ttr->trans = ko->trans.inverse() * tr;
	ttr->add_flags(go, flag_part + flag_sub_go);
	go->set_text(hint);
	ko->add_flags(ttr, flag_part + flag_sub_go);
	n_hint = go;
}

void change_hint(std::wstring_view hint)
{
	if (!n_hint) return;
	_g_text* go = *n_hint;
	go->set_text(hint);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_test_graph::ris2(_trans tr, bool final)
{
	static bool first = true;
	if (first)
	{
		_picture te(120, 90);
		te.clear();
		for (int i = 0; i < 100; i++)
			te.line({ rnd(120), rnd(90) }, { rnd(120), rnd(90) }, 0xFF2080FF);
		first = false;
		a.clear(0xFF0000FF);
		_iarea ogr({ 30, 225 }, { 25, 125 });
		a.set_area(ogr);
		a.stretch_draw(&te, 0, 0, 3.3);
		a.rectangle(ogr, 0x80FF0000);
	}
	_area oo = tr(local_area);
	master_bm.stretch_draw(&a, (int)(oo.x.min + 20), (int)(oo.y.min + 20), 1);
}

_g_test_graph::_g_test_graph() : a(250, 150)
{
	local_area = { {0, 300}, {0, 200} };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _g_scrollbar::push(_wjson& b)
{
	_t_go::push(b);
	b.add("position", position);
	b.add("vid", vid);
}

void _g_scrollbar::pop(_rjson& b)
{
	_t_go::pop(b);
	b.read("position", position);
	b.read("vid", vid);
}

void _g_scrollbar::prilip(_t_go* r)
{
	if (!r) return;
	del_area();
	double l = ((vid & 1) == 1) ? local_area.x.length() : local_area.y.length();
	_area& o = r->local_area;
	if (vid == 2) local_area = { {o.x.min, o.x.max}, {o.y.max, o.y.max + l} };
	if (vid == 3) local_area = { {o.x.max, o.x.max + l}, {o.y.min, o.y.max} };
	if (vid == 4) local_area = { {o.x.min, o.x.max}, {o.y.min - l, o.y.min} };
	if (vid == 5) local_area = { {o.x.min - l, o.x.min}, {o.y.min, o.y.max} };
	//	if (vid > 1) trans = _trans(); //глобальная замена trans
	area_definite = false;
	add_area();
}

void _g_scrollbar::after_create_link(_link* li)
{
	if (vid > 1)
		if (li->test_flags(this, inverted_flags(flag_sub_go)))
		{
			_t_go* r = *(*li)(this);
			prilip(r);
		}
	_t_go::after_create_link(li);
}

void _g_scrollbar::ris2(_trans tr, bool final)
{
	_area a = tr(local_area);
	uint c = c_def;
	if ((vid & 1) == 0)
	{
		master_bm.line({ (int)a.x.min, (int)a.y(0.5) }, { (int)a.x.max, (int)a.y(0.5) }, c);
		master_bm.line({ (int)a.x(position), (int)a.y.min }, { (int)a.x(position), (int)a.y.max }, c);
	}
	else
	{
		master_bm.line({ (int)a.x(0.5), (int)a.y.min }, { (int)a.x(0.5), (int)a.y.max }, c);
		master_bm.line({ (int)a.x.min, (int)a.y(position) }, { (int)a.x.max, (int)a.y(position) }, c);
	}
}

void _g_scrollbar::mouse_move_left2(_xy r)
{
	double ii;
	if ((vid & 1) == 0)
		ii = (r.x - local_area.x.min) / local_area.x.length();
	else
		ii = (r.y - local_area.y.min) / local_area.y.length();
	if (ii < 0) ii = 0;
	if (ii > 1) ii = 1;
	if (ii != position)
	{
		position = ii;
		cha_area();
		run(0, this, flag_run);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
