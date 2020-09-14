﻿#pragma once

#include "tetron.h"

namespace mutator
{
	double get_main_scale(); // главный масштаб
	void save_to_txt_file(wstr fn);
	bool start(wstr fn);
	void draw(_isize r);

	void mouse_button_left(bool pressed);
	void mouse_button_right(bool pressed);
	void mouse_button_middle(bool pressed);
}