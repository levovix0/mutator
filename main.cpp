﻿#include "mutator.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr wchar_t tetfile[] = L"..\\..\\tetrons.txt";
std::wstring exe_path; // путь к запущенному exe файлу
HCURSOR g_cu = LoadCursor(0, IDC_ARROW); // активный курсор

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_cursorx(_cursor x)
{
	if (x == g_cursor) return;
	HCURSOR cu = 0;
	if (x == _cursor::normal)     cu = LoadCursor(0, IDC_ARROW);
	if (x == _cursor::size_all)   cu = LoadCursor(0, IDC_SIZEALL);
	if (x == _cursor::hand_point) cu = LoadCursor(0, IDC_HAND);
	if (x == _cursor::size_we)    cu = LoadCursor(0, IDC_SIZEWE);
	if (x == _cursor::size_ns)    cu = LoadCursor(0, IDC_SIZENS);
	if (x == _cursor::drag)       cu = LoadCursor(0, IDC_UPARROW);
	if (cu)
	{
		SetCursor(cu);
		g_cu = cu;
	}
	g_cursor = x;
}

void change_window_text(HWND hwnd)
{
	static std::wstring s_old;
	wchar_t             s[100];
	swprintf(s, 100, L"%d  %4.1e", all_tetron.size, mutator::get_main_scale());
	if (s_old == s) return;
	s_old = s;
	SetWindowText(hwnd, s);
}

void paint(HWND hwnd)
{
	change_window_text(hwnd);
	HDC hdc = GetDC(hwnd);
	RECT rect;
	GetClientRect(hwnd, &rect);
	mutator::draw({ rect.right, rect.bottom });
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, master_bm.hdc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hdc);
}

void init_shift(WPARAM wparam)
{
	*n_s_shift->operator int64* ()  = wparam & MK_SHIFT;
	*n_s_alt->operator int64* ()    = false;
	*n_s_ctrl->operator int64* ()   = wparam & MK_CONTROL;
	*n_s_left->operator int64* ()   = wparam & MK_LBUTTON;
	*n_s_right->operator int64* ()  = wparam & MK_RBUTTON;
	*n_s_middle->operator int64* () = wparam & MK_MBUTTON;
	*n_s_double->operator int64* () = false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool run_timer = true;
	static bool tracking_mouse = false;
	switch (message)
	{
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(g_cu);
			return true;
		}
		break;
	case WM_MOUSEMOVE:
		if (!tracking_mouse)
		{
			tracking_mouse = true;
			TRACKMOUSEEVENT a;
			a.cbSize = sizeof(a);
			a.dwFlags = TME_LEAVE;
			a.dwHoverTime = 0;
			a.hwndTrack = hWnd;
			TrackMouseEvent(&a);
		}
		init_shift(wParam);
		mouse_xy = { ((double)(short)LOWORD(lParam)), ((double)(short)HIWORD(lParam)) };
		n_move->run(0, n_move, flag_run);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_MOUSELEAVE:
		tracking_mouse = false;
		if (*n_s_left->operator int64 * ())
		{
			*n_s_left->operator int64* () = 0;
			n_up_left->run(0, n_up_left, flag_run);
		}
		if (*n_s_right->operator int64 * ())
		{
			*n_s_right->operator int64* () = 0;
			n_up_right->run(0, n_up_right, flag_run);
		}
		if (*n_s_middle->operator int64 * ())
		{
			*n_s_middle->operator int64* () = 0;
			n_up_middle->run(0, n_up_middle, flag_run);
		}
		return 0;
	case WM_MOUSEWHEEL:
		init_shift(GET_KEYSTATE_WPARAM(wParam));
		*n_wheel->operator int64* () = GET_WHEEL_DELTA_WPARAM(wParam);
		n_wheel->run(0, n_wheel, flag_run);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
		init_shift(wParam);
		if (message == WM_LBUTTONDOWN) mutator::mouse_button_left(true);
		if (message == WM_RBUTTONDOWN) mutator::mouse_button_right(true);
		if (message == WM_MBUTTONDOWN) mutator::mouse_button_middle(true);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
		init_shift(wParam);
		if (message == WM_LBUTTONUP) mutator::mouse_button_left(false);
		if (message == WM_RBUTTONUP) mutator::mouse_button_right(false);
		if (message == WM_MBUTTONUP) mutator::mouse_button_middle(false);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_LBUTTONDBLCLK: case WM_RBUTTONDBLCLK: case WM_MBUTTONDBLCLK:
		init_shift(wParam);
		*n_s_double->operator int64* () = true;
		if (message == WM_LBUTTONDBLCLK) n_down_left->run(0, n_down_left, flag_run);
		if (message == WM_RBUTTONDBLCLK) n_down_right->run(0, n_down_right, flag_run);
		if (message == WM_MBUTTONDBLCLK) n_down_middle->run(0, n_down_middle, flag_run);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_F1)
		{
			static bool norm = true;
			if (norm)
			{
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE);
				ShowWindow(hWnd, SW_MAXIMIZE);
			}
			else
			{
				ShowWindow(hWnd, SW_SHOWNORMAL);
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
			}
			norm = !norm;
			return 0;
		}
		if (wParam == VK_F2)
		{
			run_timer = false;
			int r = MessageBox(hWnd, L"сохранить?", L"предупреждение", MB_YESNO);
			if (r == IDYES) mutator::save_to_txt_file(exe_path + tetfile);
			run_timer = true;
			return 0;
		}
		//		InitShift(Shift);
		*n_down_key->operator int64* () = wParam;
		n_down_key->run(0, n_down_key, flag_run);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_CHAR:
		*n_press_key->operator int64* () = wParam;
		n_press_key->run(0, n_press_key, flag_run);
		if (!master_obl_izm.empty()) paint(hWnd);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		paint(hWnd);
		EndPaint(hWnd, &ps);
	}
	return 0;
	case WM_TIMER:
		if (!run_timer) return 0;
		if (wParam == 1)
		{
			n_timer1000->run(0, n_timer1000, flag_run);
			if (!master_obl_izm.empty()) paint(hWnd);
		}
		return 0;
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, 0);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(hInstance, buffer, MAX_PATH);
	exe_path = buffer;
	exe_path = exe_path.substr(0, exe_path.rfind(L'\\') + 1);
	if (!mutator::start(exe_path + tetfile)) return 1;

	static TCHAR szWindowClass[] = L"win64app";
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101)); // 101 из resourse.h
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = 0;
	if (!RegisterClassEx(&wcex)) return 2;
	HWND hWnd = CreateWindow(szWindowClass, L"mutator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800,
		NULL, NULL, hInstance, NULL);
	if (!hWnd) return 3;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	set_cursorx((*n_perenos->operator int64 * ()) ? _cursor::size_all : _cursor::normal);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
